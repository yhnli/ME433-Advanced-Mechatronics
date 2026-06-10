import serial 
import sys
import time
import matplotlib.pyplot as plt
import numpy as np

# config
BAUD_RATE   = 115200
N_SAMPLES   = 500
TIMEOUT_SEC = 20
PORT        = "/dev/tty.usbmodem101"   
BAUD        = BAUD_RATE                
TIMEOUT     = TIMEOUT_SEC             

def read_lines(ser, n, timeout):
    lines = []
    deadline = time.time() + timeout
    while len(lines) < n and time.time() < deadline:
        line = ser.readline()
        if not line:
            continue
        try:
            s = line.decode("utf-8", errors="replace").strip()
        except Exception:
            continue
        if s:
            lines.append(s)
    return lines
 
with serial.Serial(PORT, BAUD, timeout=10) as ser:
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    print(f"Requesting {N_SAMPLES} samples on {PORT} …")
    ser.write(f"{N_SAMPLES}\n".encode("utf-8"))
    ser.flush()
    raw_lines = read_lines(ser, N_SAMPLES, TIMEOUT)
 
print(f"Received {len(raw_lines)} / {N_SAMPLES} lines.")
if len(raw_lines) < N_SAMPLES:
    print("Warning: fewer lines than requested — data may be incomplete.")
 
times, raw_values, filt_values = [], [], []
 
for ln in raw_lines:
    parts = ln.split()
    if len(parts) < 3:
        continue
    try:
        times.append(float(parts[0]))
        raw_values.append(float(parts[1]))
        filt_values.append(float(parts[2]))
    except ValueError:
        continue
 
if not times:
    print("No valid data parsed — check Pico output format.")
    sys.exit(1)
 
# Convert timestamps to seconds, starting from 0
times = np.array(times)
times = (times - times[0]) / 1000.0
 
raw_values  = -np.array(raw_values)
filt_values = -np.array(filt_values)
 
# sampling stats
fs = len(times) / (times[-1] - times[0])
print(f"Sample rate ≈ {fs:.1f} Hz  |  Nyquist ≈ {fs/2:.1f} Hz")
 
# fft
n   = len(raw_values)
frq = np.fft.rfftfreq(n, d=1.0 / fs)
 
Y_raw  = np.abs(np.fft.rfft(raw_values  - raw_values.mean()))  / n
Y_filt = np.abs(np.fft.rfft(filt_values - filt_values.mean())) / n
 
# plots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
fig.suptitle(f"HX711 force sensor", fontsize=13)
 
# --- FFT (log y-axis) ---
ax1.semilogy(frq, Y_raw,  color="steelblue", alpha=0.8, label="Raw")
ax1.semilogy(frq, Y_filt, color="darkorange", linewidth=2, label="IIR filtered")
ax1.set_xlabel("Frequency (Hz)")
ax1.set_ylabel("Magnitude")
ax1.set_title("FFT of raw vs filtered signal")
ax1.legend()
ax1.grid(True, which="both", linestyle="--", alpha=0.5)
 
# --- Time domain ---
ax2.plot(times, raw_values,  color="steelblue", alpha=0.7, label="Raw")
ax2.plot(times, filt_values, color="darkorange", linewidth=2, label="IIR filtered")
ax2.set_xlabel("Time (s)")
ax2.set_ylabel("HX711 reading (counts)")
ax2.set_title("Raw vs Filtered Signal")
ax2.legend()
ax2.grid(True, linestyle="--", alpha=0.5)
 
plt.tight_layout()
plt.savefig("hx711_data.png", dpi=200)
print("Saved → hx711_data.png")
plt.show()
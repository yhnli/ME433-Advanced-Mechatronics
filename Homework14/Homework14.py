import serial 
import time
import matplotlib.pyplot as plt
import numpy as np

BAUD_RATE   = 115200
N_SAMPLES   = 100
TIMEOUT_SEC = 20

OUTPUT_TIME_PLOT = "raw_filtered_time.png"
OUTPUT_FFT_PLOT  = "raw_filtered_fft.png"

ser = serial.Serial('/dev/tty.usbmodem101', BAUD_RATE, timeout=1)
print('Opening port: ', ser.name)

# sends a number of data points to collect
print(f"Requesting {N_SAMPLES} samples …\n")
ser.write(f"{N_SAMPLES}\n".encode())

timestamps, raw_vals, filtered_vals = [], [], []
receiving = False
deadline  = time.time() + TIMEOUT_SEC

# reads all the data back
while time.time() < deadline:
    line = ser.readline().decode("utf-8", errors="ignore").strip()

    if not line:
        continue

    print(f"  Pico › {line}")

    if line.startswith("START"):
        receiving = True
        continue

    if line.startswith("END"):
        print(f"\nReceived {len(timestamps)} samples.")
        break

    if line.startswith("TIMEOUT"):
        print("\nPico reported an HX711 read timeout.")
        break

    if not receiving:
        continue

    # Expected format: timestamp_ms,raw,filtered
    parts = line.split(",")
    if len(parts) != 3:
        continue

    try:
        timestamps.append(float(parts[0]))
        raw_vals.append(float(parts[1]))
        filtered_vals.append(float(parts[2]))
    except ValueError:
        print(f"  (skipped malformed line: {line!r})")

else:
    raise TimeoutError(f"No END marker within {TIMEOUT_SEC}s — check Pico firmware.")

ser.close()

# validate
if len(timestamps) < 2:
    raise RuntimeError("Too few samples received — check Pico output.")

time_ms       = np.array(timestamps)
raw_vals      = np.array(raw_vals)
filtered_vals = np.array(filtered_vals)

# sampling stats
mean_interval_ms = np.diff(time_ms).mean()
sample_rate_hz   = 1000.0 / mean_interval_ms
time_s           = time_ms / 1000.0

print(f"\nSampling statistics:")
print(f"  Mean interval : {mean_interval_ms:.3f} ms")
print(f"  Sample rate   : {sample_rate_hz:.2f} Hz")
print(f"  Duration      : {time_s[-1] - time_s[0]:.3f} s")

# time domain plot
fig, ax = plt.subplots(figsize=(10, 5))
ax.plot(time_s, raw_vals,      label="Raw",      alpha=0.7)
ax.plot(time_s, filtered_vals, label="Filtered", linewidth=2)
ax.set_xlabel("Time (s)")
ax.set_ylabel("Sensor Reading")
ax.set_title("HX711 — Raw vs Filtered Signal")
ax.legend()
ax.grid(True)
fig.tight_layout()
fig.savefig(OUTPUT_TIME_PLOT, dpi=200)
print(f"\nSaved → {OUTPUT_TIME_PLOT}")

# fft plot
def compute_fft(signal, fs):
    n           = len(signal)
    zero_mean   = signal - signal.mean()
    magnitudes  = np.abs(np.fft.rfft(zero_mean)) / n
    frequencies = np.fft.rfftfreq(n, d=1.0 / fs)
    return frequencies, magnitudes

freqs_raw,  mag_raw  = compute_fft(raw_vals,      sample_rate_hz)
freqs_filt, mag_filt = compute_fft(filtered_vals, sample_rate_hz)

fig, ax = plt.subplots(figsize=(10, 5))
ax.plot(freqs_raw,  mag_raw,  label="Raw FFT",      alpha=0.7)
ax.plot(freqs_filt, mag_filt, label="Filtered FFT", linewidth=2)
ax.set_xlabel("Frequency (Hz)")
ax.set_ylabel("Magnitude")
ax.set_title("HX711 — Frequency Spectrum (FFT)")
ax.legend()
ax.grid(True)
fig.tight_layout()
fig.savefig(OUTPUT_FFT_PLOT, dpi=200)
print(f"Saved → {OUTPUT_FFT_PLOT}")

plt.show()
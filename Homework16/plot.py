import serial
import matplotlib.pyplot as plt

# ---- CONFIG ----
PORT      = '/dev/tty.usbmodem101'
BAUD      = 115200
N_SAMPLES = 500     
KP        = 0.1
KI        = 0.1
# ----------------

# Open serial port 
ser = serial.Serial(PORT, BAUD, timeout=10)
ser.reset_input_buffer()
print(f"Opened {PORT} at {BAUD} baud")
print("Waiting for data... (press the USER button on the STM32 to send a CAN trigger)")

# Collect lines until "done" or N_SAMPLES rows received
indices = []
desired = []
actual  = []

while True:
    raw = ser.readline()
    if not raw:
        # timeout — nothing received in 10 s
        print("Timeout waiting for data. Check the CAN trigger was sent.")
        break

    line = raw.decode('utf-8', errors='ignore').strip()

    if line == 'done':
        print("Received 'done' from MCU.")
        break

    if line == '' or not line[0].lstrip('-').isdigit():
        if line:
            print(f"  [info] {line}")
        continue

    parts = line.split()
    if len(parts) != 3:
        continue

    try:
        indices.append(int(parts[0]))
        desired.append(int(parts[1]))
        actual.append(int(parts[2]))
    except ValueError:
        continue

    if len(indices) >= N_SAMPLES:
        break

ser.close()
print(f"Collected {len(indices)} samples")

if len(indices) == 0:
    print("No data received — nothing to plot.")
    raise SystemExit(1)

# ---- Plot ----
fig, axes = plt.plot(figsize=(12, 8), sharex=True)

# desired vs actual
ax = axes[0]
ax.plot(indices, desired, 'r--', linewidth=2,   label='Desired current')
ax.plot(indices, actual,  'b-',  linewidth=1.2, label='Actual current')
ax.set_ylabel('Current (units of 1/3 mA)')
ax.set_title(f'PI Current Controller  (Kp={KP}, Ki={KI})  —  triggered via FDCAN')
ax.legend(loc='upper right')
ax.grid(True, alpha=0.3)


plt.tight_layout()
plt.savefig('controller_plot.png', dpi=150)
print("Saved controller_plot.png")
plt.show()
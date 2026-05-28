import serial
 
PORT = '/dev/tty.usbmodem101'  # ← your port
BAUD = 115200
 
print(f"Opening {PORT} ...")
with serial.Serial(PORT, BAUD, timeout=3) as ser:
    print("Connected! Press the button a few times. Ctrl+C to quit.\n")
    while True:
        line = ser.readline()
        print(repr(line))   # shows exactly what bytes arrive
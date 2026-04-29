import numpy as np
import matplotlib.pyplot as plt
import csv

### HELPER FUNCTIONS ###
# function to read data from csv files
def read_csv(filename):
    # data containers
    data1 = []
    data2 = []

    with open(filename, 'r') as f:
        # open csv file
        reader = csv.reader(f)
        
        for row in reader:
            # read the rows 1 one by one
            data1.append(float(row[0])) 
            data2.append(float(row[1]))

    return np.array(data1), np.array(data2)

# function to compute fft 
def compute_fft(data, sample_rate):
    n = len(data) # length of the signal
    k = np.arange(n)
    T = n/sample_rate
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(data)/n # fft computing and normalization
    Y = Y[range(int(n/2))]

    return frq, Y

# function to apply low-pass IIR filter
def low_pass_filter(signal, A, B):
    filtered_signal = np.zeros_like(signal)
    filtered_signal[0] = signal[0] # initialize the first value

    for i in range(1, len(signal)):
        filtered_signal[i] = A * filtered_signal[i-1] + B * signal[i]

    return filtered_signal

# csv reads
A1, A2 = read_csv('sigA.csv')
B1, B2 = read_csv('sigB.csv')
D1, D2 = read_csv('sigD.csv')

signals = [
    ("A", A1, A2, "b"),
    ("B", B1, B2, "r"),
    ("D", D1, D2, "g"),
]

# plots (part 5)
plt.figure()
fig, axs = plt.subplots(3, 2, figsize=(12, 10))
for i, (name, t, x, color) in enumerate(signals):
    # time-domain plot
    axs[i, 0].plot(t, x, color, lw=0.5)
    axs[i, 0].set_title(f"Signal {name}")
    axs[i, 0].set_xlabel("Time")
    axs[i, 0].set_ylabel("Amplitude")

    # frequency-domain plot
    frq, Y = compute_fft(x, 1000)
    axs[i, 1].loglog(frq, abs(Y), color)
    axs[i, 1].set_title(f"FFT of Signal {name}")
    axs[i, 1].set_xlabel("Freq (Hz)")
    axs[i, 1].set_ylabel("|Y(freq)|")
plt.show()
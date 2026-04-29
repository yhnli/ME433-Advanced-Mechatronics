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
C1, C2 = read_csv('sigC.csv')
D1, D2 = read_csv('sigD.csv')

signals = [
    ("A", A1, A2),
    ("B", B1, B2),
    ("C", C1, C2),
    ("D", D1, D2),
]

# fft (part 5)
# plots
fig, axs = plt.subplots(4, 2, figsize=(12, 10))
for i, (name, t, x) in enumerate(signals):
    # time-domain plot
    axs[i, 0].plot(t, x, lw=0.5)
    axs[i, 0].set_title(f"Signal {name}")
    axs[i, 0].set_xlabel("Time")
    axs[i, 0].set_ylabel("Amplitude")

    # frequency-domain plot
    frq, Y = compute_fft(x, 1000)
    axs[i, 1].loglog(frq, abs(Y), lw=0.5)
    axs[i, 1].set_title(f"FFT of Signal {name}")
    axs[i, 1].set_xlabel("Freq (Hz)")
    axs[i, 1].set_ylabel("|Y(freq)|")
plt.show()

# filtering (part 6)
A_filtered = low_pass_filter(A2, 0.9, 0.1) 
B_filtered = low_pass_filter(B2, 0.9, 0.1)
C_filtered = low_pass_filter(C2, 0.9, 0.1)
D_filtered = low_pass_filter(D2, 0.9, 0.1)  
# plots for filtered signals
fig, axs = plt.subplots(4, 2, figsize=(12, 10))
for i, (name, t, x) in enumerate(signals):
    # time-domain plot of filtered signal
    if name == "A":
        axs[i, 0].plot(t, A2, lw=0.5, color='k')
        axs[i, 0].plot(t, A_filtered, lw=0.5, color='r')
    elif name == "B":
        axs[i, 0].plot(t, B2, lw=0.5, color='k')
        axs[i, 0].plot(t, B_filtered, lw=0.5, color='r')
    elif name == "C":
        axs[i, 0].plot(t, C2, lw=0.5, color='k')
        axs[i, 0].plot(t, C_filtered, lw=0.5, color='r')
    else:
        axs[i, 0].plot(t, D2, lw=0.5, color='k')
        axs[i, 0].plot(t, D_filtered, lw=0.5, color='r')
    
    axs[i, 0].set_title(f"Filtered Signal {name}")
    axs[i, 0].set_xlabel("Time")
    axs[i, 0].set_ylabel("Amplitude")

    # frequency-domain plot of filtered signal
    if name == "A":
        frq, Y = compute_fft(A_filtered, 1000)
    elif name == "B":
        frq, Y = compute_fft(B_filtered, 1000)
    else:
        frq, Y = compute_fft(D_filtered, 1000)

    axs[i, 1].loglog(frq, abs(Y), lw=0.5)
    axs[i, 1].set_title(f"FFT of Filtered Signal {name}")
    axs[i, 1].set_xlabel("Freq (Hz)")
    axs[i, 1].set_ylabel("|Y(freq)|")
plt.show()
import numpy as np
import matplotlib.pyplot as plt
import csv

### helper filters ###
# read data
def read_csv(file_name, t, signal):
    with open(file_name) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            signal.append(float(row[1])) # second column

# fft
def calc_fft(t, signal):
    Fs = 10000 # sample rate
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector
    y = np.interp(ts, t, signal) # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]

    # plot
    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(ts, y, 'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq, abs(Y), 'b')
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()

# moving average filter
def moving_avg_filter(signal, x):
    signal = np.asarray(signal, dtype=float)
    out = np.empty(len(signal))
    for i in range(len(signal)):
        start = max(0, i - x + 1)
        out[i] = signal[start : i + 1].mean()
    return out

# iir filter
def iir_filter(data, A, B):
    datanew = []
    y_prev = 0
    for i in range(len(data)):
        y = A * data[i] + (B) * y_prev
        datanew.append(y)
        y_prev = y
    return datanew

# fir filter
def fir_filter(data, h):
    n = len(data)
    m = len(h)
    datanew = []
    for i in range(n):
        y = 0.0
        for j in range(m):
            if i - j >= 0:
                y += h[j] * data[i - j]
        datanew.append(y)
    return datanew

# plot filtered signal
def plot_filtered(time, original, filtered, title):
    fig, ax = plt.subplots(figsize=(10, 6))
    fig.suptitle(title)
    ax.plot(time, original, 'k', label='Unfiltered')
    ax.plot(time[:len(filtered)], filtered, 'r', label='Filtered')
    ax.set_xlabel('Time')
    ax.set_ylabel('Amplitude')
    ax.legend()
    plt.tight_layout()
    plt.show()


# read signal csv files
t_A, signal_A = [], []
t_B, signal_B = [], []
t_C, signal_C = [], []
t_D, signal_D = [], []
read_csv('sigA.csv', t_A, signal_A)
read_csv('sigB.csv', t_B, signal_B)
read_csv('sigC.csv', t_C, signal_C)
read_csv('sigD.csv', t_D, signal_D)

# calculate and plot fft
# calc_fft(t_A, signal_A)
# calc_fft(t_B, signal_B)
# calc_fft(t_C, signal_C)
# calc_fft(t_D, signal_D)

# calculate and plot moving average filter
maf_A = moving_avg_filter(signal_A, 20)
maf_B = moving_avg_filter(signal_B, 12)
maf_C = moving_avg_filter(signal_C, 10)
maf_D = moving_avg_filter(signal_D, 8)

# plot_filtered(t_A, signal_A, maf_A, 'sigA - Moving Average Filter (X = 20 points)')
# plot_filtered(t_B, signal_B, maf_B, 'sigB - Moving Average Filter (X = 12 points)')
# plot_filtered(t_C, signal_C, maf_C, 'sigC - Moving Average Filter (X = 10 points)')
# plot_filtered(t_D, signal_D, maf_D, 'sigD - Moving Average Filter (X = 8 points)')

# calculate and plot iir filter
iir_A = iir_filter(signal_A, 0.1, 0.9)
iir_B = iir_filter(signal_B, 0.4, 0.6)
iir_C = iir_filter(signal_C, 0.1, 0.9)
iir_D = iir_filter(signal_D, 0.1, 0.9)

plot_filtered(t_A, signal_A, iir_A, 'sigA - IIR Filter (A = 0.1, B = 0.9)')
plot_filtered(t_B, signal_B, iir_B, 'sigB - IIR Filter (A = 0.4, B = 0.6)')
plot_filtered(t_C, signal_C, iir_C, 'sigC - IIR Filter (A = 0.1, B = 0.9)')
plot_filtered(t_D, signal_D, iir_D, 'sigD - IIR Filter (A = 0.1, B = 0.9)')

import numpy as np
import matplotlib.pyplot as plt
import csv

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


# FFT Parameters


# Fs = 10000 # sample rate
# Ts = 1.0/Fs; # sampling interval
# ts = np.arange(0,t[-1],Ts) # time vector
# n = len(y) # length of the signal
# k = np.arange(n)
# T = n/Fs
# frq = k/T # two sides frequency range
# frq = frq[range(int(n/2))] # one side frequency range
# Y = np.fft.fft(y)/n # fft computing and normalization
# Y = Y[range(int(n/2))]

# fig, (ax1, ax2) = plt.subplots(2, 1)
# ax1.plot(t,y,'b')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq,abs(Y),'b') # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')
# plt.show()
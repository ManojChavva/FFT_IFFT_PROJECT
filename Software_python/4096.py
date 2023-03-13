import numpy as np
import matplotlib.pyplot as plt
#If using termux
import subprocess
import shlex
#end if
import soundfile as sf
from scipy import signal
from scipy.fftpack import fft
from scipy.fftpack import ifft
import math
from math import log

#reading input .wav
input_signal,fs = sf.read('filter_codes_Sound_Noise.wav')
sampl_freq=fs

#order of the filter
order=3

#cutoff frquency 4kHz
cutoff_freq=4000.0

#digital frequency
Wn=2*cutoff_freq/sampl_freq

# b and a are numerator and denominator polynomials respectively
b, a = signal.butter(order, Wn, 'low')

#filter the input signal with butterworth filter
output_signal = signal.filtfilt(b, a, input_signal)

#write the output signal into .wav file
sf.write('Reduced_Noise.wav', output_signal, fs)
#assuming output_signal as a

x=output_signal

#a=np.ones(1226536)

print("x before padding =", x)
print("length of x before padding =", len(x))

x=np.pad(x, (0,870616))

#a=np.pad(a, (0, 2264))

print("x after padding=", x)
print("length of x after padding=", len(x))

#splitting array x into 512 sub_arrays of equal size
#then the size of each sub_array will be 4096 (=2^21/512)
#and consider each sub_array is denoted by xi (i ranges from 0 to 511)

xi=np.array_split(x, 512)
print("xi=", xi)
print("length of array xi=", len(xi))
print("length of sub_array = len(xi[0]) =", len(xi[0]))

#finding FFT and IFFT of xi

Xi_fft=fft(xi)

xi_ifft=ifft(Xi_fft)

#concatenating all sub_arrays of xi into an array "x_ifft"
x_ifft=np.concatenate(xi_ifft, axis=None)

print("x_ifft=", x_ifft.real)
print("length of x_ifft=", len(x_ifft))

#unpadding "870616" zeros from x_ifft
x_ifft=x_ifft[:1226536]
print("Unpadded x_ifft=", x_ifft.real)
print("length of upadded x_ifft=", len(x_ifft))


#writing x_ifft into sound wave file
sf.write('x_ifft.wav', x_ifft.real, 44100)


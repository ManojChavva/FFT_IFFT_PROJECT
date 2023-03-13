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
#assuming output_signal as x
#x=output_signal


#filter design

#Coefficients
#a0= 1
#a1=-1.87302725    
#a2= 1.30032695
#a3=-0.31450204

   
#b0= 0.01409971    
#b1= 0.04229913 
#b2= 0.04229913 
#b3= 0.01409971 

#defining impulse sequence  
k = 100
h = np.zeros(k)
#defining loop
v= 0.0140997 #for n<=0
h[0]=v
for n in range(1,k):
		p=(0.640832 - 3.68314*10**(-17)*1j) * 0.546882**n - (0.29095 - 0.141431*1j) * (0.663072 - 0.36799*1j)**n - (0.29095 + 0.141431*1j) *(0.663072 + 0.36799*1j)**n #for n>0
		h[n]=p.real
		

#finding output sequence by convolution operation
y=np.convolve(input_signal, h, "same")
#assuming covolution output  as x
x=y

#size of x (lx=1226536) is in between 2^20 and 2^21.  
#padding x with 0s to let its size in power of 2. 
#if 870616 zeros are padded, then size of x will become equal to 2^21. 
#(1226536+870616) = 2^21
x=np.pad(x, (0,870616))


#defining FFT and IFFT

def FFT(x):
    """
    A recursive implementation of 
    the 1D Cooley-Tukey FFT, the 
    input should have a length of 
    power of 2. 
    """
    N = len(x)
    
    if N == 1:
        return x
    else:
        X_even = FFT(x[::2])
        X_odd  = FFT(x[1::2])
        factor = \
          np.exp(-2j*np.pi*np.arange(N)/ N)
        
        X = np.concatenate(\
            [X_even+factor[:int(N/2)]*X_odd,
             X_even+factor[int(N/2):]*X_odd])
        return X

def IFFT(x):
    """
    A recursive implementation of 
    the 1D Cooley-Tukey FFT, the 
    input should have a length of 
    power of 2. 
    """
    N = len(x)
    
    if N == 1:
        return x
    else:
        X_even = IFFT(x[::2])
        X_odd  = IFFT(x[1::2])
        factor = \
          np.exp(2j*np.pi*np.arange(N)/ N)
        
        X = np.concatenate(\
            [X_even+factor[:int(N/2)]*X_odd,
             X_even+factor[int(N/2):]*X_odd])
        return X

#Finding FFT and IFFT using Defined functions
#finding FFT
X_defined=FFT(x) 

#finding IFFT
X_defined=np.conj(X_defined)

x_defined=IFFT(X_defined)
x_defined=np.conj(x_defined)/2**21

x_defined1=x_defined[:1]
x_defined2=x_defined[1:2**21]
x_defined2=x_defined[::-1]
x_defined=np.concatenate((x_defined1, x_defined2), axis=None)
x_defined=x_defined[:1226536]
x_defined=x_defined.real

#writing the output signal into .wav file
sf.write('IFFT_defined_function.wav', x_defined, 44100)

#plt.stem(x_defined)
#plt.show()

print("-----------------------------------------------")
print("***********************************************")
print("-----------------------------------------------")
print("input_signal=x(n)=", input_signal)
print("length of input signal lx=", len(input_signal))
print("Sampling frequency fs=", fs)
print("output_signal =reduced noise signal =x(n)=", output_signal)
print("length of output signal lx=", len(output_signal))
print("-----------------------------------------------")
print("***********************************************")
print("-----------------------------------------------")
print("Designing filter and finding convolution of input signal and impulse")
#printing initial impulse sequence
print("-----------------------------------------------")
print("Impulse sequence of filter =h(n) =")
print(h)
print("-----------------------------------------------")
print("length of impulse sequence of filter =lh =", len(h))
print("-----------------------------------------------")
print("output response of filter, y(n)=")
print(y)
print("-----------------------------------------------")
print("length of output sequence y(n) =", len(y))
print("-----------------------------------------------")
print("***********************************************")
print("-----------------------------------------------")
print("Output response of filter y (reduced noise signal) is assumed as x")
print("-----------------------------------------------")
print("-----------------------------------------------")
print("***********************************************")
print("-----------------------------------------------")
print("fft using defined_function:")
print("fft of x =X_defined =",X_defined)
print("-----------------------------------------------")
print("ifft using defined_function:")
print("ifft of X =x_defined =",x_defined)
print("length of output signal after defined_function_IFFT =", len(x_defined))
print("-----------------------------------------------")
print("***********************************************")
print("-----------------------------------------------")


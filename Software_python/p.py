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

h=[1,2]
x=[1,2,3,4]
y=[5,6,7,8]
z=np.concatenate((x,y), axis=None)
print(z)
a=np.convolve(x,h)
b=np.convolve(y,h)
print("a=", a)
print("b=", b)
c=np.convolve(z,h)
d=np.concatenate((a,b), axis=None)

print("c=",c)
print("d=",d)

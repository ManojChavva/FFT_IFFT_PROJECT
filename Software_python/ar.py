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




a=np.array((np.array_split(range(1228800), 300)))
print(a)
b=fft(a)
print("b=", b.real)
c=ifft(b)
print("c=", c.real)

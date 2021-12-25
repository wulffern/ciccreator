#!/usr/bin/env python3

import pandas as pd
import sys
import numpy as np
import matplotlib.pyplot as plt

def apply(x,y,func):

    if(func is None):
        return (x,y)

    if(func.startswith("fft")):
        (name,start,sample) = func.replace("n","e-9").replace("f","e-15").replace("p","e-12").split("_")


        fstart = float(start)
        fsample = float(sample)
        spls = list()
        ts = None
        for i in range(0,len(x)):
            t = x[i]
            if(ts is None and t > fstart):
                ts = t
                spls.append(y[i])
            if(ts is not None and t - ts >= fsample):
                ts = t
                spls.append(y[i])
                print(ts)
        M = len(spls)
        han = np.hanning(M+1)
        yfft = np.fft.fft(spls*han)

        N = int(len(yfft)/2)
        yfft = yfft/np.max(yfft)

        y = 20*np.log10(abs(yfft[:N]))
        x = np.linspace(0,0.5,N)



    return x,y


if(len(sys.argv) > 1):
    fname = sys.argv[1]
    df = pd.read_csv(fname,header=4)
    print("Waves: " + ", ".join(df.columns))

if(len(sys.argv) < 3):
    print("Usage: plot.py <csvfile> [<waveform,waveform>] [function]")
    exit(1)

func = None
if(len(sys.argv) > 3):
    func = sys.argv[3]

    
wave = sys.argv[2]
groups =  wave.split(":")

N = len(groups)
ind = 1
for g in groups:
    names = g.split(",")
    plt.subplot(N,1,ind)


    for name in names:
        (x,y) = apply(df["Time"],df[name],func)
        plt.plot(x,y,label=name)
    plt.legend()
    ind +=1


plt.show()

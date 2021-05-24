#!/usr/bin/env python3

import pandas as pd
import sys
import matplotlib.pyplot as plt

if(len(sys.argv) > 1):
    fname = sys.argv[1]
    df = pd.read_csv(fname,header=4)
    print("Options" + ", ".join(df.columns))

if(len(sys.argv) < 3):
    print("Usage: plot.py <csvfile> [<waveform,waveform>] ")
    exit(1)

wave = sys.argv[2]
groups =  wave.split(":")

N = len(groups)
ind = 1
for g in groups:
    names = g.split(",")
    plt.subplot(N,1,ind)


    for name in names:
        plt.plot(df["Time"],df[name],label=name)
    plt.legend()
    ind +=1

plt.xlabel("Time")
plt.show()

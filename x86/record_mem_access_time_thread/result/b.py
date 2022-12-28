#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
import sys

df = pd.read_csv(sys.argv[1])
df2 = pd.read_csv(sys.argv[2])
df = df[df>0]
df2 = df2[df2>0]
time = df.value_counts().idxmax()[0]
time2 = df2.value_counts().idxmax()[0]
print("Cache access time: "+str(int(time)))
print("DRAM access time: "+str(int(time2)))
threshold = (time+time2)/2
print("Threshold: "+str(int(threshold)))

print('\ndraw hist...')
plt.hist(df, bins=1000, alpha=0.5)
plt.hist(df2, bins=1000, alpha=0.5)
plt.xlabel('CYCLE')
plt.ylabel('HITS')
plt.title('CPU cycle of Cahce/DRAM pre memaccess')
plt.show()

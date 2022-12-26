#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
import sys

log = sys.argv[1]
title = sys.argv[2]
fig = sys.argv[3]
qua = float(sys.argv[4])

df = pd.read_csv(log)
df = df[df>0]
time = df.value_counts().idxmax()[0]
print("most access time: "+str(int(time)))

print('\nraw data:')
print(df.describe())

print('\nnew data:')
new = df[df<df.quantile(qua)]
print(new.describe())
nmax = int(new.max()[0])

print('\ndraw hist...')
plt.hist(new, range=(0,nmax),bins=50)
plt.xlabel('CYCLE')
plt.ylabel('HITS')
plt.title('CPU cycle of %s pre memaccess'%title)
plt.show()
# plt.savefig(fig)


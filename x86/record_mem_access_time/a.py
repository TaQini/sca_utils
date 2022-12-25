#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
import sys

log = sys.argv[1]
title = sys.argv[2]
fig = sys.argv[3]

df = pd.read_csv(log)
time = df.value_counts().idxmax()[0]
print("most access time: "+str(int(time)))

print('\nraw data:')
print(df.describe())

print('\nnew data:')
new = df[df<df.quantile(0.9999)]
print(new.describe())

print('\ndraw hist...')
plt.hist(new)
plt.xlabel('CYCLE')
plt.ylabel('HITS')
plt.title('CPU cycle of %s pre memaccess'%title)
# plt.show()
plt.savefig(fig)


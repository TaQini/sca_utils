#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
import sys

df = pd.read_csv(sys.argv[1])

print('\nraw data:')
print(df.describe())

print('\nnew data:')
print(df[df<df.quantile(0.9999)].describe())

print('\ndraw hist...')
plt.hist(df[df<df.quantile(0.9999)])
plt.xlabel('CYCLE')
plt.ylabel('HITS')
plt.title('CPU cycle of cache hit pre memaccess')
plt.show()


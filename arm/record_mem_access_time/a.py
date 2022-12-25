#!/usr/bin/python
import pandas as pd
import sys

df = pd.read_csv(sys.argv[1])

print('\nraw data:')
print(df.describe())

print('\nnew data:')
print(df[df<df.quantile(0.9999)].describe())


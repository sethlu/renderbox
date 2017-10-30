
# This script assumes wavelength range over 0.35-2.5 micros with 2151 channels

import sys

if len(sys.argv) != 2:
    print("Usage: python3 usgs-spectral-albedo.py <filepath>")
    exit()

f = open(sys.argv[1], "r")

f.readline()  # Skip the header line

albedo = []
c = 350
for line in f:
    if c > 780:
        break
    if c >= 380:
        albedo.append(float(line))
    c += 5

f.close()

print("{{{}}}".format(", ".join(map(str, albedo))))

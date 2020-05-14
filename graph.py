#! /usr/bin/env python3

import matplotlib.pyplot as plt

# Bandwidth: 40 Mbps
# Link delay: 1000 us

# Desired throughput, actual througput (both Mbps)
data = [
    (0.1, 0.18),
    (1, 0.997),
    (5,  4.96),
    (10, 9.76),
    (20, 19.0973),
    (30, 28.1024),
]

x, y = zip(*data)
plt.plot(x, y)
plt.plot(x, x, linestyle="dotted")
plt.show()


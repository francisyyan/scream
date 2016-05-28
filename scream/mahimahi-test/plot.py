#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

# Process data
f = open('scream.log', 'r')
time = []
frame_delay = []
owd = []
delay_target = []
bitrate = []
cwnd = []
bytes_in_flight = []

while True:
    line = f.readline()
    if not line:
        break
    data = line.split(' ')

    time.append(data[0])
    frame_delay.append(data[1])
    owd.append(data[2])
    delay_target.append(data[3])
    bitrate.append(float(data[4]) / 1000000)
    cwnd.append(data[5])
    bytes_in_flight.append(data[6])

# Plot video frame delay, OWD and delay target
fig, ax = plt.subplots()
fig.set_size_inches(10, 5)
ax.grid(True)

plt.plot(time, frame_delay, color='blue', linewidth=2, label='Video frame delay')
plt.plot(time, owd, color='red', linewidth=2, label='OWD')
plt.plot(time, delay_target, color='green', linewidth=2, label='Delay target')

plt.xlabel('Time (s)', fontsize = 18)
plt.ylabel('Frame delay and OWD (s)', fontsize = 18)
ax.set_xlim(0, 40)
xlabels = range(0, 45, 5)
ax.set_xticklabels(xlabels)

plt.xticks(fontsize = 18)
plt.yticks(fontsize = 18)

plt.legend(loc = 'best', prop = {'size':16})

fig.set_tight_layout(True)
fig.savefig('delay.pdf', format='pdf', dpi=500)

# Plot throughput
fig, ax = plt.subplots()
fig.set_size_inches(10, 5)
ax.grid(True)

plt.plot(time, bitrate, color='blue', linewidth=2)

plt.xlabel('Time (s)', fontsize = 18)
plt.ylabel('Throughput (Mbps)', fontsize = 18)
ax.set_xlim(0, 40)
xlabels = range(0, 45, 5)
ax.set_xticklabels(xlabels)

plt.xticks(fontsize = 18)
plt.yticks(fontsize = 18)

fig.set_tight_layout(True)
fig.savefig('bitrate.pdf', format='pdf', dpi=500)

# Plot cwnd and bytes in flight
fig, ax = plt.subplots()
fig.set_size_inches(10, 5)
ax.grid(True)

plt.plot(time, cwnd, color='red', linewidth=2, label='CWND')
plt.scatter(time, bytes_in_flight, s=12, color='blue', label='Bytes in flight')

plt.xlabel('Time (s)', fontsize = 18)
plt.ylabel('Network congstion control (bytes)', fontsize = 18)
ax.set_xlim(0, 40)
ax.set_ylim(0, 12000)
xlabels = range(0, 45, 5)
ax.set_xticklabels(xlabels)

plt.xticks(fontsize = 18)
plt.yticks(fontsize = 18)

plt.legend(loc = 'best', prop = {'size':16})

fig.set_tight_layout(True)
fig.savefig('cwnd.pdf', format='pdf', dpi=500)


## Reproduce [SCReAM](https://github.com/EricssonResearch/scream) (Self-Clocked Rate Adaptation for Multimedia)

### Install dependencies:

1. [Mahimahi](http://mahimahi.mit.edu/)
2. `sudo apt-get install iperf python-numpy python-matplotlib`

### To build:

```
$ ./autogen.sh
$ ./configure
$ make
```

###  To reproduce the results:

1. Enable Linux's IP forwarding for mahimahi to work:

  ```
  sudo sysctl -w net.ipv4.ip_forward=1
  ```

2. Change to the directory that contains all the experimental scripts:

  ```
  $ cd scream/scream/mahimahi-test
  ```

3. In one shell, start SCReAM and iperf servers:

  ```
  $ ./run-scream-iperf-servers.sh
  ```

4. In another shell, start SCReAM and iperf clients:

  ```
  $ ./run-scream-iperf-clients.sh
  ```

5. You can then generate the plots by running:

  ```
  $ ./plot.py
  ```

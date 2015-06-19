# CRandom
Linux Kernel Driver to Produce Pseudo Random Characters

# Install
```
$ git clone https://github.com/Stuart4/CRandom.git
$ cd CRandom
$ make
$ sudo insmod crandom.ko
```
# Run
```
$ head -c 80 /dev/crandom
```

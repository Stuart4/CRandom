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
# Sample Output
```
$ head -c 100 /dev/crandom
c=08Jso%8h3uDS$Wbmqt2KZQd2QV1Cnn97^u^c%BY!+uJEpc?4zPqSLuozqK$m!=W*T9LfJ3y9ak=Ghr02VUnvdX03U87wYd?W*Q
```

echo open uart clock
e 8051 6e
echo set GPIO 1,2 TX,RX
e 8083 08
e 8084 09
echo set clock 48MHZ
e 8043 1

echo set ring buffer
e 8054 1800
e 8056 1900
e 8058 1800
e 805a 1a00
e 805c 1b00
e 805e 1a00

echo set baud rate 
e 8052 a0
e 8053 01

echo enable uart
e 8060 81

e 1a00 68
e 1a01 65
e 1a02 6C
e 1a03 6C
e 1a04 6F
e 1a05 20
echo write tx
e 805e 1a06
echo read uart state
e 8111
e 8110

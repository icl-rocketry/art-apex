import time
import serial

ser = serial.Serial("COM12", 9600)
text = ""

n = 1000

for i in range(n):
     cc=str(ser.readline())[2:][:-5]
     l = str(time.time_ns()) + ", " + cc
     text += l
     print(l)

data = open("voltage.csv", "w")
data.write(text)
data.close()

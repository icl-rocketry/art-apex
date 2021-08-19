import socket
import struct

with open("sample_data.csv", "r") as f:
    data = (list(map(float, i.split(","))) for i in f.readlines())


IP = "192.168.1.16"
UDP_PORT = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP

sock.sendto(bytes("start", "ascii"), (IP, UDP_PORT))

for vals in data:
    arr = bytearray(45)
    struct.pack_into("b", arr, 0, 97)
    struct.pack_into("f", arr, 1+0, vals[0])
    struct.pack_into("f", arr, 1+4, vals[1])
    struct.pack_into("f", arr, 1+8, vals[2])
    struct.pack_into("f", arr, 1+12, vals[3])
    struct.pack_into("f", arr, 1+16, vals[4])
    struct.pack_into("f", arr, 1+20, vals[5])
    struct.pack_into("f", arr, 1+24, vals[6])
    struct.pack_into("f", arr, 1+28, vals[7])
    struct.pack_into("f", arr, 1+32, vals[8])
    struct.pack_into("f", arr, 1+36, vals[9])
    struct.pack_into("f", arr, 1+40, 0)
    sock.sendto(bytes(arr), (IP, UDP_PORT))

sock.sendto(bytes("end", "ascii"), (IP, UDP_PORT))

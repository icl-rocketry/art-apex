import socket
import struct
import random

IP = "192.168.1.16"
UDP_PORT = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

sock.sendto(bytes("start", "ascii"), (IP, UDP_PORT))

time = 0
altitude = 0
for i in range(100):
    time += random.uniform(45, 55)
    accel_x = random.uniform(-8, 8)
    accel_y = random.uniform(-8, 8)
    accel_z = random.uniform(-8, 8)
    gyro_x = random.uniform(-8, 8)
    gyro_y = random.uniform(-8, 8)
    gyro_z = random.uniform(-8, 8)
    mag_x = random.uniform(-8, 8)
    mag_y = random.uniform(-8, 8)
    mag_z = random.uniform(-8, 8)
    altitude += random.uniform(0, 2)

    arr = bytearray(45)
    struct.pack_into("b", arr, 0, 97)
    struct.pack_into("f", arr, 1+0, time)
    struct.pack_into("f", arr, 1+4, accel_x)
    struct.pack_into("f", arr, 1+8, accel_y)
    struct.pack_into("f", arr, 1+12, accel_z)
    struct.pack_into("f", arr, 1+16, gyro_x)
    struct.pack_into("f", arr, 1+20, gyro_y)
    struct.pack_into("f", arr, 1+24, gyro_z)
    struct.pack_into("f", arr, 1+28, mag_x)
    struct.pack_into("f", arr, 1+32, mag_y)
    struct.pack_into("f", arr, 1+36, mag_z)
    struct.pack_into("f", arr, 1+40, altitude)
    print(time, altitude)
    sock.sendto(bytes(arr), (IP, UDP_PORT))

sock.sendto(bytes("end", "ascii"), (IP, UDP_PORT))
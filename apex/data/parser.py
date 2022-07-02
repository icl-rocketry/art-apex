import pathlib
import struct
import sys
import os

header = ["Time (ms)", "Acc_x", "Acc_y", "Acc_z", "Gyro_x", "Gyro_y", "Gyro_z", "Mag_x", "Mag_y", "Mag_z", "Lin_acc_x", "Lin_acc_y", "Lin_acc_z", "Geo_mag_i", "Geo_mag_j", "Geo_mag_k", "Geo_mag_w", "Rot_vec_i", "Rot_vec_j", "Rot_vec_k", "Rot_vec_w", "Pressure", "Temperature"]

if __name__ == "__main__":
    path = sys.argv[1]

    with open(path, "rb") as file:
        records = list(struct.iter_unpack("Iffffffffffffffffffffff", file.read()))

    records = [header] + records

    fname, _ = os.path.splitext(path)    
    with open(fname + "_parsed.csv", "w") as file:
        for record in records:
            file.write(", ".join(map(str, record)) + "\n")


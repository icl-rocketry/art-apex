import pathlib
import struct
import sys
import os


if __name__ == "__main__":
    path = sys.argv[1]

    with open(path, "rb") as file:
        records = list(struct.iter_unpack("Iffffffffffffffffffffff", file.read()))
    
    folder, fname = os.path.split(path)
    with open(os.path.join(folder, fname.replace("datafile", "parsed") + ".csv"), "w") as file:
        for record in records:
            file.write(", ".join(map(str, record)))


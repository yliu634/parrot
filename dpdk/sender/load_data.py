import struct
import random

def load_data(path, unif = True):
  BUF_SIZE = 2
  vec = []
  n = 20000000
  with open(path, "rb") as fin:
        fin.seek(0)  # Start from the beginning of the file
        value_bytes = fin.read(8)  # Read the first uint64_t element
        first_value = int.from_bytes(value_bytes, byteorder='little', signed=False)
        if first_value != 200e6:
            raise ValueError("First value is not 200e6")
        item_num = 0
        while item_num < n:
            buf = fin.read(BUF_SIZE * 8)  # Read BUF_SIZE uint64_t elements
            num_read = len(buf) // 8  # Calculate the number of uint64_t elements read
            for i in range(num_read):
                value_bytes = buf[i*8:(i+1)*8]
                value = int.from_bytes(value_bytes, byteorder='little', signed=False)
                vec.append(value)
                item_num += 1
            if num_read < BUF_SIZE:
                break
  if (unif):
    vec = random.sample(vec, 50000); # maybe this is the maximum, anyway 1e5 doesn't work
  else:
    a = 0.9
    indices = [int(n * (1.0 / pow(random.random(), 1.0 / a)) - 1) for _ in range(50000)]
    vec = [vec[i % n] for i in indices]

  print(len(vec))
  return vec;
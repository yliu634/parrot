import sys
import os

# Redirect stderr to /dev/null
sys.stderr = open(os.devnull, 'w')
from scapy.all import Packet, ShortField
sys.stderr = sys.__stderr__
import time
import socket
import struct
import thread

query_rate = 1000
counter = 0
def counting():
  last_counter = 0
  while True:
    print 'Throughput:', counter - last_counter, ', Total:', counter
    last_counter = counter
    time.sleep(1)
thread.start_new_thread(counting, ())

# Define your custom header class
class myHeader(Packet):
  name = "my_header_t"
  fields_desc = [
    ShortField("short_mac_addr_t", 0)
    # ByteField("op", 0), 8 bits
    # LongField("idx", 0), 64bits
  ]

s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)
iface = 'veth0'
s.bind((iface, 0))
dst_ip = '192.168.1.2'
# dst_ip = '10.0.0.2'
dst_port = 8080
interval = 1.0 / (query_rate + 1)
try:
  while True:
    for i in range(0, 255):
      my_header = myHeader(short_mac_addr_t=i)
      header_bytes = bytes(my_header)

      # Construct the entire packet
      eth_header = struct.pack('!6s6sH', \
                              b'\xff\xff\xff\xff\xff\xff', b'\x00\x00\x00\x00\x00\x02', 0x0800)
      ip_header = struct.pack('!BBHHHBBH4s4s', 0x45, 0, 0, 0, 0, 255, 17, 0, \
                              socket.inet_aton(dst_ip), socket.inet_aton(dst_ip))
      udp_header = struct.pack('!HHHH', 12345, 12345, 0, 0)
      packet = eth_header + ip_header + udp_header + header_bytes

      # Send the packet
      # s.sendto(packet, ("192.168.1.2", 8888))
      s.send(packet)
      counter += 1
      #time.sleep(interval)
except KeyboardInterrupt:
  print("Packet sending stopped by user. Sent", counter, "packets.")
finally:
  s.close()

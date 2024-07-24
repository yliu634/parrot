import sys
import os
sys.stderr = open(os.devnull, 'w')
from scapy.all import *
sys.stderr = sys.__stderr__
from load_data import load_data

names = [
  "wiki","fb","osm","book"
]
paths = [
    "../datasets/wiki_ts_200M_uint64",
    "../datasets/fb_200M_uint64",
    "../datasets/osm_cellids_200M_uint64",
    "../datasets/books_200M_uint64"
]
unif = False;
# Make sure your data cannot beyond "uint48_t", which is a limit of mac address
for idx in range(0,2):
  vec = load_data(paths[idx], unif)
  packets = []
  for i in range(len(vec)):
      dst_ethernet = ":".join("{:02x}".format(c) for c in vec[i].to_bytes(6, byteorder='big'))
      # Calculate the size of the payload (128 bytes - IP header size - UDP header size)
      if (i < 10):
        print(dst_ethernet)
      payload_size = 128 - (len(IP()) + len(UDP()))
      # Create a UDP packet with a payload of zeros to fill the remaining space
      udp_payload = b'\x00' * payload_size
      pkt = Ether(dst=dst_ethernet)/IP(dst="192.168.1.1")/UDP()/Raw(load=udp_payload)
      packets.append(pkt)
  # Write packets to a pcap file
  if (unif):
    wrpcap("./pcap_files/"+names[idx]+"_unif.pcap", packets)
  else:
    wrpcap("./pcap_files/"+names[idx]+"_zipf.pcap", packets)

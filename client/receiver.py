import socket
import struct
import time
import thread

# Create a raw socket
s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.htons(0x0800))
# Set the interface name
iface = 'veth3'
# Bind the socket to the interface
s.bind((iface, 0))

counter = 0
def counting():
  last_counter = 0
  while True:
    print 'Receiver Throughput:', counter - last_counter, ', Total:', counter
    last_counter = counter
    time.sleep(1)
thread.start_new_thread(counting, ())

try:
  while True:
    # Receive a packet
    packet, addr = s.recvfrom(512)

    # Extract the Ethernet header
    eth_header = packet[:14]
    eth = struct.unpack('!6s6sH', eth_header)
    eth_dest_mac = ':'.join(['{:02x}'.format(ord(x)) for x in eth[0]])
    eth_src_mac = ':'.join(['{:02x}'.format(ord(x)) for x in eth[1]])
    eth_type = eth[2]

    if eth_type == 0x0800:  # IPv4 packet
      # Extract the IP header
      ip_header = packet[14:34]
      ip = struct.unpack('!BBHHHBBH4s4s', ip_header)
      ip_version = ip[0] >> 4
      ip_ihl = (ip[0] & 0x0F) * 4
      ip_length = ip[2]
      ip_src = socket.inet_ntoa(ip[8])
      ip_dst = socket.inet_ntoa(ip[9])
      print("Received IPv4 packet: Source MAC={}, Destination MAC={}, Source IP={}\
            , Destination IP={}".format(eth_src_mac, eth_dest_mac, ip_src, ip_dst))
    counter = counter + 1;
except KeyboardInterrupt:
  pass
finally:
  s.close()

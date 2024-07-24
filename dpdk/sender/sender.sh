#!/bin/bash
sudo pktgen -c 0x30 --file-prefix pg2 --proc-type auto --socket-mem 512 -- -m "[5-6].0" -p 0x4 -f ./pktgen.txt
sudo pktgen -l 40-41 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -m "[41-42].0" -p 0x1 -f ./pcap_files/wiki_unif.pcap
sudo pktgen -l 40-41 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -m "[41-42].0" -p 0x1 -f ./pcap_files/fb_unif.pcap
sudo pktgen -l 40-41 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -m "[41-42].0" -p 0x1 -f ./pcap_files/osm_unif.pcap
sudo pktgen -l 40-41 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -m "[41-42].0" -p 0x1 -f ./pcap_files/book_unif.pcap


sudo sh -c 'echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages'

sudo pktgen -l 40-41 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -m "[41-42].0" -p 0x1 -s 0:/home/yiliu/build/parrot/dpdk/pcap_files/wiki_unif.pcap
sudo ./build/cuckoo_l2fwd -c 0x03 --file-prefix pg2 --proc-type auto --socket-mem 512 -- -p 0x4

# make sure the l2fwd port is 2 (0x03)


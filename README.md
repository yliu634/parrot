## Parrot hashing

This is a novel hashing scheme used for l2 forwarding in computer networking, as well as its counterparts like Ludo hashing, othello hashing, and chained based learned index.

To run it for lookup performance, you gotta download datasets first with the script setup_build.sh:

```
mkdir build
cd build && cmake ..
make -j4
./benchs/bench
```

If you want to evalute its insertion and mem, you can try:
```
./benchs/mem
```

For dpdk test, provided we got two mlnx in your server, for sender and receiver you can go:
```
sudo pktgen -c 0x30 --file-prefix pg2 --proc-type auto --socket-mem 512 -- -m "[5-6].0" -p 0x4 -f ./pktgen.txt
sudo ./build/parrot_l2fwd -c 0x03 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -p 0x1

```
Cool

# Debug in ubuntu:

After you set ENV varibales, run tofino model with veths and check the stage usage  
```
cd tna_parrot_hash/  
sudo ../veth_setup.sh  
sudo -E ../p4_build.sh tna_parrot_hash.p4 --with-tofino  
sudo -E ../run_tofino_model.sh -p tna_parrot_hash -f ports.json  
sudo -E ../run_switchd.sh -p tna_parrot_hash  
(optional) sudo -E ../run_bfshell.sh  
```

If you want to send only one pkt with scapy, you can try this with the new terminal  
```
sudo scapy  
sendp(Ether(src='11:11:11:11:11:11', dst='00:00:00:00:00:02') / IP(src='10.0.0.1', dst='192.168.1.2') / UDP(dport=12345) / '00', iface='veth1')  
sniff(iface='veth3', prn=lambda x: x.summary())  
```

# Run code on the Tofino switch:

Same you have to set ENV varibales, but without veth and model running aftermath  
```
sudo -E ../p4_build.sh tna_parrot_hash.p4 --with-tofino  
sudo -E ../run_switchd.sh -p tna_parrot_hash  
```

You better open the Port number on your own or with script port.py  
```
sudo -E ../run_bfshell.sh  
ucli  
pm  
port-add 8/- 40G NONE  
port-enb 8/-  
port-add 16/- 40G NONE  
port-enb 16/-  
```

Do not forget to run controller if you have runtime control logic or initial register/table set  
```
sudo -E python ./controller/controller.py --switch-mac  --switch-ip   
```

For throughput test, go with client/server.cc to send pkts with pktgen/dpdk-20.11 based  
```
cd dpdk_code
make
sudo sh -c 'echo 512 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages'
sudo ./build/server -c 0x03 --file-prefix pg1 --proc-type auto --socket-mem 512 -- -p 0x1
sudo ./build/client -c 0x30 --file-prefix pg2 --proc-type auto --socket-mem 512 -- -p 0x4
```
If you prefer to use pktgen to be dpdk sender:
```
sudo pktgen -c 0x30 --file-prefix pg2 --proc-type auto --socket-mem 512 -- -m "[5-6].0" -p 0x4 -f ./pktgen.txt
```
Good Luck!

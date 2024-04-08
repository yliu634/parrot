#!/bin/bash
sudo pktgen -c 0x30 --file-prefix pg2 --proc-type auto --socket-mem 512 -- -m "[5-6].0" -p 0x4 -f ./pktgen.txt

for i in 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144; do for j in 0 2 3 4; do ./randmst 0 $i 5 $j; done; done > $1
killall src dst
./dst udp://127.0.0.1:8001 >1.log &
./dst udp://127.0.0.1:8002 >2.log &
./src udp://127.0.0.1:8001 udp://127.0.0.1:8002 > 0.log &

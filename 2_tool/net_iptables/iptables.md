example
===============
````
iptables -I INPUT -p icmp -j ACCEPT
iptables -I INPUT -p tcp -m state --state NEW --dport 22 -j ACCEPT
iptables -I INPUT -p tcp -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -I INPUT -p udp -m multiport --destination-port 10000,10001,20000,20001 -j ACCEPT
iptables -A INPUT -p all -j reject-with icmp-host-prohibited
````
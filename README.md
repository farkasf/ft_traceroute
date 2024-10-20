# ft_traceroute
This project will make you recode the traceroute command.

## Project Plan

1. [❌] create a command-line argument parser to validate user input
   * target IP, number of probes (`-q`), max hops (`-m`), first hop (`-f`), pause time (`-z`), no IP domain resolution (`-n`)
2. [❌] cesolve the target's IPv4 address
   * convert the domain name or provided hostname to an IPv4 address using `getaddrinfo()`
3. [❌] create the UDP socket on the base port (default 33434)
4. [❌] initialize the probe packets
   * send packets starting from the base port (default 33434) up to `base + (max hops - 1)`
   * start with a TTL of 1 and increase until the max hops or the destination is reached
   * send the desired number of probes (default 3) per TTL value
5. [❌] track time and wait for socket responses
   * measure the round-trip time (RTT) for each probe
   * implement a timeout mechanism (default 1000 ms)
6. [❌] receive and process ICMP response packets
   * handle `ICMP Time Exceeded` and `ICMP Port Unreachable` messages
7. [❌] convert router IP addresses into hostnames
   * if possible, perform a reverse DNS lookup using `getnameinfo()`
8. [❌] check if the final destination is reached
   * yes? Close sockets and exit
   * no? Increment the TTL and send the next round of probes


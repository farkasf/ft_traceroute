# ft_traceroute
`ft_traceroute` is a C-based implementation of the classic `traceroute` command. The project is designed to trace the path packets take across an IP network by sending probe packets and processing ICMP responses.

## features
- command-line argument parsing with option flags
   * `-q`: set number of probes per TTL (default: 3)
   * `-m`: set maximum hops (default: 30)
   * `-f`: set first hop TTL (default: 1)
   * `-p`: set the destination port (default: 33434)
   * `-n`: disable reverse DNS lookup for addresses
   * `--help`: display usage information
- IPv4 address resolution using `getaddrinfo()`
- UDP packet generation and ICMP response handling
- time-to-live (TTL) management for probe routing
- round-trip time (RTT) calculation for each probe
- optional reverse DNS lookup for IP addresses

## usage
Clone and compile the project:
``` bash
git clone https://github.com/farkasf/ft_traceroute
cd ft_traceroute
make
```

Run ft_traceroute:
``` bash
./ft_traceroute [options] <target_ip_or_domain>
```

Example:
``` bash
./ft_traceroute google.com -q 3 -m 15 -f 5 -n
```
   * runs 3 probes per hop, starting from TTL 5 and going up to 15, without resolving IP addresses to domain names.

## project plan

1. [✅] create a command-line argument parser to validate user input
   * target IP, number of probes (`-q`), max hops (`-m`), first hop (`-f`), default port (`-p`), no IP domain resolution (`-n`) and help (`--help`)
2. [✅] resolve the target's IPv4 address
   * convert the domain name or provided hostname to an IPv4 address using `getaddrinfo()`
3. [✅] create the UDP socket on the base port (default 33434)
4. [✅] initialize the probe packets
   * send packets starting from the base port (default 33434) up to `base + (max hops - 1)`
   * start with a TTL of 1 and increase until the max hops or the destination is reached
   * send the desired number of probes (default 3) per TTL value
5. [✅] receive and process ICMP response packets
   * implement a timeout mechanism (default 1000 ms)
   * handle `ICMP Time Exceeded` and `ICMP Port Unreachable` messages
6. [✅] convert router IP addresses into hostnames
   * if possible, perform a reverse DNS lookup using `getnameinfo()`
7. [✅] check if the final destination is reached
   * yes? close sockets and exit
   * no? increment the TTL and send the next round of probes
8. [✅] track time and wait for socket responses
   * measure the round-trip time (RTT) for each probe
9. [✅] implement solutions for each of the supported options

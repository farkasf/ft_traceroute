#!/bin/bash

TROUTE_CMD="./ft_traceroute"
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

passed_tests=0
total_tests=0
full_count=0

if [ "$EUID" -ne 0 ]; then
    echo -e "${RED}Error:${NC} This script must be run as root."
    exit 1
fi

echo -e "\n${YELLOW}   __ _     _                                      _       
  / _| |   | |                                    | |      
 | |_| |_  | |_ _ __ __ _  ___ ___ _ __ ___  _   _| |_ ___ 
 |  _| __| | __| '__/ _\` |/ __/ _ \\ '__/ _ \\| | | | __/ _ \\
 | | | |_  | |_| | | (_| | (_|  __/ | | (_) | |_| | ||  __/
 |_|  \\__|  \\__|_|  \\__,_|\\___\\___|_|  \\___/ \\__,_|\\__\\___|
       ______                                             
      |______|                        ${NC}tester by ffarkas\n\n"

run_test() {
    local description="$1"
    local command="$2"
    local expected_output="$3"

    total_tests=$((total_tests + 1))

    echo -e "${YELLOW}TEST ${total_tests}:${NC} $description"
    echo -e "${YELLOW}COMMAND:${NC} $command"

    output=$(eval "$command" 2>&1)
    echo -e "${BLUE}$output${NC}"

    if echo "$output" | grep -q "$expected_output"; then
        echo -e "\n${GREEN}[✓ ${total_tests}/20] PASSED${NC}\n"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "\n${RED}[x ${total_tests}/20] FAILED${NC}\n"
        echo -e "${RED}expected: $expected_output${NC}"
        echo -e "${RED}got: $output${NC}\n"
    fi
}

run_test "Traceroute valid IP #1" "$TROUTE_CMD 8.8.8.8" "dns.google"
run_test "Traceroute valid IP #2" "$TROUTE_CMD 1.1.1.1" "one.one.one.one"

run_test "Traceroute reserved IP" "$TROUTE_CMD 192.0.2.1 -m 11" "11  * * *"
run_test "Traceroute private IP" "$TROUTE_CMD 192.168.0.1 -m 13" "13  * * *"
run_test "Traceroute invalid IP #1" "$TROUTE_CMD 999.999.999.999" "Name or service not known"
run_test "Traceroute invalid IP #2" "$TROUTE_CMD 256.0.0.1" "Name or service not known"

run_test "Traceroute valid hostname #1" "$TROUTE_CMD google.com" "in-f14.1e100.net"
run_test "Traceroute valid hostname #2" "$TROUTE_CMD 42prague.com" "ludvik.izon.cz"

run_test "Traceroute invalid hostname #1" "$TROUTE_CMD invalid_hostname" "Name or service not known"
run_test "Traceroute invalid hostname #2" "$TROUTE_CMD hashtag#.com" "Name or service not known"

run_test "Traceroute valid, non-traceable hostname #1" "$TROUTE_CMD facebook.com -m 20" "20  * * *"
run_test "Traceroute valid, non-traceable hostname #2" "$TROUTE_CMD wikipedia.org -m 20" "20  * * *"

run_test "Traceroute valid IP with first_ttl hop set to 10" "$TROUTE_CMD 8.8.8.8 -f 10" "dns.google"
run_test "Traceroute valid hostname with first_ttl hop set to 5" "$TROUTE_CMD 42prague.com -f 5" "ludvik.izon.cz"

run_test "Traceroute valid IP with max number of hops set to 10" "$TROUTE_CMD 8.8.8.8 -m 10" "10  "
run_test "Traceroute valid hostname with max number of hops set to 5" "$TROUTE_CMD 42prague.com -m 10" "5  "

run_test "Traceroute valid IP with number of probes set to 5" "$TROUTE_CMD 8.8.8.8 -q 5" "dns.google"
run_test "Traceroute valid hostname with number of probes set to 1" "$TROUTE_CMD 42prague.com -q 1" "ludvik.izon.cz"

run_test "Traceroute valid IP with destination port set to 33435" "$TROUTE_CMD 8.8.8.8 -p 33435" "dns.google"
run_test "Traceroute valid hostname with destination port set to 33500" "$TROUTE_CMD 42prague.com -p 33500" "ludvik.izon.cz"

echo -e "\n${YELLOW}SUMMARY:${NC} $passed_tests out of $total_tests tests passed.\n"

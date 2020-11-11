#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
n="6"

make || exit

if [ $# -eq 1 ]
then
	i=$1
	rm -f dijkstraTests/Dijkstra$i.out
	./testDijkstra graphs/$i.in > dijkstraTests/Dijkstra$i.out
	r=`diff dijkstraTests/Dijkstra$i.out dijkstraTests/Dijkstra$i.exp`
	
	if [[ "$r" == "" ]]
	then
		echo -e "====== ${GREEN}[$i] Output Matches${NC} ======"
	else
		echo -e "=========== ${RED}[$i] Output Mismatch${NC} ============"
		echo -e "${RED}(your output on left, expected on right)${NC}"
        diff -y -B dijkstraTests/Dijkstra$i.out dijkstraTests/Dijkstra$i.exp
        echo -e "${RED}Your output in: ./dijkstraTests/Dijkstra$i.out${NC}"
        echo -e "${RED}Expected output in: ./dijkstraTests/Dijkstra$i.exp${NC}"
	fi
	
	exit
fi

for i in `seq 1 $n`
do
	rm -f dijkstraTests/Dijkstra$i.out
    ./testDijkstra graphs/$i.in > dijkstraTests/Dijkstra$i.out
    r=`diff dijkstraTests/Dijkstra$i.out dijkstraTests/Dijkstra$i.exp`
    
	if [[ "$r" == "" ]]
	then
		echo -e "====== ${GREEN}[$i] Output Matches${NC} ======"
	else
		echo -e "=========== ${RED}[$i] Output Mismatch${NC} ============"
		echo -e "${RED}(your output on left, expected on right)${NC}"
        diff -y -B dijkstraTests/Dijkstra$i.out dijkstraTests/Dijkstra$i.exp
        echo -e "${RED}Your output in: ./dijkstraTests/Dijkstra$i.out${NC}"
        echo -e "${RED}Expected output in: ./dijkstraTests/Dijkstra$i.exp${NC}"
	fi
done


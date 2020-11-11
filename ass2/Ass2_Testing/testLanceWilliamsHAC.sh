#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [ $# -eq 1 ]
then
    make testLanceWilliamsHAC || exit

    i=$1

    case $i in
        1|2|3|4)
            rm -f lanceWilliamsHACTests/LW$i.out
            ./testLanceWilliamsHAC graphs/$i.in | sort -n > lanceWilliamsHACTests/LW$i.out
            r=`diff -B lanceWilliamsHACTests/LW$i.out lanceWilliamsHACTests/LW$i.exp`

            if [[ "$r" == "" ]]
            then
                echo -e "====== ${GREEN}[$i] Output Matches${NC} ======"
            else
                echo -e "=========== ${RED}[$i] Output Mismatch${NC} ============"
                echo -e "${RED}(your output on left, expected on right)${NC}"
                diff -y -B lanceWilliamsHACTests/LW$i.out lanceWilliamsHACTests/LW$i.exp
                echo -e "${RED}Your output in: ./lanceWilliamsHACTests/LW$i.out${NC}"
                echo -e "${RED}Expected output in: ./lanceWilliamsHACTests/LW$i.exp${NC}"
            fi
            ;;
        *)
            echo "Usage: ./testLW.sh [1|2|3|4]"
            ;;
    esac
else
    echo "Usage: ./testLW.sh [1|2|3|4]"
fi


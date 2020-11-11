#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
n="6"

make || exit

if [ $# -eq 1 ]
then
    i=$1
    for t in 'c' 'b' 'bn'
    do
    	rm -f centralityMeasuresTests/$i$t.out
        ./testCentralityMeasures graphs/$i.in $t > centralityMeasuresTests/$i$t.out
        r=`diff centralityMeasuresTests/$i$t.out centralityMeasuresTests/$i$t.exp`
        if [[ "$r" == "" ]]
        then
            echo -e "====== ${GREEN}[$i][$t] Output Matches${NC} ======"
        else
            echo -e "=========== ${RED}[$i][$t] Output Mismatch${NC} ============"
            echo -e "${RED}(your output on left, expected on right)${NC}"
		    diff -y -B centralityMeasuresTests/$i$t.out centralityMeasuresTests/$i$t.exp
		    echo -e "${RED}Your output in: ./centralityMeasuresTests/$i$t.out${NC}"
		    echo -e "${RED}Expected output in: ./centralityMeasuresTests/$i$t.exp${NC}"
        fi
    done
    exit
elif [ $# -eq 2 ]
then
    i=$1
    t=$2
    rm -f centralityMeasuresTests/$i$t.out
    ./testCentralityMeasures graphs/$i.in $t > centralityMeasuresTests/$i$t.out
    r=`diff centralityMeasuresTests/$i$t.out centralityMeasuresTests/$i$t.exp`
    
    if [[ "$r" == "" ]]
    then
        echo -e "====== ${GREEN}[$i][$t] Output Matches${NC} ======"
    else
        echo -e "=========== ${RED}[$i][$t] Output Mismatch${NC} ============"
        echo -e "${RED}(your output on left, expected on right)${NC}"
	    diff -y -B centralityMeasuresTests/$i$t.out centralityMeasuresTests/$i$t.exp
	    echo -e "${RED}Your output in: ./centralityMeasuresTests/$i$t.out${NC}"
	    echo -e "${RED}Expected output in: ./centralityMeasuresTests/$i$t.exp${NC}"
    fi
    exit
fi

for i in `seq 1 $n`
do
    for t in 'c' 'b' 'bn'
    do
    	rm -f centralityMeasuresTests/$i$t.out
        ./testCentralityMeasures graphs/$i.in $t > centralityMeasuresTests/$i$t.out
        r=`diff centralityMeasuresTests/$i$t.out centralityMeasuresTests/$i$t.exp`
        
        if [[ "$r" == "" ]]; then
            echo -e "====== ${GREEN}[$i][$t] Output Matches${NC} ======"
        else
            echo -e "=========== ${RED}[$i][$t] Output Mismatch${NC} ============"
            echo -e "${RED}(your output on left, expected on right)${NC}"
		    diff -y -B centralityMeasuresTests/$i$t.out centralityMeasuresTests/$i$t.exp
		    echo -e "${RED}Your output in: ./centralityMeasuresTests/$i$t.out${NC}"
		    echo -e "${RED}Expected output in: ./centralityMeasuresTests/$i$t.exp${NC}"
        fi
    done
done


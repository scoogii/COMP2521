#!/bin/sh

REFERENCE_IMPLEMENTATION="/web/cs2521/20T3/labs/week05/reference/runway"

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET_COLOR="\033[0m"

tests="$@"
if [ $# -eq 0 ]
then
    tests="1 2 3 4 5"
fi

echo "Compiling..."
make || exit 1
echo

for test in $tests
do
    case $test in
        1|2|3|4|5|6)
            case $test in
                1) echo "rotateLeft tests"  ;;
                2) echo "rotateRight tests" ;;
                3) echo "doInsert tests"    ;;
                4) echo "TreeFloor tests"   ;;
                5) echo "TreeCeiling tests" ;;
                6) echo "ScheduleAdd tests" ;;
            esac

            case $test in
                6)
                    if [ ! -x "$REFERENCE_IMPLEMENTATION" ]
                    then
                        echo "Reference implementation not found. Are you on CSE?"
                        exit 1
                    fi

                    "$REFERENCE_IMPLEMENTATION" -e < commands.txt > tests/$test.exp
                    ./runway -e < commands.txt > tests/$test.out
                    ;;

                *) 
                    ./testTree $test > tests/$test.out
                    ;;
            esac

            if diff tests/$test.exp tests/$test.out > /dev/null
            then
                printf "${GREEN}Tests passed\n$RESET_COLOR"
            else
                printf "${RED}Tests failed\n$RESET_COLOR"
                printf "${YELLOW}Check differences between tests/$test.exp (expected output) and tests/$test.out (your output)\n$RESET_COLOR"
            fi
            echo
            ;;

        *)
            echo "Invalid test number '$test'"
            ;;
    esac
done

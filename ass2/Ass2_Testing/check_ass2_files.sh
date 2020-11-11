#!/bin/bash

MYFILES="  Dijkstra.c CentralityMeasures.c  LanceWilliamsHAC.c  "
ls -l $MYFILES

for i in $MYFILES
do
echo "--------------------------------- ---------------------------------"
cat $i
echo "--------------------------------- ---------------------------------"

done 

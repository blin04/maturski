#!/bin/bash

cat /dev/null > results/params

for i in {1..100};
do
    echo $i
    bin/main >> ./results/params
done

#!/bin/bash

cat /dev/null > results/params

for i in {1..10};
do
    echo $i
    bin/main >> ./results/params
done
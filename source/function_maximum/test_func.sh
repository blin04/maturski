#!/bin/bash

read filename

for i in {1..100};
do
    echo $i
    bin/main >> './results/'$filename 
done
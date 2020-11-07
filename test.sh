#!/bin/bash

#make clean
#make

rm results.out
touch results.out

while read line; do
    echo "$line" <<<EOF | ./mycc >> results.out
    #echo "$line"
done < tests.in

answer=$(diff solutions.out results.out)

if [ -z "$answer" ]; then
    echo "All tests passed!"
else
    echo "Failure"
    echo "$answer"
fi

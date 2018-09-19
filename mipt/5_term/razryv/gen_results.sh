\#!/bin/sh

mkdir -p results/coefs
mkdir -p results/root_intervals

echo "Calculating 1 var. coefs..."

for i in $(seq 8)
	do
		./razryv -1 -q -c < data/var_1.$i > results/coefs/coefs_1.$i
done

echo "Calculating 2 var. coefs..."

for i in $(seq 8)
        do
                ./razryv -2 -q -c < data/var_2.$i > results/coefs/coefs_2.$i
done

echo "Calculating 1 var. root intervals..."
\
for i in $(seq 8)
	do
		./razryv -1 -q -i -s 1000000 < data/var_1.$i > results/root_intervals/root_intervals_1.$i
done


echo "Done"

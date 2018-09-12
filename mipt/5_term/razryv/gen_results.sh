\#!/bin/sh

mkdir -p results/coefs

echo "Calculating 1 var. coefs..."

for i in $(seq 8)
	do
		./razryv -1 -q -c < data/var_1.$i > results/coefs/coefs_1.$i
done

echo "Done"

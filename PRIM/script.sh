#!/bin/bash
for i in `seq 1 100`;
do
	touch out.txt
	./prim ../data/point'('$i')'.txt >> out.txt
	
done
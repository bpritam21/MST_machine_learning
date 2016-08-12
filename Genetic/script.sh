#!/bin/bash
for i in `seq 1 100`;
do
	touch out.txt
	./genetic ../data/point'('$i')'.txt 1 >> out.txt
	
done
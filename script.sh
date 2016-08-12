#!/bin/bash
for i in `seq 1 100`;
do
	touch data/point'('$i')'.txt
	./random data/point'('$i')'.txt
	sleep 1
	./weed data/point'('$i')'.txt

	if [ $? -eq 1 ]
	then
		rm data/point'('$i')'.txt
	fi
done
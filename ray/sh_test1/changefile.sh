#!/bin/bash
#18373187 write for lab0

#delete file 71 - 100
a=71
while [ $a -le 100 ]
do
	rm -r file$a
	a=$[$a+1]
done

#rename file 41 - 70
a=41
while [ $a -le 70 ]
do
	mv ./file$a ./newfile$a
	a=$[$a+1]
done


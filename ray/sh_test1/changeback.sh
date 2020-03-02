#!/bim/bash
#18373187 write for lab0

#creat file 71-100
a=71
while [ $a -le 100 ]
do
	mkdir file$a
	a=$[$a+1]
done

#rename

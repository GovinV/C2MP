#!/bin/bash

rm -r testLog
rm -r testPreCompiled
rm -r testCompiled
rm -r tmpResultTest

mkdir testLog
mkdir testPreCompiled
mkdir testCompiled

FileName=0
for file in ./tests/test*.c
do
	FileName=`echo $file | tr "/" "\n" | grep .c`
	echo "./C2MP $file"

	./C2MP $file > testLog/$FileName.output
	if [ $? -eq 0 ]
	then 
		echo "PreCompiled $FileName"
		cp C2MP_result.c testPreCompiled/$FileName
	else
		echo "Fail to precompile $file" 
	fi

	echo "./C2MP $file -O"
	./C2MP $file -O > testLog/opti_$FileName.output
	if [ $? -eq 0 ]
	then 
		cp C2MP_result.c testPreCompiled/opti_$FileName
	else
		echo "Fail to precompile with optimization $FileName" 
	fi
done

output=0

#File check result
for file in ./testPreCompiled/test*.c
do
	output=0
	FileName=`echo $file | tr "/" "\n" | grep .c`
	Name=`echo "$FileName" | cut -d'.' -f1`
	echo "gcc $file -lmpc"

	gcc -o ./testCompiled/$Name $file -lmpc > testLog/$FileName.output 2>&1
	if [ $? -eq 0 ]
	then 
		echo "Compiled $FileName"
	else
		echo "Fail to compile $FileName with MPC"
	fi
done


mkdir tmpResultTest
for file in ./testCompiled/test*
do
	output=0
	FileName=`echo $file | cut -d'/' -f3` 
	echo "$file"

	timeout 1s ./testCompiled/$FileName | grep -v "^#" > tmpResultTest/$FileName
	if [ $? -eq 0 ]
	then 
		echo "Success : $FileName executed, output:"
		# cat tmpResultTest/$FileName
	else
		echo "Fail to execute $FileName with MPC"
	fi
done



for file in ./tmpResultTest/test*
do
	NotSame=0
	NotSame=$(awk '{
					    f1=$2; f2=$3
					    getline 
					    if (!(f1 < f2+0.0001 && f1 > f2-0.0001 )) {
					    	print 1
					    }
			  		}' $file)

	if [ "$NotSame" = "" ]
	then
		echo "Success : $file all expected value"
	else
		echo "Failed : $file do not return expected value."
	fi
done

# rm -r tmpResultTest
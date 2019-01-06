#!/bin/bash

rm -r testLog
rm -r testPreCompiled
rm -r testCompiled

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
		mv C2MP_result.c testPreCompiled/$FileName
	else
		echo "Fail to precompile $file" 
	fi

	echo "./C2MP $file -O"
	./C2MP $file -O > testLog/opti_$FileName.output
	if [ $? -eq 0 ]
	then 
		mv result.c testPreCompiled/opti_$FileName
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
		continue 
	fi
done


mkdir tmpResultTest
for file in ./testCompiled/test*
do
	output=0
	FileName=`echo $file | cut -d'/' -f3` 
	echo "$file"

	timeout 1s ./testCompiled/$FileName > tmpResultTest/$FileName
	if [ $? -eq 0 ]
	then 
		echo "Succes : $FileName executed, output:"
		# cat tmpResultTest/$FileName
	else
		echo "Fail to execute $FileName with MPC"
	fi
done


# for file in /testCompiled/test*
# do
# 	output=0
# 	FileName=`echo $file | cut -d'/' -f3` 
# 	echo "$file"

# 	timeout 1s ./testCompiled/$FileName > tmpResultTest/$FileName
# 	if [ $? -eq 0 ]
# 	then 
# 		echo "Succes : $FileName executed, output:"
# 		# cat tmpResultTest/$FileName
# 	else
# 		echo "Fail to execute $FileName with MPC"
# 	fi
# done

rm -r tmpResultTest
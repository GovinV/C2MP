#!/bin/bash

if [ ! -d "testLog" ]; then
	mkdir testLog
fi

if [ ! -d "testPreCompiled" ]; then
	mkdir testPreCompiled
fi

if [ ! -d "testCompiled" ]; then
	mkdir testCompiled
fi

if [ ! -d "tmpResultTest" ]; then
	mkdir tmpResultTest
fi


FileName=0
Expected=0
Success=0
Processed=0


for file in ./tests/test*.c
do
	FileName=`echo $file | tr "/" "\n" | grep .c`
	Name=`echo "$FileName" | cut -d'.' -f1`

	AddToBehaviorFile=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`

	if [ "$AddToBehaviorFile" = "" ]
	then
		echo "$Name 9" >> ./tests/expected_behaviour
	fi

	Processed=$((Processed+1))
	Expected=0

	echo "Test on $file"

	./C2MP $file > testLog/$FileName.output
	if [ $? -eq 0 ]
	then 
		# echo "PreCompiled $FileName"
		cp C2MP_result.c testPreCompiled/$FileName
	else
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 1 ]
		then
			Success=$((Success+1))
		else
			echo "	Fail to precompile (step 1) $file : Not Expected, expected to stop step $Expected"
		fi
		continue
	fi

	gcc -o ./testCompiled/$Name testPreCompiled/$FileName -lmpc > testLog/$FileName.output 2>&1
	if [ $? -ne 0 ]
	then
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 2 ]
		then
			Success=$((Success+1))
		else
			echo "	Fail to compile (step 2) $file : Not Expected, expected to stop step $Expected"
		fi
		continue
	fi

	timeout 3s ./testCompiled/$Name | grep -v "^#" > tmpResultTest/$Name
	if [ $? -ne 0 ]
	then
		if [ $? -eq 124 ]
		then
			echo "	Warning : $file : fail due to a timeout, check for infinite loop, or ask for longer timeout"
		fi
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 3 ]
		then
			Success=$((Success+1))
		else
			echo "	Fail to execute (step 3) $file : Not Expected, expected to stop step $Expected"
		fi	
		continue
	fi

	NotSame=0
	NotSame=$(awk '{
					    f1=$2; f2=$3
					    getline 
					    if (!(f1 < f2+0.0001 && f1 > f2-0.0001 )) {
					    	print "1"
					    }
			  		}' tmpResultTest/$Name)

	if [ "$NotSame" = "1" ]
	then
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 4 ]
		then
			Success=$((Success+1))
		else
			echo "	Not expected result (step 4) $file : Not Expected, expected to stop step $Expected"
		fi	
		continue
	fi


	./C2MP $file -O > testLog/opti_$FileName.output
	if [ $? -eq 0 ]
	then 
		# echo "PreCompiled $FileName"
		cp C2MP_result.c testPreCompiled/opti_$FileName
	else
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 5 ]
		then
			Success=$((Success+1))
		else
			echo "	Fail to precompile with optimization (step 5) $file : Not Expected, expected to stop step $Expected"
		fi
		continue
	fi

	gcc -o ./testCompiled/opti_$Name testPreCompiled/opti_$FileName -lmpc > testLog/opti_$FileName.output 2>&1
	if [ $? -ne 0 ]
	then
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 6 ]
		then
			Success=$((Success+1))
		else
			echo "	Fail to compile with optimization (step 6) $file : Not Expected, expected to stop step $Expected"
		fi
		continue
	fi

	timeout 3s ./testCompiled/opti_$Name | grep -v "^#" > tmpResultTest/opti_$Name
	if [ $? -ne 0 ]
	then
		if [ $? -eq 124 ]
		then
			echo "	Warning : $file : fail due to a timeout, check for infinite loop, or ask for longer timeout"
		fi
		Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
		if [ $Expected -eq 7 ]
		then
			Success=$((Success+1))
		else
			echo "	Fail to execute with optimization (step 7) $file : Not Expected, expected to stop step $Expected"
		fi	
		continue
	fi

	Expected=`cat ./tests/expected_behaviour | grep $Name | cut -d' ' -f2`
	diff tmpResultTest/opti_$Name tmpResultTest/$Name > /dev/null 2>&1
	if [ $? -eq 0 ]
	then
		if [ $Expected -eq 9 ]
		then
			Success=$((Success+1))
		else
			echo "	$file expected to fail at step $Expected, but test did not fail"
		fi
	else
		if [ $Expected -eq 8 ]
		then
			Success=$((Success+1))
		else
			echo "	Optimized test and test without it are not the same (step 8) $file : Not Expected, expected to stop step $Expected"
		fi
	fi
done


echo "############"
echo "$Success/$Processed Tests Passed"
echo "############"
#!/bin/sh

$2 &
TASK_PID=$!

# ret=0, command run end;
# ret=1, command run timeout;

ret=1;

for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	if [ $i == $1 ]; then 
		break;
	fi
	sleep 1
	if [ ! -d /proc/${TASK_PID} ]; then
		ret=0
		break;
	fi
done

if [ ${ret} = 1 ]; then
	kill ${TASK_PID}
fi

exit ${ret};

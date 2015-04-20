#/bin/sh
file_list=`find . -name .svn`
for i in $file_list; do
	echo $i
	rm -rf $i
done

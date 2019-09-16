directory="relations"

for file in $directory/*
do
  if [ -f $file ]; then
	cat $file | uniq > lala
	mv lala $file
  fi
done

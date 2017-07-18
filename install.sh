#! /bin/bash

f=$1

if ! [ -d /usr/include/my_dev ];
then 
	sudo mkdir /usr/include/my_dev
fi

if [ "$f" = "debug" ] 
then 
	echo "Установлена дебажная версия <my_dev/treap.hpp>"
	sudo cp ./debug/treap.hpp /usr/include/my_dev/
else
	echo "Установлена релизная версия <my_dev/treap.hpp>"
	sudo cp ./include/treap.hpp /usr/include/my_dev/
fi


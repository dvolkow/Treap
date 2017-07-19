#! /bin/bash

f=$1

if ! [ -d /usr/include/my_dev ];
then 
	sudo mkdir /usr/include/my_dev
fi

if [ "$f" = "debug" ] 
then 
    if [ -f debug/treap.hpp ]
    then
	    echo "Устанавливаем дебажную версию <my_dev/treap.hpp>"
	    sudo cp ./debug/treap.hpp /usr/include/my_dev/
        echo "Успех!"
    else 
        echo "Запустите ./install.sh из корневой директории репозитория!"
    fi
else
    if [ -f include/treap.hpp ]
    then
	    echo "Устанавливаем релизную версию <my_dev/treap.hpp>"
	    sudo cp ./include/treap.hpp /usr/include/my_dev/
        echo "Успех!";
    else 
        echo "Запустите ./install.sh из корневой директории репозитория!"
    fi
fi


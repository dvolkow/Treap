#! /bin/bash

f=$1
name1='treap.hpp'
name2='tree.hpp'

if ! [ -d /usr/include/my_dev ];
then 
	sudo mkdir /usr/include/my_dev
fi

if [ "$f" = "debug" ] 
then 
    if [ -f debug/treap.hpp ]
    then
	    echo "Устанавливаем дебажную версию <my_dev/$name1>..."
	    sudo cp ./debug/$name1 /usr/include/my_dev/
	    echo "Устанавливаем дебажную версию <my_dev/$name2>..."
	    sudo cp ./debug/$name2 /usr/include/my_dev/
        echo "Успех!"
    else 
        echo "Запустите ./install.sh из корневой директории репозитория!"
    fi
else
    if [ -f include/treap.hpp ]
    then
	    echo "Устанавливаем релизную версию <my_dev/$name1>"
	    sudo cp ./include/$name1 /usr/include/my_dev/
	    echo "Устанавливаем релизную версию <my_dev/$name2>"
	    sudo cp ./include/$name2 /usr/include/my_dev/
        echo "Успех!";
    else 
        echo "Запустите ./install.sh из корневой директории репозитория!"
    fi
fi


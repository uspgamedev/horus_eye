#!/bin/bash
cd src
FILES=`find externals/ game/ module/ | grep "^.*\.\(h\|cc\)$" | grep -v "game/config.h" | sort`
echo "SET(GAME_SRCS " > ./src_list.cmake
for f in $FILES; do
	echo "    $f " >> ./src_list.cmake
done
echo ")" >> ./src_list.cmake
echo "Arquivo src_list.txt criado com sucesso."

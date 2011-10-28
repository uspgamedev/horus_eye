#!/bin/bash
FILES=`find ./src/ | grep "^.*\.\(h\|cc\)$"`
echo "SET(GAME_SRCS " > ./src/src_list.cmake
for f in $FILES; do
	echo "    $f " >> ./src/src_list.cmake
done
echo ")" >> ./src/src_list.cmake
echo "Arquivo src/src_list.txt criado com sucesso."

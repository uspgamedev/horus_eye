#!/bin/bash
FILES=`find ./src/ | grep "^.*\.\(h\|cc\)$"`
echo -n "SET(GAME_SRCS " > ./sourcelist.txt
for f in $FILES; do
	echo -n "$f " >> ./sourcelist.txt
done
echo ")" >> ./sourcelist.txt
echo "Arquivo ./sourcelist.txt criado com sucesso."

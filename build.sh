#!/usr/bin/env bash

clang++ -ggdb \
    -I./raylib \
    -o "${1}.exe" "${1}.cc" \
    -L./raylib \
    -lraylib -lm


if [[ $? -eq 0 ]]
then

  "./${1}.exe"
else
    echo "build failure"
fi

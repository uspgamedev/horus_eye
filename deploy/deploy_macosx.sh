#!/bin/sh

# TODO: change the game version in Info.plist when running this script
# TODO: package it in a DMG (Disk Image)
# TODO: Document this script's usage...
# TODO: improve compilation steps in this script (check if code is commited, clean it - including the removal of CMakeCache.txt, compile it)
# TODO: Allow personalization of -jN parameter to make (currently it's hard coded to -j2)

if [ `uname -s` != "Darwin" ]; then
    echo "Warning: this script will deploy a Mac OS X app, but it appears that
         you're not using a Mac." 1>&2
fi

# Change to the script folder
cd `dirname ${BASH_SOURCE[0]}`

# Testing arguments
if [ "$#" -eq 0 ]; then
    echo "Warning: Deploying for testing purposes only.
         Won't create a DMG and won't create an Universal Binary."
    ## Moving to parent dir and compiling
    cd ..
    cmake .
    make -j2
    # Moving back to script dir
    cd deploy
else
    echo "Deploying version $1."
    ## Moving to parent dir and compiling
    cd ..
    # Cleaning
    rm ./CMakeCache.txt
    make clean
    # Compiling Universal Binary
    cmake -DUNIVERSAL=YES .
    make -j2
    # Cleaning up so it doesn't affect a "testing" compilation.
    rm ./CMakeCache.txt
    # Moving back to script dir
    cd deploy
fi

# Check if compiled game executable exists
if [ ! -e ../horus_eye ]; then
    echo "Error: file horus_eye not found! Check if there are compilation errors above." 1>&2
    exit 1
fi

# Remove any previous version
rm -r "./Horus Eye.app"

# Create "Horus Eye.app" and its subfolders
mkdir -p "./Horus Eye.app/Contents/Frameworks"
mkdir -p "./Horus Eye.app/Contents/MacOS"
mkdir -p "./Horus Eye.app/Contents/Resources"

# Copy all the files into the app
cp ../horus_eye "./Horus Eye.app/Contents/MacOS/Horus Eye"
cp ./Info.plist "./Horus Eye.app/Contents/"
cp "./Horus Eye.icns" "./Horus Eye.app/Contents/Resources/"
cp -R ../data/* "./Horus Eye.app/Contents/Resources/"
# Copy frameworks
cp -R /Library/Frameworks/SDL.framework "./Horus Eye.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_image.framework "./Horus Eye.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_mixer.framework "./Horus Eye.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_ttf.framework "./Horus Eye.app/Contents/Frameworks/"

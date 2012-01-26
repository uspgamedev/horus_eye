#!/bin/sh

# TODO: change the game version in Info.plist when running this script
# TODO: copy the data folder into Resources (and change all the hard coded paths in the game source)
# TODO: package it in a DMG (Disk Image)
# TODO: Document this script's usage...
# TODO: include compilation steps into this script ??? (check if code is commited, clean it, compile it)

if [ `uname -s` != "Darwin" ]; then
    echo "Warning: this script will deploy a Mac OS X app, but it appears that
         you're not using a Mac." 1>&2
fi

# Change to the script folder
cd `dirname ${BASH_SOURCE[0]}`

# Check if compiled game executable exists
if [ ! -e ../horus_eye ]; then
    echo "Error: file horus_eye not found! Are you sure you compiled it?" 1>&2
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
# Copy frameworks
cp -R /Library/Frameworks/SDL.framework "./Horus Eye.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_image.framework "./Horus Eye.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_mixer.framework "./Horus Eye.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_ttf.framework "./Horus Eye.app/Contents/Frameworks/"

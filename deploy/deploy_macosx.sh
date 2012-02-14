#!/bin/sh

# TODO: change the game version in Info.plist when running this script
# TODO: package it in a DMG (Disk Image)
# TODO: Document this script's usage...
# TODO: improve compilation steps in this script (check if code is commited before doing anything in a Release)


help() {
    echo "Deployment script for Horus Eye on Mac OS X
Usage:
        $0 [-v Version] [-j NumberOfParallelJobs]
Example: (on a Mac with 4 processor cores)
        $0 -v 1.2.1alpha -j4
If you do not pass the version argument, it will deploy only for local testing."
}

if [ `uname -s` != "Darwin" ]; then
    echo "Warning: this script will deploy a Mac OS X app, but it appears that
         you're not using a Mac." 1>&2
fi

# Change to the script folder
cd `dirname ${BASH_SOURCE[0]}`

# Initializing defaults
JOBS=2
DEPLOY="TESTING"

# Processing parameters
while getopts ":v:j:h" optname
do
    case "$optname" in
        "h")
            help
            exit
            ;;
        "v")
            VERSION=$OPTARG
            DEPLOY="RELEASE"
            ;;
        "j")
            JOBS=$OPTARG
            ;;
        "?")
            echo "Unknown option $OPTARG"
            ;;
        ":")
            echo "No argument value for option $OPTARG"
            exit
            ;;
        *)
            # Should not occur
            echo "Unknown error while processing options" 1>&2
            ;;
    esac
done
echo "-- Using make -j$JOBS"

# Compiling
case "$DEPLOY" in
    "TESTING")
        echo "Warning: Deploying for testing purposes only.
         Won't create a DMG and won't create an Universal Binary."
        ## Moving to parent dir and compiling
        cd ..
        cmake .
        make -j$JOBS
        # Moving back to script dir
        cd deploy
        ;;
    "RELEASE")
        # Check if git is installed
        if [ `which git` ]; then
            echo "-- git is installed on this computer"
            # If git is installed, then require that the code be committed
            if [ "`git status -s 2>&1 | egrep '^\?\?|^ M|^A |^ D|^fatal:'`" ] ; then
                echo "Code is not committed into git. Commit into git before deployment."
                exit
            fi
            echo "-- Repository up-to-date."
        fi
        echo "-- Deploying version $VERSION."
        ## Moving to parent dir and compiling
        cd ..
        # Cleaning
        rm ./CMakeCache.txt
        rm ./horus_eye
        make clean
        # Compiling Universal Binary
        cmake -DUNIVERSAL=YES .
        make -j2
        # Cleaning up so it doesn't affect a "testing" compilation.
        rm ./CMakeCache.txt
        # Moving back to script dir
        cd deploy
        ;;
    *)
        echo "Error: Unknown type of deployment: $DEPLOY" 1>&2
        ;;
esac

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

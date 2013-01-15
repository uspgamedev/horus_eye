#!/bin/sh

# Initializing defaults
JOBS=2
DEPLOY="TESTING"
PROJECT_NAME="Horus Eye"
ICON="./${PROJECT_NAME}.icns"

help() {
    echo "Deployment script for Horus Eye on Mac OS X
Usage:
        $0 [-v Version] [-j NumberOfParallelJobs]
Example: (on a Mac with 4 processor cores)
        $0 -v 1.2.1alpha -j4
If you do not pass the version argument, it will deploy only for local testing."
}

create_DMG() {
    # Create a temporary directory to work in
    TEMP_DIR="`mktemp -d "${TMPDIR}${PROJECT_NAME}.XXXXX"`"

    # Create the folder from which we'll make the disk image
    DISK_IMAGE_SOURCE_PATH="${TEMP_DIR}/${PROJECT_NAME}"
    mkdir "${DISK_IMAGE_SOURCE_PATH}"

    # Copy the application into the folder
    cp -R "./${PROJECT_NAME}.app" \
        "${DISK_IMAGE_SOURCE_PATH}/${PROJECT_NAME}.app"

    # Make a symlink to the Applications folder
    # (so we can prompt the user to install the application)
    ln -s "/Applications" "${DISK_IMAGE_SOURCE_PATH}/Applications"

    # If a "background.png" file is present in the project directory,
    # we'll use that for the background of the folder.
    # An assumption is made in this script that the background image is 400x300px
    # If you are using a different sized image, you'll need to adjust the
    # placement and sizing parameters in the Applescript below
    if [ -e "background.png" ]; then
        cp "background.png" \
            "${DISK_IMAGE_SOURCE_PATH}/background.png"
    fi

    # Create the read-write version of the disk image from the folder
    # Also note the path at which the disk is mounted so we can open the disk
    # to adjust its attributes
    DISK_IMAGE_READWRITE_PATH="${DISK_IMAGE_SOURCE_PATH}-rw.dmg"
    VOLUME_MOUNT_PATH="`hdiutil create -srcfolder "${DISK_IMAGE_SOURCE_PATH}" \
        -format UDRW -attach "${DISK_IMAGE_READWRITE_PATH}" | \
        sed -n 's/.*\(\/Volumes\/.*\)/\1/p'`"

    # Now we use Applescript to tell the Finder to open the disk image,
    # set the view options to a bare, icon arranged view
    # set the background image (if present)
    # and set the icon placements
    if [ -e "background.png" ]; then
        echo '
        tell application "Finder"
        open ("'"${VOLUME_MOUNT_PATH}"'" as POSIX file)
        set statusbar visible of front window to false
        set toolbar visible of front window to false
        set view_options to the icon view options of front window
        set icon size of view_options to 96
        set arrangement of view_options to not arranged
        set the bounds of front window to {100, 100, 500, 400}
        set app_icon to item "'"${XCODE_PROJECT_NAME}"'" of front window
        set app_folder to item "Applications" of front window
        set background_image to item "background.png" of front window
        set background picture of view_options to item "background.png" of front window
        set position of background_image to {200, 200}
        set position of app_icon to {120, 100}
        set position of app_folder to {280, 100}
        set current view of front window to icon view
        end tell' | osascript
        # Make the background.png file invisible
        SetFile -a V "${VOLUME_MOUNT_PATH}/background.png"
    else
        echo '
        tell application "Finder"
        open ("'"${VOLUME_MOUNT_PATH}"'" as POSIX file)
        set statusbar visible of front window to false
        set toolbar visible of front window to false
        set view_options to the icon view options of front window
        set icon size of view_options to 96
        set arrangement of view_options to not arranged
        set the bounds of front window to {100, 100, 500, 400}
        set app_icon to item "'"${PROJECT_NAME}"'" of front window
        set app_folder to item "Applications" of front window
        set position of app_icon to {120, 100}
        set position of app_folder to {280, 100}
        set current view of front window to icon view
        end tell' | osascript
    fi

    # Eject the disk image so that we can convert it to a compressed format
    hdiutil eject "${VOLUME_MOUNT_PATH}"

    # Create the final, compressed disk image
    hdiutil convert "${DISK_IMAGE_READWRITE_PATH}" -format UDBZ \
        -o "./${PROJECT_NAME}.dmg"

    # Remove the temp directory
    rm -Rf "${TEMP_DIR}"
}    

if [ `uname -s` != "Darwin" ]; then
    echo "Warning: this script will deploy a Mac OS X app, but it appears that
         you're not using a Mac." 1>&2
fi

# Change to the script folder
cd `dirname ${BASH_SOURCE[0]}`

# Processing parameters
while getopts ":v:j:h" optname
do
    case "$optname" in
        "h")
            help
            exit
            ;;
        "v")
            NEW_VERSION=$OPTARG
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
echo "-- Using make -j${JOBS}"

# Compiling
case "$DEPLOY" in
    "TESTING")
        echo "Warning: Deploying for testing purposes only.
         Won't create a DMG and won't create an Universal Binary."
        ## Moving to parent dir and compiling
        cd ..
        cmake .
        make -j${JOBS}
        # Moving back to script dir
        cd deploy
        ;;
    "RELEASE")
        # Check if git is installed
        if [ `which git` ]; then
            echo "-- git is installed on this computer"
            # If git is installed, then require that the code be committed
            if [ "`git status -s 2>&1 | egrep '^\?\?|^ M|^A |^ D|^fatal:'`" ] ;
            then
                echo "Error: Code is not committed into git. Commit into git" \
                     "before deployment."
                exit
            fi
            echo "-- Repository up-to-date."
        fi
        echo "-- Deploying version ${NEW_VERSION}."
        ## Moving to parent dir and compiling
        cd ..
        # Cleaning
        rm ./CMakeCache.txt
        rm ./horus_eye
        rm "./deploy/${PROJECT_NAME}.dmg"
        make clean
        # Compiling Universal Binary
        cmake -DUNIVERSAL=YES .
        make -j${JOBS}
        # Cleaning up so it doesn't affect a "testing" compilation.
        rm ./CMakeCache.txt
        # Moving back to script dir
        cd deploy
        # Changing version
        CURRENT_VERSION="`/usr/libexec/PlistBuddy -c 'Print CFBundleVersion' \
        "./Info.plist"`"
        # Report the current version and change it
        echo "Current version is ${CURRENT_VERSION}"
        echo "Changing version to ${NEW_VERSION}"
        /usr/libexec/PlistBuddy -c "Set CFBundleVersion ${NEW_VERSION}" \
            "./Info.plist"
        # Commit the updated Info.plist
        if [ `which git` ]; then
            git commit -m "Updated Info.plist to version ${NEW_VERSION}" \
                "./Info.plist"
        fi
        ;;
    *)
        echo "Error: Unknown type of deployment: $DEPLOY" 1>&2
        ;;
esac

# Check if compiled game executable exists
if [ ! -e ../horus_eye ]; then
    echo "Error: file horus_eye not found! Check if there are compilation" \
         "errors above." 1>&2
    exit 1
fi

# Remove any previous version
rm -r "./${PROJECT_NAME}.app"

# Create "Horus Eye.app" and its subfolders
mkdir -p "./${PROJECT_NAME}.app/Contents/Frameworks"
mkdir -p "./${PROJECT_NAME}.app/Contents/MacOS"
mkdir -p "./${PROJECT_NAME}.app/Contents/Resources"

# Copy all the files into the app
cp ../horus_eye "./${PROJECT_NAME}.app/Contents/MacOS/Horus Eye"
cp ./Info.plist "./${PROJECT_NAME}.app/Contents/"
cp "${ICON}" "./${PROJECT_NAME}.app/Contents/Resources/"
cp -R ../data/* "./${PROJECT_NAME}.app/Contents/Resources/"
# Copy frameworks
cp -R /Library/Frameworks/SDL.framework "./${PROJECT_NAME}.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_image.framework "./${PROJECT_NAME}.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_mixer.framework "./${PROJECT_NAME}.app/Contents/Frameworks/"
cp -R /Library/Frameworks/SDL_ttf.framework "./${PROJECT_NAME}.app/Contents/Frameworks/"

if [ "${DEPLOY}" == "RELEASE" ] ; then
    # Create DMG file for distribution
    create_DMG
fi

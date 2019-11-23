LINUX_DEPLOY=$1
BUILD_DIR=$2

if [ ! -d $BUILD_DIR ]; then
	echo "Couldn't locate build directory: $BUILD_DIR"
	exit
fi

EXECUTABLE=$BUILD_DIR/Soundboard

if [ ! -f $EXECUTABLE ]; then
	echo "Couldn't locate executable: $EXECUTABLE"
	exit
fi

# Output useful debug info
echo "linuxdeploy Executable: $LINUX_DEPLOY"
echo "Build Dir: $BUILD_DIR"
if [ $# -ge 3 ]; then
	if [ ! -f $QMAKE ]; then
		echo "Couldn't locate qmake: $QMAKE"
		exit
	fi
	export QMAKE=$3
	echo "QMAKE export: $QMAKE"
fi
#wget --no-clobber https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
$LINUX_DEPLOY --appdir AppDir -e $EXECUTABLE -i ./soundboard.png -d ./soundboard.desktop --plugin qt --output appimage

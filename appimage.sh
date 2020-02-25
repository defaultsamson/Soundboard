if [ "$1" == "clean" ]; then
	rm -rf ./AppDir
	rm ./*.AppImage
	echo Cleaned!
	exit
fi

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

# Makes the AppImage bundle Qt's libqgtk3.so library instead of libqgtk2.so 
# https://github.com/linuxdeploy/linuxdeploy-plugin-qt/blob/8f37917e428dd1c0734457074d211a432a09410f/src/deployers/PlatformPluginsDeployer.cpp#L41
export DEPLOY_PLATFORM_THEMES=1

$LINUX_DEPLOY --appdir AppDir -e $EXECUTABLE -i ./soundboard.png -d ./soundboard.desktop --plugin qt --output appimage

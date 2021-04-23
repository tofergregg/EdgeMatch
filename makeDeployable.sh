#!/bin/bash

~/Qt/5.15.2/clang_64/bin/macdeployqt TileMatchDemo.app
exit 0

otool -L TileMatchComplete.app/Contents/MacOS/TileMatchComplete 

mkdir TileMatchComplete.app/Contents/Frameworks

#files=QtWidgets QtMultimedia QtGui QtNetwork QtCore
files="QtWidgets QtMultimedia QtNetwork QtGui QtCore"

for f in $files; do
    # cp -R /Users/tofer/Qt/Tools/CMake/CMake.app/Contents/Frameworks/$f.framework TileMatchComplete.app/Contents/Frameworks
    cp -R /Users/tofer/Qt/5.15.2/clang_64/lib/$f.framework TileMatchComplete.app/Contents/Frameworks
done

echo "part 1 done"
echo "<enter> to continue"
read

for f in $files; do
    install_name_tool -id @executable_path/../Frameworks/$f.framework/Versions/5/$f TileMatchComplete.app/Contents/Frameworks/$f.framework/Versions/5/$f
done

echo "part 2 done"
echo "<enter> to continue"
read

for f in $files; do
    install_name_tool -change @rpath/$f.framework/Versions/5/$f @executable_path/../Frameworks/$f.framework/Versions/5/$f TileMatchComplete.app/Contents/MacOS/TileMatchComplete
done

echo "part 3 done"
echo "<enter> to continue"
read

# dependent_files="QtWidgets QtMultimedia QtNetwork QtGui"
dependent_files="QtGui"
for f in $dependent_files; do
    install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore TileMatchComplete.app/Contents/Frameworks/$f.framework/Versions/5/$f
done

echo "part 4 done"
echo "<enter> to continue"
read

echo
echo "running otool again"
otool -L TileMatchComplete.app/Contents/MacOS/TileMatchComplete 


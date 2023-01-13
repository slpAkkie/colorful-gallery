# Viewer

Compact image viewer with feature to sort them by color properties.

## Getting started

Use QtCreator to open this project

## Build

In project root directory

### Debug

```sh
mkdir -p build/Debug
cd build/Debug

make clean
qmake6 ../../colorful-gallery.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
make qmake_all
make
```

### Release

```sh
mkdir -p build/Release
cd build/Release

qmake6 ../../colorful-gallery.pro -spec linux-g++ CONFIG+=qtquickcompiler
make qmake_all
make
```

## Version

1.x-cpp-dev

## Author

Alexandr Shamanin (@slpAkkie). `Idea - Denyaev Vladislav (@WhiteFox123c)`

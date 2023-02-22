# Viewer

Компактный обозреватель фотографий с возможностью сортировки по цветовым характеристикам

## Начало работы

Используйте `QtCreator`, чтобы открыть проект

## Сборка

Сборка происходит средствами `QtCreator`, но если необходимо собрать проект без его использования смотрите ниже

### Сборка в режиме отладки

```sh
mkdir -p build/Debug
cd build/Debug

make clean
qmake6 ../../colorful-gallery.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
make qmake_all
make
```

### Сборка в режиме релиза

```sh
mkdir -p build/Release
cd build/Release

qmake6 ../../colorful-gallery.pro -spec linux-g++ CONFIG+=qtquickcompiler
make qmake_all
make
```

## Версия

cpp-dev

## Автор

Alexandr Shamanin (@slpAkkie)

## Идея

Denyaev Vladislav ([@WhiteFox123c](https://github.com/WhiteFox123c/))

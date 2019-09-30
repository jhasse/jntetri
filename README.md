# JNTETRI [![pipeline status](https://gitlab.com/jhasse/jntetri/badges/master/pipeline.svg)](https://gitlab.com/jhasse/jntetri/commits/master)

Make sure you clone recursively or check out the submodules using `git submodule update --init`.

## Windows

```
pacman -S mingw-w64-x86_64-SDL2
```

## Linux

On Ubuntu you can install all the dependencies using:

```
sudo apt install libboost-filesystem-dev libboost-serialization-dev meson
```

## Building Using Meson

```sh
meson build
ninja -C build
./build/jntetri
```

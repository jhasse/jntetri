# JNTETRI

Make sure you clone recursively or check out the submodules using `git submodule update --init`.

## Windows

Open `JNTETRI.sln` with Visual Studio and press F5.

### MSYS2:

```
pacman -S mingw-w64-x86_64-SDL2
```

## Linux

On Ubuntu you can install all the dependencies using:

```
sudo apt install libboost-filesystem-dev libboost-serialization-dev scons
```

## Building Using Scons

```sh
cd jngl
scons release=1
cd -
scons release=1
./bin/jntetri
```

Type

```
scons -h
```

to get a list of build options available.

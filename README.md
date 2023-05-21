# ReadMe.MD

## 1. Compilation of the game

This game was packed by me by hand with the help of CMake. Below is the **file structure of the project**:

```
./docs: All docs are stored here
./include: Libraries Headers are stored here. In this case, only econio.
./src: Source code of the game and of the libraries.
    ./src/obj: obj files (artifacts of the compilation process).
```

To build the game, we need **4 libraries**:

- stdio.h
- stdlib.h
- unistd.h
- econio.h

These libraries should be on your computer, except for **econio.h**, which is already include in the **src** folder.

To compile and run the game, you will nead to run these commands from the ./ of this game.

```ZSH
$ cd src
$ make
$ ./main
```

In case, you can't run the game due to some enviroment incompatibility. There is a packed and compiled file for you to run.

```ZSH
$ ./main
```

## 2. How to start the game

After compiling the game, you can **start the game** by:

```ZSH
$ ./main
```

# IsoCross

In this game, you will be challenged to created a path that doesn't cross a guards way! If it does, you're doomed... but if it doesn't, you'll find the  treasures!!!!!

![img](img/Screen%20Shot%202021-06-21%20at%2021.54.35.png)

 
IsoCross supports all major PC operating systems (MacOS and Linux), with other versions coming soon for Xbox Series X and Playstation 5.

## Compiling

A Makefile is supported, which automatically identifies all `.cpp` and `.c` files
inside `/dev/src`. All you need to do in order to compile this masterpiece of 
gaming is run:

```
cd dev
cmake . -G"Unix Makefiles"
make
./app
```

You can also clean the generated binaries by running:

```bash
$ cd dev
$ make clean
```

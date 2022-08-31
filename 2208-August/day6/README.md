# DAY-6: Demoscene MOD player

![C](https://img.shields.io/badge/c-blue?style=flat-square&logo=c&logoColor=white)

---

## Goals

Following the previous day where we explore the MOD format, today we interpret it and produce some music.

Only the .MOD format is supported, mainly because it took a long time to figure out how all the effects are implemented and how to support them.

The output quality is not the best we can have, as some "pops" and "clicks" are still present sometime.

## Usage

``` bash
modplayer - Play a MOD tracker file using Alsa or PulseAudio
Syntax:
    modplayer [options]

options:
    --file/-f   : path to the mod tracker file
    --driver/-d : audio driver (default: pulse)
    --sample/-s : play sample with audio driver
    --track/-t  : display track information
    --info/-i   : display song information
    --help/-h   : this help
```

To play a MOD file:

``` bash
../build/modplayer -f ~/Music/Enigma.mod
```

## Compilation

To compile, just use make:

``` bash
make clean; make
```


# DAY-1: Tone generation with Alsa

![C](https://img.shields.io/badge/c-blue?style=flat-square&logo=c&logoColor=white)

---

## Goals

The goal of Day-1 is to generate a tone at a specific frequency and playing it through the ALSA driver.

That's the very basic, kind of "Hello, World!" of Alsa.

*Frequency* and *duration* are set by the user on the command line.

``` bash
tone - Generate a tone at the frequency
Syntax:
    tone [--help] <frequency> [duration]

frequency: selected frequency
duration : selected duration (default: 1s)
```

To **build** the program, just use *make*.

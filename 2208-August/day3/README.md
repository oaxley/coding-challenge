# DAY-3: Play a frequency tone with PulseAudio

![C](https://img.shields.io/badge/c-blue?style=flat-square&logo=c&logoColor=white)

---

## Goals

The goal of Day-3 is to move away from Alsa and try PulseAudio which is the prefered way to build sound application on Linux. This way audio output can be shared between programs.

This version now uses the *getopt* function to read the arguments on the command line.

``` bash
Tone - Generate a tone with PulseAudio server
Syntax:
    pTone [--help] --freq/-f <frequency> [--duration-d <duration>]

--freq/-f: generate a tone at the selected frequency
--duration/-d: duration for the tone (default: 1s)
```

To **build** the program, just use *make*.

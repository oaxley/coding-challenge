# DAY-2: Play a MIDI note with Alsa

![C](https://img.shields.io/badge/c-blue?style=flat-square&logo=c&logoColor=white)

---

## Goals

The goal of Day-2 is to generate a specific note given its value from the MIDI keyboard. Everything is still played through the ALSA driver.

The *note* and *duration* are set by the user on the command line.

``` bash
note - Plays a note
Syntax:
    note [--help] <note> [duration]

note     : selected note from A0 (21) to C8 (108)
duration : selected duration (default: 1s)
```

To **build** the program, just use *make*.

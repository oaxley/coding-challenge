# DAY-4: Implementing an AudioDriver

![C](https://img.shields.io/badge/c-blue?style=flat-square&logo=c&logoColor=white)

---

## Goals

Now that both Alsa & PulseAudio have been tested, it's time to create an Audio Driver that will allow us to select any of these ones to output sound.

Today we still generating a boring tone, but the user has the ability to select a sound driver too. By default, ALSA will be selected.


``` bash
cppTone - Generate a tone with either Alsa or PulseAudio
Syntax:
    cppTone [--help] --freq/-f <frequency> [--time/-t <duration>] [--driver/-d <driver>]

--freq/-f   : generate a sound at the selected frequency
--time/-t   : duration for playing the sound (default: 1s)
--driver/-d : select alsa (Alsa) / pulse (PulseAudio) driver for output (default: alsa)
```

To **build** the program, just use *make*.

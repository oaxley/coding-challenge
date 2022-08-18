# DAY-5: Demoscene tracker format

![Python](https://img.shields.io/badge/Python-3.9.6-blue?style=flat-square)

---

## Goals

We venture now in the demoscene zone, more specifically in the music production.
In that glorious time, the music was produced by sometime single musician with help of Trackers. Some teams, like Future Crew, even built their own tracker (ScreamTracker).
Today we will build a tracker file explorer that will support for now:
- *.mod : the original file format from the Amiga days
- *.s3m : the format from ScreamTracker
- *.xm  : the format from FastTracker

As we use a plugins system, more formats can be added later on, without changing the main code.


``` bash
usage: modinfo.py [-h] [--file filename] [--list]

options:
  -h, --help       show this help message and exit
  --file filename  show information on MOD file
  --list           list of available plugins
```

The information displayed are:

```
./modinfo.py --file ../tmp/stardust.mod

====== Music MOD Information ======
Format      : MOD
Tracker     :
Song Name   : stardust memories
Song Length : 31 tracks (estimated 00:03:58)
Speed / BPM : 6 / 125
Channels    : 4
Max Pattern : 19
Track List  : [2, 0, 0, 1, 1, 3, 4, 16, 5, 6, 7, 8, 5, 6, 7, 8, 17, 18, 13, 14, 13, 14, 9, 10, 11, 12, 5, 6, 7, 8, 19]
Instruments :
    |                                |   Length   |          |          |  Loop   |  Loop   |
 ID |              Name              |  (bytes)   | Finetune |  Volume  |  Start  | Length  |
----+--------------------------------+------------+----------+----------+---------+---------+
 00 | composed & sampled by          |      10320 |        0 |       64 |       0 |       2 |
 01 | <+> jester/sanity <+>          |       7168 |        0 |       64 |       0 |       2 |
 02 | ^ all samples by me ^          |       2416 |        0 |       64 |       0 |       2 |
 03 | except #10 by ff/pha.          |       1616 |        0 |       64 |       0 |       2 |
 04 | for commercial musics          |       3614 |        0 |       64 |    2770 |     844 |
 05 | write to this address          |       7904 |        0 |       64 |    3324 |    4580 |
 06 | <+> volker  tripp <+>          |       2192 |        0 |       48 |       0 |       2 |
 07 | in der ziegelheide 15          |       9936 |        0 |       64 |    4218 |    5718 |
 08 | 429 bocholt - germany          |       9968 |        0 |       64 |    4272 |    5696 |
 09 |  or dial this number           |       2158 |        0 |       48 |     454 |    1704 |
 10 | * +49-(0)2871-14692 *          |      43522 |        0 |       64 |    6194 |   37328 |
 11 |                                |       1104 |        0 |       64 |       0 |       2 |
 12 |                                |      10640 |        0 |       64 |       0 |       2 |
 13 |                                |       7762 |        0 |       64 |    2536 |    5226 |
 14 |                                |       2304 |        0 |       64 |       0 |       2 |
 15 |                                |       9488 |        0 |       53 |    9362 |     126 |
```


# InfoNES64

A Nintendo Entertainment Emulator running on th Nintendo 64.  Run it on real hardware using a flash drive, or an emulator.

> Note This project is more of a fun thing for me to try if it works. It runs way too slow to be really usable and playable and there is no sound. I don't know if the emulator will ever run at full speed. Help is always welcome. 

It has a unlicensed built-in game ["Blade Buster"](https://www.rgcd.co.uk/2011/05/blade-buster-nes.html), but you can replace it with your own game. See below.

Built with [Libdragon](https://github.com/DragonMinded/libdragon)

The emulator code is ported from [Infones by Jay Kumogata](https://github.com/jay-kumogata/InfoNES)

## Release

To try the emulator with the free built-in game Blade Buster, download infones.64 from the [releases](https://github.com/fhoedemakers/InfoNES64/releases/latest) page en copy it to your flash card.

## Building from source

1. Install the Libdragon SDK. For more info and instructions, see https://github.com/DragonMinded/libdragon
2. Make sure the environment var N64_INST points to the installed SDK
3. Get the sources and build

````bash
git clone https://github.com/fhoedemakers/InfoNES64.git
cd InfoNes64
make
````

Then copy **infones64.z64** to your flash drive.

## Replace the built-in game with your own rom

One of the ways to do this is: 

1. xxd -i myrom.nes > myrom.c
2. Copy myrom.c to the assets folder
3. edit myrom.c

````C
// Change the define
#define BUILTINROMNAME "myrom"

// Change murom_nes[] to builtinrom[]
unsigned char builtinrom[] = {
  0x4e, 0x45, 0x53, 0x1a, 0x10, 0x10, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xa9, 0x08, 0x8d, 0x00, 0x20, 0xa9, 0x00, 0x8d,
  0x01, 0x20, 0x20, 0xd6, 0xe1, 0x20, 0xeb, 0xe2, 0xa9, 0x00, 0x20, 0xe6,
  0xd0, 0xa0, 0x0c, 0x84, 0xe

...

 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};
// change variabele from myrom_nes_len to builtinrom
unsigned int builtinrom_len = 393232;
````

4. Edit builtinrom.c

Insert

#ifdef BUILTINROM_MYROM  
#include "myrom.c"  
#endif  

as shown below

````C
#include "builtinrom.h"
#ifdef BUILTINROM_BLADEBUSTER
#include "bladebuster.c"
#endif
#ifdef BUILTINROM_SMB3
#include "smb3.c"
#endif
#ifdef BUILTINROM_GALAGA
#include "galaga.c"
#endif
#ifdef BUILTINROM_CV1
#include "CV1.c"
#endif
#ifdef BUILTINROM_CV2
#include "CV2.c"
#endif
#ifdef BUILTINROM_CV3
#include "CV3.c"  // not working
#endif
#ifdef BUILTINROM_MYROM
#include "myrom.c"
#endif
````

5. edit builtinrom.h

````C
#pragma once

// Specify the name of the built-in ROM to include
#define BUILTINROM_MYROM
extern unsigned char builtinrom[];
extern unsigned int builtinrom_len;

#ifdef __cplusplus
extern "C"
{
#endif

char *GetBuiltinROMName();
#ifdef __cplusplus
}
#endif
````
   

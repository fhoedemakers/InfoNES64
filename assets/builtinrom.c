// You must use your own roms to replace the built-in rom.
// Convert a rom file to a C array: xxd -i rom.nes > rom.c
// Modify rom.c as follows:
// 1. Change the array name to builtinrom
// 2. Change the array length to builtinrom_len
// 3. Add #define BUILTINROM_NAME and give the game a name.
//
// Modify this file to include the new rom based on the #define
// Modify builtinrom.h and change the #define of the rom you want to use.
// Only one rom can be used at a time.

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
#ifdef BUILTINROM_SMB
#include "smb.c"
#endif
char *GetBuiltinROMName()
{
    return BUILTINROMNAME;
}
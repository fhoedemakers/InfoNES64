
#include "builtinrom.h"
#ifdef BUILTINROM_BLADEBUSTER
#include "bladebuster.c"
#endif
#ifdef BUILTINROM_SMB3
#include "smb3.c"
#endif

char *GetBuiltinROMName()
{
    return BUILTINROMNAME;
}
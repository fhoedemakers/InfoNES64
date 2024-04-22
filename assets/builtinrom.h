#pragma once

// Specify the name of the built-in ROM to include
#define BUILTINROM_SMB
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
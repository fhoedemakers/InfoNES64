#pragma once
// Remove the #pragma once directive to fix the incompatible language modes error.
// Is currently Blade Buster (http://hlc6502.web.fc2.com/Bbuster.htm)
// Generated from BladeBuster.nes using
// xxd -i BladBuster.nes > builtinrom.h

#define BUILTINROM_SMB3
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
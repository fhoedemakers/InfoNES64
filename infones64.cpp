#include <stdio.h>

#include <memory>
#include <math.h>

#include <string.h>
#include <stdarg.h>
#include <algorithm>

#include <InfoNES.h>
#include <InfoNES_System.h>
#include <InfoNES_pAPU.h>
#include "libdragon.h"
#include "rom_selector.h"

#include "builtinrom.h"	


// test

#define CC(x) (((x >> 1) & 15) | (((x >> 6) & 15) << 4) | (((x >> 11) & 15) << 8))
const WORD (NesPalette)[64] = {
    CC(0x39ce), CC(0x1071), CC(0x0015), CC(0x2013), CC(0x440e), CC(0x5402), CC(0x5000), CC(0x3c20),
    CC(0x20a0), CC(0x0100), CC(0x0140), CC(0x00e2), CC(0x0ceb), CC(0x0000), CC(0x0000), CC(0x0000),
    CC(0x5ef7), CC(0x01dd), CC(0x10fd), CC(0x401e), CC(0x5c17), CC(0x700b), CC(0x6ca0), CC(0x6521),
    CC(0x45c0), CC(0x0240), CC(0x02a0), CC(0x0247), CC(0x0211), CC(0x0000), CC(0x0000), CC(0x0000),
    CC(0x7fff), CC(0x1eff), CC(0x2e5f), CC(0x223f), CC(0x79ff), CC(0x7dd6), CC(0x7dcc), CC(0x7e67),
    CC(0x7ae7), CC(0x4342), CC(0x2769), CC(0x2ff3), CC(0x03bb), CC(0x0000), CC(0x0000), CC(0x0000),
    CC(0x7fff), CC(0x579f), CC(0x635f), CC(0x6b3f), CC(0x7f1f), CC(0x7f1b), CC(0x7ef6), CC(0x7f75),
    CC(0x7f94), CC(0x73f4), CC(0x57d7), CC(0x5bf9), CC(0x4ffe), CC(0x0000), CC(0x0000), CC(0x0000)};

namespace {
    ROMSelector romSelector_;
    static  uintptr_t NES_FILE_ADDR = (uintptr_t)builtinrom;
}
uint32_t getCurrentNVRAMAddr()
{
    if (!romSelector_.getCurrentROM())
    {
        return {};
    }
    int slot = romSelector_.getCurrentNVRAMSlot();
    if (slot < 0)
    {
        return {};
    }
    debugf("SRAM slot %d\n", slot);
    return NES_FILE_ADDR - SRAM_SIZE * (slot + 1);
}

void saveNVRAM()
{
    if (!SRAMwritten)
    {
        debugf("SRAM not updated.\n");
        return;
    }

    debugf("save SRAM\n");
    // TODO
    debugf("done\n");

    SRAMwritten = false;
}

void loadNVRAM()
{
    if (auto addr = getCurrentNVRAMAddr())
    {
        debugf("load SRAM %lx\n", addr);
       // TODO
        debugf("done\n");
    }
    SRAMwritten = false;
}

void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem)
{
   
}

void InfoNES_MessageBox(const char *pszMsg, ...)
{
    debugf("[MSG]");
    va_list args;
    va_start(args, pszMsg);
    vdebugf(pszMsg, args);
    va_end(args);
    debugf("\n");
}

bool parseROM(const uint8_t *nesFile)
{
    memcpy(&NesHeader, nesFile, sizeof(NesHeader));
    if (!checkNESMagic(NesHeader.byID))
    {
        return false;
    }

    nesFile += sizeof(NesHeader);

    memset(SRAM, 0, SRAM_SIZE);

    if (NesHeader.byInfo1 & 4)
    {
        memcpy(&SRAM[0x1000], nesFile, 512);
        nesFile += 512;
    }

    auto romSize = NesHeader.byRomSize * 0x4000;
    ROM = (BYTE *)nesFile;
    nesFile += romSize;

    if (NesHeader.byVRomSize > 0)
    {
        auto vromSize = NesHeader.byVRomSize * 0x2000;
        VROM = (BYTE *)nesFile;
        nesFile += vromSize;
    }

    return true;
}

void InfoNES_ReleaseRom()
{
    ROM = nullptr;
    VROM = nullptr;
}

void InfoNES_SoundInit()
{
}

int InfoNES_SoundOpen(int samples_per_sync, int sample_rate)
{
    return 0;
}

void InfoNES_SoundClose()
{
}

int InfoNES_GetSoundBufferSize()
{
    //debugf("GetSoundBufferSize\n");
    return 256; // TODO
}
void (InfoNES_SoundOutput)(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5)
{
    
}

extern WORD PC;

int framecounter=0;
void InfoNES_LoadFrame()
{
   //debugf("InfoNES_LoadFrame %d\n", framecounter++);
   framecounter++;
   
    if (framecounter % 60 == 0)
    {
         debugf("FPS: %d\n", framecounter);
         framecounter = 0;
    }

}



WORD buffer[256];
void (InfoNES_PreDrawLine)(int line)
{
     InfoNES_SetLineBuffer(buffer, 256);
}

void (InfoNES_PostDrawLine)(int line)
{

}

bool loadAndReset()
{
    auto rom = romSelector_.getCurrentROM();
    if (!rom)
    {
        debugf("ROM does not exists.\n");
        return false;
    }

    if (!parseROM(rom))
    {
        debugf("NES file parse error.\n");
        return false;
    }
    loadNVRAM();

    if (InfoNES_Reset() < 0)
    {
        debugf("NES reset error.\n");
        return false;
    }

    return true;
}

int InfoNES_Menu()
{
    // InfoNES_Main() のループで最初に呼ばれる
    loadAndReset();
    return 0;
}



int main()
{

    debug_init(DEBUG_FEATURE_LOG_ISVIEWER);  
    debugf("Starting InfoNES 64, a Nintendo Entertainment System emulator for the Nintendo 64\n");
    debugf("Built on %s %s\n", __DATE__, __TIME__);
  
    debugf("Now running %s\n", GetBuiltinROMName());
    romSelector_.init(NES_FILE_ADDR);
    InfoNES_Main();
    return 0;
}

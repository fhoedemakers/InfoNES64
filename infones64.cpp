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

/* hardware definitions */
// Pad buttons
#define A_BUTTON(a)     ((a) & 0x8000)
#define B_BUTTON(a)     ((a) & 0x4000)
#define Z_BUTTON(a)     ((a) & 0x2000)
#define START_BUTTON(a) ((a) & 0x1000)

// D-Pad
#define DU_BUTTON(a)    ((a) & 0x0800)
#define DD_BUTTON(a)    ((a) & 0x0400)
#define DL_BUTTON(a)    ((a) & 0x0200)
#define DR_BUTTON(a)    ((a) & 0x0100)

// Triggers
#define TL_BUTTON(a)    ((a) & 0x0020)
#define TR_BUTTON(a)    ((a) & 0x0010)

// Yellow C buttons
#define CU_BUTTON(a)    ((a) & 0x0008)
#define CD_BUTTON(a)    ((a) & 0x0004)
#define CL_BUTTON(a)    ((a) & 0x0002)
#define CR_BUTTON(a)    ((a) & 0x0001)

#define PAD_DEADZONE     5
#define PAD_ACCELERATION 10
#define PAD_CHECK_TIME   40

volatile int gTicks; /* incremented every vblank */
surface_t *_dc;

bool fps_enabled = false;

// RGB551 nes color palette
const WORD NesPalette[64] = {
    0x6319, 0x00ED, 0x2033, 0x502D, 0x701D, 0x8009, 0x7041, 0x5141, 
    0x2201, 0x0281, 0x02C1, 0x0289, 0x01DD, 0x0001, 0x0001, 0x0001, 
    0xAD6B, 0x0ABF, 0x49BF, 0x88BF, 0xB875, 0xD09B, 0xC141, 0x9A81, 
    0x63C1, 0x24C1, 0x0501, 0x04D1, 0x03ED, 0x0001, 0x0001, 0x0001, 
    0xFFFF, 0x557F, 0x943F, 0xD33F, 0xFABF, 0xFAF3, 0xFB95, 0xFCC1, 
    0xBE01, 0x7F01, 0x4785, 0x275F, 0x2EBD, 0x4A53, 0x0001, 0x0001, 
    0xFFFF, 0xB73F, 0xCEBF, 0xEE3F, 0xFDFF, 0xFDFD, 0xFE31, 0xFEA7, 
    0xEF21, 0xCFA1, 0xB7E7, 0xAFF1, 0xAFBD, 0xBDEF, 0x0001, 0x0001
};

namespace
{
    ROMSelector romSelector_;
    static uintptr_t NES_FILE_ADDR = (uintptr_t)builtinrom;
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

struct controller_data gKeys;
static DWORD prevButtons[2]{};
static int rapidFireMask[2]{};
static int rapidFireCounter = 0;
void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem)
{
    static constexpr int LEFT = 1 << 6;
    static constexpr int RIGHT = 1 << 7;
    static constexpr int UP = 1 << 4;
    static constexpr int DOWN = 1 << 5;
    static constexpr int SELECT = 1 << 2;
    static constexpr int START = 1 << 3;
    static constexpr int A = 1 << 0;
    static constexpr int B = 1 << 1;
  controller_scan();
     gKeys = get_keys_pressed();

      ++rapidFireCounter;
    bool reset = false;

    for (int i = 0; i < 2; ++i)
    {
        auto &dst = i == 0 ? *pdwPad1 : *pdwPad2;
        auto gp = gKeys.c[i].data >> 16;

        int v = (DL_BUTTON(gp) ? LEFT : 0) |
                (DR_BUTTON(gp) ? RIGHT : 0) |
                (DU_BUTTON(gp) ? UP : 0) |
                (DD_BUTTON(gp) ? DOWN : 0) |
                (A_BUTTON(gp) ? A : 0) |
                (B_BUTTON(gp) ? B : 0) |
                (Z_BUTTON(gp) ? SELECT : 0) |
                (START_BUTTON(gp)  ? START : 0) |
                0;
      
        int rv = v;
        if (rapidFireCounter & 2)
        {
            // 15 fire/sec
            rv &= ~rapidFireMask[i];
        }

        dst = rv;

        auto p1 = v;

        auto pushed = v & ~prevButtons[i];

        // Toggle frame rate
        if (p1 & START)
        {
            if (pushed & A)
            {
                fps_enabled = !fps_enabled;
            }
        }
        if (p1 & SELECT)
        {
            // if (pushed & LEFT)
            // {
            //     saveNVRAM();
            //     romSelector_.prev();
            //     reset = true;
            // }
            // if (pushed & RIGHT)
            // {
            //     saveNVRAM();
            //     romSelector_.next();
            //     reset = true;
            // }
            if (pushed & START)
            {
                saveNVRAM();
                reset = true;
            }
            if (pushed & A)
            {
                rapidFireMask[i] ^= A_BUTTON(gp);
            }
            if (pushed & B)
            {
                rapidFireMask[i] ^= B_BUTTON(gp);
            }
            // if (pushed & UP)
            // {
            //     screenMode(-1);
            // }
            // else if (pushed & DOWN)
            // {
            //     screenMode(+1);
            // }
        }

        prevButtons[i] = v;
    }

    *pdwSystem = reset ? PAD_SYS_QUIT : 0;
}

void InfoNES_MessageBox(const char *pszMsg, ...)
{
    debugf("[MSG]");
    va_list args;
    va_start(args, pszMsg);
    vfprintf(stderr, pszMsg, args);
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
    // debugf("GetSoundBufferSize\n");
    return 256; // TODO
}
void(InfoNES_SoundOutput)(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5)
{
}

extern WORD PC;

int framecounter = 0;
int framedisplay = 0;
void InfoNES_LoadFrame()
{
    
    char buffer[10];
    sprintf(buffer, "FPS: %d", framedisplay);
    graphics_draw_text(_dc, 5, 5, buffer);
    
    display_show(_dc);
    framecounter++;  
  
}

WORD buf[256];
void(InfoNES_PreDrawLine)(int line)
{
    if ( line == 4) {
        // debugf("Getting Display Context %d\n", line);
        _dc = display_get();
    }
    // get the correct position in the framebuffer
    WORD *buffer = ((WORD *)(_dc)->buffer) + (line << 8);
    assert(buffer != nullptr);
    InfoNES_SetLineBuffer(buffer, 256);
}

void(InfoNES_PostDrawLine)(int line)
{
    // debugf("InfoNES_PostDrawLine %d\n", line);
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

/* vblank callback */
void vblCallback(void)
{
    gTicks++;
}

void frameratecalc(int ovfl)
{
   debugf("FPS: %d\n", framecounter);
   framedisplay=framecounter;
   framecounter=0;
}
int main()
{

    debug_init(DEBUG_FEATURE_LOG_ISVIEWER);
    debugf("Starting InfoNES 64, a Nintendo Entertainment System emulator for the Nintendo 64\n");
    debugf("Built on %s %s using libdragon\n", __DATE__, __TIME__);
    debugf("Now running %s\n", GetBuiltinROMName());

    /* Initialize peripherals */
    display_init(RESOLUTION_256x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    register_VI_handler(vblCallback);
    controller_init();
    timer_init();
    new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, frameratecalc);
    romSelector_.init(NES_FILE_ADDR);
    _dc = display_get();
    void *buffer = (_dc)->buffer;
    debugf("Flags: %d\n", _dc->flags);
    debugf("Width: %d\n", _dc->width);
    debugf("Height: %d\n", _dc->height);
    debugf("Stride: %d\n", _dc->stride);
     display_show(_dc);
    InfoNES_Main();
    return 0;
}

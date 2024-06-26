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

// #define NORENDER

// mute sound for now
#define SOUNDISMUTED 1
/* hardware definitions */
// Pad buttons
#define A_BUTTON(a) ((a) & 0x8000)
#define B_BUTTON(a) ((a) & 0x4000)
#define Z_BUTTON(a) ((a) & 0x2000)
#define START_BUTTON(a) ((a) & 0x1000)

// D-Pad
#define DU_BUTTON(a) ((a) & 0x0800)
#define DD_BUTTON(a) ((a) & 0x0400)
#define DL_BUTTON(a) ((a) & 0x0200)
#define DR_BUTTON(a) ((a) & 0x0100)

// Triggers
#define TL_BUTTON(a) ((a) & 0x0020)
#define TR_BUTTON(a) ((a) & 0x0010)

// Yellow C buttons
#define CU_BUTTON(a) ((a) & 0x0008)
#define CD_BUTTON(a) ((a) & 0x0004)
#define CL_BUTTON(a) ((a) & 0x0002)
#define CR_BUTTON(a) ((a) & 0x0001)

#define PAD_DEADZONE 5
#define PAD_ACCELERATION 10
#define PAD_CHECK_TIME 40

volatile int gTicks; /* incremented every vblank */
surface_t *_dc;
bool controller1IsInserted = false;
bool controller2IsInserted = false;

bool fps_enabled = false;
extern int APU_Mute;
// RGB551 nes color palette
const WORD NesPalette[64] = {
    0x6319, 0x00ED, 0x2033, 0x502D, 0x701D, 0x8009, 0x7041, 0x5141,
    0x2201, 0x0281, 0x02C1, 0x0289, 0x01DD, 0x0001, 0x0001, 0x0001,
    0xAD6B, 0x0ABF, 0x49BF, 0x88BF, 0xB875, 0xD09B, 0xC141, 0x9A81,
    0x63C1, 0x24C1, 0x0501, 0x04D1, 0x03ED, 0x0001, 0x0001, 0x0001,
    0xFFFF, 0x557F, 0x943F, 0xD33F, 0xFABF, 0xFAF3, 0xFB95, 0xFCC1,
    0xBE01, 0x7F01, 0x4785, 0x275F, 0x2EBD, 0x4A53, 0x0001, 0x0001,
    0xFFFF, 0xB73F, 0xCEBF, 0xEE3F, 0xFDFF, 0xFDFD, 0xFE31, 0xFEA7,
    0xEF21, 0xCFA1, 0xB7E7, 0xAFF1, 0xAFBD, 0xBDEF, 0x0001, 0x0001};

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

    ++rapidFireCounter;
    bool reset = false;

    for (int i = 0; i < 2; ++i)
    {
        if ((i == 0 && controller1IsInserted == false) ||
            (i == 1 && controller2IsInserted == false))
        {
            continue;
        }
        auto &dst = i == 0 ? *pdwPad1 : *pdwPad2;
        auto gp = gKeys.c[i].data >> 16;

        int v = (DL_BUTTON(gp) ? LEFT : 0) |
                (DR_BUTTON(gp) ? RIGHT : 0) |
                (DU_BUTTON(gp) ? UP : 0) |
                (DD_BUTTON(gp) ? DOWN : 0) |
                (A_BUTTON(gp) ? A : 0) |
                (B_BUTTON(gp) ? B : 0) |
                (Z_BUTTON(gp) ? SELECT : 0) |
                (START_BUTTON(gp) ? START : 0) |
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
            // if (pushed & SELECT)
            // {
            //     APU_Mute = !APU_Mute;
            //     debugf("APU %s\n", APU_Mute ? "Muted" : "Unmuted");
            // }

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
                fps_enabled = !fps_enabled;
                debugf("FPS %s\n", fps_enabled ? "Enabled" : "Disabled");
                // rapidFireMask[i] ^= A_BUTTON(gp);
            }
            if (pushed & B)
            {
                // rapidFireMask[i] ^= B_BUTTON(gp);
                APU_Mute = !APU_Mute;
                debugf("APU %s\n", APU_Mute ? "Muted" : "Unmuted");
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

#if 0
int InfoNES_GetSoundBufferSize()
{
    // debugf("GetSoundBufferSize\n");
    return 16; // 
}
#endif

void(InfoNES_SoundOutput)(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5)
{
    short p[2];
    if ( samples == 0 )
    {
        return;
    }
    // debugf("InfoNES_SoundOutput %d\n", samples);
    while (samples--)
    {

        int w1 = *wave1++;
        int w2 = *wave2++;
        int w3 = *wave3++;
        int w4 = *wave4++;
        int w5 = *wave5++;
        //            w3 = w2 = w4 = w5 = 0;
        int l = w1 * 6 + w2 * 3 + w3 * 5 + w4 * 3 * 17 + w5 * 2 * 32;
        int r = w1 * 3 + w2 * 6 + w3 * 5 + w4 * 3 * 17 + w5 * 2 * 32;
        
        p[0] = (short)l;
        p[1] = (short)r;
        audio_push(p,2,true);

        // pulse_out = 0.00752 * (pulse1 + pulse2)
        // tnd_out = 0.00851 * triangle + 0.00494 * noise + 0.00335 * dmc

        // 0.00851/0.00752 = 1.131648936170213
        // 0.00494/0.00752 = 0.6569148936170213
        // 0.00335/0.00752 = 0.4454787234042554

        // 0.00752/0.00851 = 0.8836662749706228
        // 0.00494/0.00851 = 0.5804935370152762
        // 0.00335/0.00851 = 0.3936545240893067
    }


}

extern WORD PC;

int framecounter = 0;
int framedisplay = 0;
void InfoNES_LoadFrame()
{
#ifndef NORENDER
    char buffer[15];
    if (fps_enabled)
    {
        char mute = APU_Mute ? 'M' : ' ';
        sprintf(buffer, "FPS: %d %c", framedisplay, mute);
        graphics_draw_text(_dc, 5, 5, buffer);
    }
    display_show(_dc);
#endif
    framecounter++;
    controller_scan();
    gKeys = get_keys_pressed();
}

WORD buf[256];
void(InfoNES_PreDrawLine)(int line)
{
    WORD *buffer = nullptr;
#ifndef NORENDER
    if (line == 4)
    {
        // debugf("Getting Display Context %d\n", line);
        _dc = display_get();
    }
    buffer = ((WORD *)(_dc)->buffer) + (line << 8);
    assert(buffer != nullptr);
#else
    buffer = buf;
#endif
    // get the correct position in the framebuffer

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
// void vblCallback(void)
// {
//     gTicks++;
// }

void frameratecalc(int ovfl)
{
    debugf("%d\n", framecounter);
    framedisplay = framecounter;
    framecounter = 0;
}

void checkcontrollers()
{
    controller1IsInserted = controller2IsInserted = false;
    int controllers = get_controllers_present();
    if (controllers & CONTROLLER_1_INSERTED)
    {
        debugf("Controller 1 inserted\n");
        controller1IsInserted = true;
    }
    if (controllers & CONTROLLER_2_INSERTED)
    {
        debugf("Controller 2 inserted\n");
        controller2IsInserted = true;
    }
}

int main()
{
    APU_Mute = SOUNDISMUTED;
    debug_init(DEBUG_FEATURE_LOG_ISVIEWER);
    debugf("Starting InfoNES 64, a Nintendo Entertainment System emulator for the Nintendo 64\n");
    debugf("Built on %s %s using libdragon\n", __DATE__, __TIME__);
    debugf("Now running %s\n", GetBuiltinROMName());

    /* Initialize peripherals */
    display_init(RESOLUTION_256x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    // register_VI_handler(vblCallback);
    controller_init();
    debugf("Init audio\n");
    audio_init(44100, 4);
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
    checkcontrollers();
    InfoNES_Main();
    return 0;
}

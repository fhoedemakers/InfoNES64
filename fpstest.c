#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

int framecounter = 0;
int framedisplay = 0;
void frameratecalc(int ovfl) {
    framedisplay = framecounter;
    framecounter = 0;
}

int main() {
    surface_t *_dsp;
    char buffer[15];
    debug_init(DEBUG_FEATURE_LOG_ISVIEWER);
    display_init(RESOLUTION_256x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    controller_init();
    timer_init();
    new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, frameratecalc);
    uint32_t color = graphics_make_color(0, 0, 0, 255);
    while(true) {
        sprintf(buffer, "FPS: %d", framedisplay);
        _dsp = display_get();
        graphics_fill_screen(_dsp, color);
        graphics_draw_text(_dsp, 10, 10, buffer);
        display_show(_dsp);
        framecounter++;
    }
}
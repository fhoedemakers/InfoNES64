
#include <stdio.h>

#define RGB888_TO_RGB5551(r, g, b) (((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1)

int color_array[] = {
    0x626262,
    0x001FB2,
    0x2404C8,
    0x5200B2,
    0x730076,
    0x800024,
    0x730B00,
    0x522800,
    0x244400,
    0x005700,
    0x005C00,
    0x005324,
    0x003C76,
    0x000000,
    0x000000,
    0x000000,
    0xABABAB,
    0x0D57FF,
    0x4B30FF,
    0x8A13FF,
    0xBC08D6,
    0xD21269,
    0xC72E00,
    0x9D5400,
    0x607B00,
    0x209800,
    0x00A300,
    0x009942,
    0x007DB4,
    0x000000,
    0x000000,
    0x000000,
    0xFFFFFF,
    0x53AEFF,
    0x9085FF,
    0xD365FF,
    0xFF57FF,
    0xFF5DCF,
    0xFF7757,
    0xFA9E00,
    0xBDC700,
    0x7AE700,
    0x43F611,
    0x26EF7E,
    0x2CD5F6,
    0x4E4E4E,
    0x000000,
    0x000000,
    0xFFFFFF,
    0xB6E1FF,
    0xCED1FF,
    0xE9C3FF,
    0xFFBCFF,
    0xFFBDF4,
    0xFFC6C3,
    0xFFD59A,
    0xE9E681,
    0xCEF481,
    0xB6FB9A,
    0xA9FAC3,
    0xA9F0F4,
    0xB8B8B8,
    0x000000,
    0x000000,
};
// Converts NES color pallette from RGB888 to RGB5551 
// Colors from palette are extracted from the html table at https://www.nesdev.org/wiki/PPU_palettes and converted to RGB5551
main() {

    for ( int i = 0; i < sizeof(color_array) / sizeof(int); i++ ) {
        int r = (color_array[i] >> 16) & 0xFF;
        int g = (color_array[i] >> 8) & 0xFF;
        int b = color_array[i] & 0xFF;
        printf("0x%04X, ", RGB888_TO_RGB5551(r, g, b));
    }
}

#include "ppu.h"
#include "cpu.h"
#include "disp.h"
#include <log.h>

const u8 PALETTE_2C02_NTSC[192] = {
    0x52, 0x52, 0x52, 0x00, 0x00, 0x80, 0x08, 0x00, 0x80, 0x2e, 0x00, 0x7e, 0x4a, 0x00, 0x4e, 0x50,
    0x00, 0x06, 0x44, 0x00, 0x00, 0x26, 0x08, 0x00, 0x0a, 0x20, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x26, 0x0a, 0x00, 0x1c, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xa4, 0xa4, 0xa4, 0x00, 0x38, 0xce, 0x34, 0x16, 0xec, 0x5e, 0x04, 0xdc, 0x8c, 0x00, 0xb0, 0x9a,
    0x00, 0x4c, 0x90, 0x18, 0x00, 0x70, 0x36, 0x00, 0x4c, 0x54, 0x00, 0x0e, 0x6c, 0x00, 0x00, 0x74,
    0x00, 0x00, 0x6c, 0x2c, 0x00, 0x5e, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0x4c, 0x9c, 0xff, 0x7c, 0x78, 0xff, 0xa6, 0x64, 0xff, 0xda, 0x5a, 0xff, 0xf0,
    0x54, 0xc0, 0xf0, 0x6a, 0x56, 0xd6, 0x86, 0x10, 0xba, 0xa4, 0x00, 0x76, 0xc0, 0x00, 0x46, 0xcc,
    0x1a, 0x2e, 0xc8, 0x66, 0x34, 0xc2, 0xbe, 0x3a, 0x3a, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xb6, 0xda, 0xff, 0xc8, 0xca, 0xff, 0xda, 0xc2, 0xff, 0xf0, 0xbe, 0xff, 0xfc,
    0xbc, 0xee, 0xfa, 0xc2, 0xc0, 0xf2, 0xcc, 0xa2, 0xe6, 0xda, 0x92, 0xcc, 0xe6, 0x8e, 0xb8, 0xee,
    0xa2, 0xae, 0xea, 0xbe, 0xae, 0xe8, 0xe2, 0xb0, 0xb0, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


// TODO sprite evaluation
// TODO y increment and x increment in v

void ppu_coarse_x_incr(ppu_state_t *st) {

}

void ppu_coarse_y_incr(ppu_state_t *st) {

}

void ppu_prerender_scanline_tick(ppu_state_t *ppu_st, cpu_state_t *cpu_st, disp_t *disp) {
    switch (ppu_st->_col) {
        case 1:
            ppu_st->ppustatus.V = ppu_st->ppustatus.S = ppu_st->ppustatus.O = 0;

    }
}

void ppu_render_visible_scanline_tick(ppu_state_t *ppu_st, cpu_state_t *cpu_st, disp_t *disp) {
    if (ppu_st->_col < 256) {
        // log_info("%d %d", ppu_st->_row, ppu_st->_col);
        disp_putpixel(disp, ppu_st->_col, ppu_st->_row, ppu_st->_row, ppu_st->_col, 0xFF);
    }
}

void ppu_postrender_scanline_tick(ppu_state_t *ppu_st, disp_t *disp) {
    if (ppu_st->_col == 1) {
        disp_blit(disp); // ppu_st->ppustatus.V = 1;
    }
}

void ppu_tick(ppu_state_t *ppu_st, cpu_state_t *cpu_st, disp_t *disp) {

    // tick forward and produce one pixel worth of data 
    // it's okay to coalesce processing logic and do multiple pixel writes in 
    // a single cycle

    // TODO sprite evaluation
    // TODO rendering (read nametable byte, attribute byte, pattern table lo/hi tile)

    switch (ppu_st->_row) {
        case 261: ppu_prerender_scanline_tick(ppu_st, cpu_st, disp); break;
        case 0 ... 239: ppu_render_visible_scanline_tick(ppu_st, cpu_st, disp); break;
        case 241: ppu_postrender_scanline_tick(ppu_st, disp); break;
    }

    ppu_st->_col = (ppu_st->_col+1)%341;
    if (ppu_st->_col == 0) ppu_st->_row = (ppu_st->_row+1)%262;

}

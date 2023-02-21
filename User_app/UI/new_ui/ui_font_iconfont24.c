/*******************************************************************************
 * Size: 16 px
 * Bpp: 4
 * Opts: --bpp 4 --size 16 --font C:\Users\001\Desktop\sample\assets\iconfont.ttf -o C:\Users\001\Desktop\sample\assets\ui_font_iconfont24.c --format lvgl -r 0xe6f9 -r 0xe6eb -r 0xe6e7 --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_ICONFONT24
#define UI_FONT_ICONFONT24 1
#endif

#if UI_FONT_ICONFONT24

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E6E7 "" */
    0x0, 0x5b, 0xb6, 0x0, 0x8, 0xc5, 0x5c, 0x90,
    0xd, 0x2, 0x20, 0xc1, 0x0, 0xe, 0xe0, 0x0,
    0x0, 0x6, 0x60, 0x0,

    /* U+E6EB "" */
    0x0, 0x2, 0x55, 0x20, 0x0, 0x2, 0xbe, 0xaa,
    0xec, 0x20, 0x2e, 0x70, 0x0, 0x6, 0xe2, 0xa7,
    0x5, 0xbb, 0x60, 0x6b, 0x20, 0x8c, 0x44, 0xb9,
    0x2, 0x0, 0xd0, 0x22, 0xc, 0x10, 0x0, 0x0,
    0xee, 0x0, 0x0, 0x0, 0x0, 0x77, 0x0, 0x0,

    /* U+E6F9 "" */
    0x0, 0x0, 0x6a, 0xcc, 0xa6, 0x0, 0x0, 0x0,
    0x6e, 0xb6, 0x33, 0x6b, 0xe6, 0x0, 0x8, 0xd2,
    0x2, 0x55, 0x20, 0x2d, 0x80, 0x6d, 0x2, 0xbe,
    0xaa, 0xec, 0x20, 0xc6, 0x31, 0x2e, 0x70, 0x0,
    0x6, 0xe2, 0x13, 0x0, 0xa7, 0x5, 0xbb, 0x60,
    0x6b, 0x0, 0x0, 0x20, 0x8c, 0x44, 0xb9, 0x2,
    0x0, 0x0, 0x0, 0xd0, 0x22, 0xc, 0x10, 0x0,
    0x0, 0x0, 0x0, 0xee, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x76, 0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 8, .box_h = 5, .ofs_x = 4, .ofs_y = 1},
    {.bitmap_index = 20, .adv_w = 256, .box_w = 10, .box_h = 8, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 60, .adv_w = 256, .box_w = 14, .box_h = 10, .ofs_x = 1, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x4, 0x12
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 59111, .range_length = 19, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t ui_font_iconfont24 = {
#else
lv_font_t ui_font_iconfont24 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
    .base_line = -1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if UI_FONT_ICONFONT24*/


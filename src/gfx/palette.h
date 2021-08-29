#ifndef palette_include_file
#define palette_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define palette_width 255
#define palette_height 1
#define palette_size 257
#define palette ((gfx_sprite_t*)palette_data)
extern unsigned char palette_data[257];

#ifdef __cplusplus
}
#endif

#endif

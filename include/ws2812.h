#ifndef WS2812_H
#define WS2812_H

static inline void put_pixel(uint32_t pixel_grb);

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

void set_one_led(uint8_t r, uint8_t g, uint8_t b, uint8_t intensidade);

#endif
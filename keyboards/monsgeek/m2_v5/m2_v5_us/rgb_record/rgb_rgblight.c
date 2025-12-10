// Copyright 2024 Su (@isuua)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

void rgb_light_color_set(uint8_t r, uint8_t g, uint8_t b) {
    for(uint8_t i = 0; i < RGBLED_NUM; i++) {
        rgb_matrix_set_color(i, r, g, b);
    }
}
static uint8_t neon_color_index = 0;
void hs_rgblight_increase(uint8_t mode) {

    switch(mode) {
        case 0: { 
            static uint16_t rainbow_counter = 0;
            static uint32_t last_update = 0;
            uint32_t interval = 15; 

            if (timer_elapsed32(last_update) >= interval) {
                last_update = timer_read32();
                rainbow_counter += 3;
            }

            for (uint8_t i = 0; i < RGBLED_NUM; i++) {
                uint8_t hue = (rainbow_counter + i * 8) % 255;
                HSV hsv = { .h = hue, .s = 255, .v = (RGBLIGHT_LIMIT_VAL) };
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
        } break;

        case 1: { 
            static uint16_t hue = 0;            
            static uint32_t last_update = 0;

            uint8_t hue_step = 3; 
            neon_color_index = 0;

            if (timer_elapsed32(last_update) >= 20) { 
                last_update = timer_read32();
                hue = (hue + hue_step) % 255;
            }

            HSV hsv = { .h = hue, .s = 255, .v = RGBLIGHT_LIMIT_VAL };
            RGB rgb = hsv_to_rgb(hsv);
            rgb_light_color_set(rgb.r, rgb.g, rgb.b);
        } break;

        case 2: { 
            static const RGB neon_colors[] = {
                {255, 0, 0},        
                {255, 255, 0},      
                {0, 255, 0},        
                {0, 255, 255},      
                {0, 0, 255},        
                {255, 0, 255},      
                {255, 255, 255},    
            };

            static uint8_t brightness = 0;
            static bool increasing = true;
            static uint32_t last_update = 0;

            uint32_t interval = 15;
            uint8_t max_val = RGBLIGHT_LIMIT_VAL;
            uint8_t step = 1;

            if (timer_elapsed32(last_update) >= interval) {
                last_update = timer_read32();

                if (increasing) {
                    brightness += step;
                    if (brightness >= max_val) {
                        brightness = max_val;
                        increasing = false;
                    }
                } else {
                    if (brightness > step) {
                        brightness -= step;
                    } else {
                        brightness = 0;
                        increasing = true;
                        neon_color_index = (neon_color_index + 1) % ARRAY_SIZE(neon_colors);
                    }
                }
            }

            RGB col = neon_colors[neon_color_index];

            uint8_t r = (col.g * brightness) / 255;
            uint8_t g = (col.r * brightness) / 255;
            uint8_t b = (col.b * brightness) / 255;

            rgb_light_color_set(r, g, b);
        } break;

        case 3: 
            rgb_light_color_set(RGBLIGHT_LIMIT_VAL, 0, 0); break;

        case 4: 
            rgb_light_color_set(RGBLIGHT_LIMIT_VAL, RGBLIGHT_LIMIT_VAL, 0); break;

        case 5: 
            rgb_light_color_set(0, RGBLIGHT_LIMIT_VAL, 0); break;

        case 6:
            rgb_light_color_set(0, RGBLIGHT_LIMIT_VAL, RGBLIGHT_LIMIT_VAL); break;

        case 7:
            rgb_light_color_set(0, 0, RGBLIGHT_LIMIT_VAL); break;

        case 8:
            rgb_light_color_set(RGBLIGHT_LIMIT_VAL, 0, RGBLIGHT_LIMIT_VAL); break;

        case 9:
            rgb_light_color_set(RGBLIGHT_LIMIT_VAL, RGBLIGHT_LIMIT_VAL, RGBLIGHT_LIMIT_VAL); break;

        case 10:
            rgb_light_color_set(0, 0, 0); break;

        default: mode = 0; break;
    }
}



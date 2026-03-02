#ifndef SNAKE_H
#define SNAKE_H

#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>

#define GRID_SIZE 20
#define MAX_SNAKE_LENGTH 50
#define COLOR_APPLE {0,0,255,0} // b,g,r,reserved
#define COLOR_SNAKE {0,255,0,0}
#define COLOR_BACKGROUND_1{10,10,10,0}
#define COLOR_BACKGROUND_2 {20,20,20,0}
#define COLOR_PROGRESSBAR {0,255,0,0}
#define COLOR_PROGRESSBAR_BG {100,100,100,0}
#define FPS 5
#define START_SNAKE_LENGTH 3
#define APPLE_SPAWN_CHANCE 50
#define SCORE_BAR_HEIGHT 10
#define WINNING_SCORE 10


typedef enum {
    UP , DOWN , LEFT , RIGHT
} Direction;

typedef struct {
    UINT32 x;
    UINT32 y;
} Point;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    UINT32 length;
    Direction dir;
    UINT32 score;
} Snake;

typedef struct {
    BOOLEAN active;
    Point position;
} Apple;

typedef struct {
    UINT32 screen_w;
    UINT32 screen_h;
    UINT32 fps;
    UINT32 grid_size;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL snake_color;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL apple_color;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL background_color_1;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL background_color_2;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL progressbar_color;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL progressbar_bg_color;
    UINT32 block_size;
    UINT32 game_size_pixels;
    UINT32 offset_x;
    UINT32 offset_y;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *backBuffer;
    UINT32 hud_height;
} GameSettings;

static inline BOOLEAN pointsEqual(Point p1, Point p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}


#endif SNAKE_H
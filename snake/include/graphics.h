#ifndef GRAPHICS_H
#define GRAPHICS_H


#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "snake.h"

void initialize(GameSettings *settings,EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 gridSize, UINT32 hudHeight);
void drawSnakeToBuffer(Snake *snake, GameSettings *settings);
void drawAppleToBuffer(Apple *apple, GameSettings *settings);
void drawGridBackground(GameSettings *settings);
void clearBuffer(GameSettings *settings);
void drawBlock(UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, GameSettings *s);
void clearScreen(void);
void resetScreen(BOOLEAN extendedVerification);
void drawHUD(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, GameSettings *s, UINT32 currentScore);
void show_game_over_screen(BOOLEAN won, GameSettings *s, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);
#endif

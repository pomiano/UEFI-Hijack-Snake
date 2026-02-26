#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H


#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "snake.h"

void updateDirection(Snake *snake);
BOOLEAN moveSnake(Snake *snake, Apple *apple, EFI_RNG_PROTOCOL *erp, GameSettings *settings);
void initializeSnakeAndApple(Snake *snake, EFI_RNG_PROTOCOL *erp, GameSettings *settings, Apple *apple);
void spawnApple(Apple *apple, Snake *snake,EFI_RNG_PROTOCOL *erp, GameSettings *settings);
BOOLEAN checkCollisionWithApple(Point head, Apple *apple);
BOOLEAN hasHitWall(Point *head, GameSettings *settings);
BOOLEAN hasHitSelf(Point *head, Snake *snake);
UINT32 getRandom(EFI_RNG_PROTOCOL *erp);

#endif

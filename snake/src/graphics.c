#include "graphics.h"

void drawBlock(UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, GameSettings *s) {
    UINTN start_x = x * s->block_size;
    UINTN start_y = y * s->block_size;

    for(UINTN i=0;i<s->block_size;i++){
        for(UINTN j=0;j<s->block_size;j++){
            UINTN current_x = start_x + j;
            UINTN current_y = start_y + i;

            if (current_x < s->game_size_pixels && current_y < s->game_size_pixels) {
                s->backBuffer[current_y * s->game_size_pixels + current_x] = color;
            }   
        }
    }
}

void initialize(GameSettings *s,EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 gridSize, UINT32 hudHeight){
    s->screen_w = gop->Mode->Info->HorizontalResolution;
    s->screen_h = gop->Mode->Info->VerticalResolution;
    
    s->apple_color = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)COLOR_APPLE;
    s->snake_color = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)COLOR_SNAKE;
    s->background_color_1 = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)COLOR_BACKGROUND_1;
    s->background_color_2 = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)COLOR_BACKGROUND_2;
    s->progressbar_color = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)COLOR_PROGRESSBAR;
    s->progressbar_bg_color= (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)COLOR_PROGRESSBAR_BG;
    s->grid_size = gridSize;

    UINT32 game_height = s->screen_h - hudHeight;

    s->block_size = (s->screen_w < game_height) ? (s->screen_w / gridSize) : (game_height / gridSize);
    s->game_size_pixels = s->block_size * s->grid_size;

    UINT32 margin_x = (s->screen_w - s->game_size_pixels) / 2;
    UINT32 margin_y = (game_height - s->game_size_pixels) / 2;

    s->offset_x = margin_x;
    s->offset_y = hudHeight + margin_y;

    UINTN frame_size = s->game_size_pixels * s->game_size_pixels  * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    
    EFI_STATUS status = BS->AllocatePool(EfiLoaderData, frame_size, (VOID**)&s->backBuffer);
    if (EFI_ERROR(status)) {
        printf("Błąd alokacji pamięci\n");
    }

    s->fps = FPS; 
    s->hud_height = hudHeight;
}


void drawSnakeToBuffer(Snake *snake, GameSettings *settings) {
    for (UINT32 i = 0; i < snake->length; i++) {
        drawBlock(snake->body[i].x, snake->body[i].y,settings->snake_color ,settings);
    }
}

void drawAppleToBuffer(Apple *apple, GameSettings *settings){
    if(apple->active == FALSE)
        return;

    drawBlock(apple->position.x, apple->position.y, settings->apple_color,settings);
}

void drawGridBackground(GameSettings *settings) {
    for(UINT32 i=0;i<settings->grid_size;i++){
        for(UINT32 j=0; j<settings->grid_size;j++){
            EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
            if((i + j) % 2 == 0)
                color = settings->background_color_1;
            else
                color = settings->background_color_2;
            drawBlock(i,j,color,settings);

        }
    }
}


void clearBuffer(GameSettings *settings){
    UINTN buffer_size = settings->game_size_pixels  * settings->game_size_pixels  * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    memset(settings->backBuffer, 0, buffer_size);
}


void clearScreen(void) {
    ST->ConOut->ClearScreen(ST->ConOut);
}

void resetScreen(BOOLEAN extendedVerification) {
    ST->ConOut->Reset(ST->ConOut, extendedVerification);
}

void drawHUD(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, GameSettings *s, UINT32 currentScore){
    UINT32 max_bar_width = s->game_size_pixels;
    UINT32 current_bar_width = (currentScore * max_bar_width) / WINNING_SCORE;

    if (current_bar_width > max_bar_width) current_bar_width = max_bar_width;

    UINT32 barX = s->offset_x;
    UINT32 barY = s->offset_y - s->hud_height;

    //progressbar background
    gop->Blt(
        gop,
        &s->progressbar_bg_color,
        EfiBltVideoFill,
        0,0,
        barX,barY,
        max_bar_width,
        s->hud_height,
        0
    );

    //progressbar
    if(currentScore > 0) {
        gop->Blt(
        gop,
        &s->progressbar_color,
        EfiBltVideoFill,
        0,0,
        barX,barY,
        current_bar_width,
        s->hud_height,
        0
    );
    }

}

void show_game_over_screen(BOOLEAN won, GameSettings *s, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop){ //TODO


    EFI_GRAPHICS_OUTPUT_BLT_PIXEL winColor = {20, 255, 20, 0};
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL loseColor = {20, 20, 255, 0};

    if (won) {
        gop->Blt(gop, &winColor, EfiBltVideoFill, 0, 0, 0, 0, s->screen_w, s->screen_h, 0);
    } else {
        gop->Blt(gop, &loseColor, EfiBltVideoFill, 0, 0, 0, 0, s->screen_w, s->screen_h, 0);
    }

}
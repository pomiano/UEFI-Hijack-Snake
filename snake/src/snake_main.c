#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h> 
#include "game_logic.h"
#include "graphics.h"




int main(int argc, char **argv) {
    resetScreen(0);

    EFI_STATUS status;

    GameSettings settings;
    EFI_RNG_PROTOCOL *erp = NULL;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;

    Snake mySnake;
    Apple myApple;

    //gop
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    status = BS->LocateProtocol(&gop_guid, NULL, (VOID**)&gop);
    if(EFI_ERROR(status)) return status;

    //rng
    EFI_GUID erp_guid = EFI_RNG_PROTOCOL_GUID;
    status = BS->LocateProtocol(&erp_guid,NULL,(VOID**)&erp);
    if (EFI_ERROR(status)) {
        erp = NULL; 
    }

    initialize(&settings, gop, GRID_SIZE, SCORE_BAR_HEIGHT);
    initializeSnakeAndApple(&mySnake, erp, &settings, &myApple);

    BOOLEAN running = TRUE;
    BOOLEAN won = FALSE;
    
    while(running) {
        clearBuffer(&settings);

        drawGridBackground(&settings);

        drawSnakeToBuffer(&mySnake, &settings);
        drawAppleToBuffer(&myApple, &settings);
        drawHUD(gop,&settings,mySnake.score);
                
        gop->Blt(
            gop,
            settings.backBuffer,
            EfiBltBufferToVideo,
            0, 0, 
            settings.offset_x,
            settings.offset_y,
            settings.game_size_pixels,
            settings.game_size_pixels,
            0
        );

        if(mySnake.score >= WINNING_SCORE){
            won = TRUE;
            running = FALSE;
        }

        updateDirection(&mySnake);
        if(moveSnake(&mySnake,&myApple,erp,&settings) == FALSE){
            won = FALSE;
            running = FALSE;
        }


        spawnApple(&myApple, &mySnake,erp,&settings);

        BS->Stall(1000000/settings.fps);  //delay
    }

    show_game_over_screen(won, &settings,gop);

    BS->Stall(3000000);
    
    if(!won)
        return 1;

    return 0;

}
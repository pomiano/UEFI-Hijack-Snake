#include "game_logic.h"

void updateDirection(Snake *snake) {
    EFI_INPUT_KEY key;
    EFI_STATUS status;
    ST->ConIn->ReadKeyStroke(ST->ConIn,&key);

    if(EFI_ERROR(status))
        return;

    UINT16 k = key.ScanCode;

    if((snake->dir==UP && k == SCAN_DOWN) || 
        (snake->dir == DOWN && k == SCAN_UP) || 
        (snake->dir == LEFT && k == SCAN_RIGHT) || 
        (snake->dir == RIGHT && k == SCAN_LEFT)){
        return;
    }

    switch(k){
        case SCAN_UP:
            snake->dir = UP;
            break;
        case SCAN_DOWN:
            snake->dir = DOWN;
            break;
        case SCAN_RIGHT:
            snake->dir = RIGHT;
            break;
        case SCAN_LEFT:
            snake->dir = LEFT;
            break;
        default:
            break;
    }
}


BOOLEAN moveSnake(Snake *snake, Apple *apple, EFI_RNG_PROTOCOL *erp, GameSettings *settings) { //false if failure
    Point new_head;
    switch(snake->dir) {
        case UP:
            new_head.x = snake->body[0].x;
            new_head.y = snake->body[0].y-1;
           break;
        case DOWN:
            new_head.x = snake->body[0].x;
            new_head.y = snake->body[0].y+1;
            break;
        case RIGHT:
            new_head.x = snake->body[0].x+1;
            new_head.y = snake->body[0].y;
            break;
        case LEFT:
            new_head.x = snake->body[0].x-1;
            new_head.y = snake->body[0].y;
            break;
    }

    if(hasHitWall(&new_head, settings) || hasHitSelf(&new_head, snake)) {
        return FALSE; 
    }
    
    BOOLEAN ateApple = checkCollisionWithApple(new_head, apple);    
    if(ateApple){
        snake->score+=10;
        if(snake->length<MAX_SNAKE_LENGTH)
            snake->length++;
        apple->active = FALSE;
    }

    for(int i=snake->length;i>0;i--){
        snake->body[i].x = snake->body[i-1].x;
        snake->body[i].y = snake->body[i-1].y;
    }
    snake->body[0].x = new_head.x;
    snake->body[0].y = new_head.y;

    return TRUE;

}

void initializeSnakeAndApple(Snake *snake, EFI_RNG_PROTOCOL *erp, GameSettings *settings, Apple *apple) {
    UINT8 randNum;
    snake->length = START_SNAKE_LENGTH;

    // if(erp && !EFI_ERROR(erp->GetRNG(erp, NULL, 1, &randNum))){
    //     snake->dir = (Direction)(randNum % 4);
    // } else{
    //     snake->dir = RIGHT;
    // }
    snake->dir = RIGHT;
    UINT32 center = settings->grid_size/2;
    
    for(UINT32 i=0;i<START_SNAKE_LENGTH;i++) {
        snake->body[i].x = center;
        snake->body[i].y = center - i;
    }

    snake->score = 0;
    apple->active = FALSE;
}

void spawnApple(Apple *apple, Snake *snake,EFI_RNG_PROTOCOL *erp, GameSettings *settings) {
    if(apple->active)
        return;

    if((getRandom(erp) % 100) > APPLE_SPAWN_CHANCE)
        return;

    BOOLEAN onSnake;
    do{
        onSnake = FALSE;
        UINT32 randX = getRandom(erp);
        UINT32 randY = getRandom(erp);
        
        apple->position = (Point){randX % settings->grid_size , randY % settings->grid_size};

        for(UINT32 i=0; i<snake->length;i++){
            if(pointsEqual(snake->body[i],apple->position) == TRUE){
                onSnake = TRUE;
                break;
            }
        }


    }while(onSnake);
    apple->active = TRUE;
}


UINT32 getRandom(EFI_RNG_PROTOCOL *erp) {
    UINT32 val;
    if(erp != NULL) {
        EFI_STATUS status = erp->GetRNG(erp, NULL, sizeof(val), (UINT8*)&val);
        if (!EFI_ERROR(status)) {
            return val;
        }
    }

    EFI_TIME time;
    RT->GetTime(&time, NULL);
    val = (time.Nanosecond) ^ (time.Second << 24) ^ (time.Minute);
    return val;
}

BOOLEAN checkCollisionWithApple(Point head, Apple *apple) {
    if(!apple->active)
        return FALSE;
    if(pointsEqual(head, apple->position) == TRUE)
        return TRUE;
    return FALSE;
}


BOOLEAN hasHitWall(Point *head, GameSettings *settings) {
    if(head->x >= settings->grid_size || head->y >= settings->grid_size)
        return TRUE;
    return FALSE;
}

BOOLEAN hasHitSelf(Point *head, Snake *snake) {
    for(UINT32 i=0;i<snake->length-1;i++) { //except last
        if(pointsEqual(snake->body[i], *head) == TRUE)
            return TRUE;
    }
    return FALSE;
}

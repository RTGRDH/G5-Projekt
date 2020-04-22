#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include "Player.h"
#include "ball.h"


const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;
bool init();
void renderBackground();
bool initPlayField();
bool initMedia();
int collisionDetectionXpos(int x_pos);
int collisionDetectionYpos(int y_pos);


int BallcollisionDetectionYpos(int y_pos);
int BallcollisionDetectionXpos(int x_pos);

bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall);

int determineVelocityX(bool left, bool right, float startingSpeed);
int determineVelocityY(bool up, bool down, float startingSpeed);
void speedLimit(Player p);
void colissionDetectionPlayerArena(Player p);



SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface *imageSurface =  NULL;
SDL_Surface *sPlayer = NULL;
SDL_Surface *sBall = NULL;
SDL_Surface *sGoal_Left = NULL;
SDL_Surface *sGoal_Right = NULL;

SDL_Texture *mField;
SDL_Texture *mBall = NULL;
SDL_Texture *mPlayer = NULL;
SDL_Texture *mGoal_Left = NULL;
SDL_Texture *mGoal_Right = NULL;

Player player = NULL;
Ball b = NULL;
SDL_Rect gField;
// struct to hold the position and size of the sprite
SDL_Rect gPlayer;
SDL_Rect gBall;
SDL_Rect gGoal_Left;
SDL_Rect gGoal_Right;

#define SPEED (150); //75 is optimal, 300 for dev.
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define TURNING_SPEED 10
#define ACCELERATION 0.1

int main(int argc, char * argv[])
{
    /**
     Implement into player object?
     Not done yet.
     */
    bool running = true;
    if(init())
    {
        printf("Initialize window and renderer successful.\n");
    }
    //Init backround here
    if(!initPlayField())
    {
        SDL_FreeSurface(imageSurface);
        imageSurface = NULL;
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(mField);
        SDL_DestroyTexture(mPlayer);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    else
    {
        printf("Initialize playfield successful.\n");
    }
    //Init other graphical media.
    if(!initMedia())
    {
        SDL_FreeSurface(imageSurface);
        imageSurface = NULL;
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(mField);
        SDL_DestroyTexture(mPlayer);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    else
    {
        printf("Initialize media successful.\n");
    }
           
    setPlayerPositionX(player, 0);
    setPlayerDirection(player, 90);
    setPlayerPositionY(player, (WINDOW_HEIGTH - gPlayer.h) / 2);
    float x_pos = getPlayerPositionX(player);
    float y_pos = getPlayerPositionY(player);

    float ball_xPos = 470;
    float ball_yPos = 260;
 
    // keep track of which inputs are given
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
        
    
    while(running)
    {
    /**
    While loop checking if an event occured.
     Code taken from Jonas Willén, SDL_net.zip
     */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = true;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = true;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = true;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = true;
                        break;
                    default:
                        break;
                }
                break;
                case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = false;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = false;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = false;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = false;
                        break;
                    default:
                        break;
                }
                break;
            }
        }

         //Update attributes of the struct
        if (up == true)
            changePlayerSpeed(player, ACCELERATION);
        if (down == true)
            changePlayerSpeed(player, -ACCELERATION);
            speedLimit(player);
        if (left == true)
            changePlayerDirection(player, TURNING_SPEED - getPlayerSpeed(player));      //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds
        if (right == true)
            changePlayerDirection(player, -TURNING_SPEED + getPlayerSpeed(player));     //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds
      
        //Update position of the struct
        updatePlayerPosition(player, 1);
        colissionDetectionPlayerArena(player);

        //Update positions of the struct
        float x_pos = getPlayerPositionX(player);
        float y_pos = getPlayerPositionY(player);

        // set the positions in the struct
        gPlayer.y = collisionDetectionYpos(y_pos);
        gPlayer.x = collisionDetectionXpos(x_pos);

        printf("%f",getPlayerDirection(player));

        if(PlayerBallCollision(&gPlayer, &gBall)){
            if(up)
            {
                gBall.y =  BallcollisionDetectionYpos(gBall.y -200);
            }else if(down){
                gBall.y =  BallcollisionDetectionYpos(gBall.y +200);
            }
            if(right){
                gBall.x = BallcollisionDetectionXpos(gBall.x +200);
            }else if(left){
                gBall.x = BallcollisionDetectionXpos(gBall.x -200);
            }


        }
        
        SDL_RenderClear(renderer);
        renderBackground();
     
        SDL_RenderCopy(renderer,mBall,NULL,&gBall);
        SDL_RenderCopyEx(renderer, mPlayer, NULL, &gPlayer, -getPlayerDirection(player), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
        
        
        SDL_Delay(1000/50);
    }
    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(mField);
    SDL_DestroyTexture(mPlayer);
    SDL_DestroyTexture(mBall);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}



void speedLimit(Player p)   //how fast do the cars go forward and in reverse? if a player goes beyond the cars limits, this sets the car back to it's limits
{
    if (getPlayerSpeed(p)>MAX_SPEED_FORWARD)
        setPlayerSpeed(p, MAX_SPEED_FORWARD);
    if (getPlayerSpeed(p)<MAX_SPEED_REVERSE)
        setPlayerSpeed(p, MAX_SPEED_REVERSE);
}

void colissionDetectionPlayerArena(Player p)    //keeping the abstract version of the car on the arena prevents you getting visually stuck in the corner while lost far off the map
{
    float slow = 0.96;                          //slow factor for colliding with arena walls helps changing direction when you messed up bad, and in theory slows down people who ram it but it's very neglible for that purpose
    if (getPlayerPositionX(p) < 0)
    {
        setPlayerPositionX(p, 0);
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
    if (getPlayerPositionY(p) < 0)
    {
        setPlayerPositionY(p, 0);
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
    if (getPlayerPositionX(p) > WINDOW_WIDTH - getPlayerHeight())
    {
        setPlayerPositionX(p, WINDOW_WIDTH - getPlayerWidth());
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
    if (getPlayerPositionY(p) > WINDOW_HEIGTH - getPlayerHeight())
    {
        setPlayerPositionY(p, WINDOW_HEIGTH - getPlayerHeight());
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
}

/*
 Determines the velocity on y-axis.
 */
int determineVelocityY(bool up, bool down, float startingSpeed)
{
    if (up && !down)
    {
        startingSpeed =- SPEED;
    }
    if (down && !up)
    {
        startingSpeed =+ SPEED;
    }
    return startingSpeed;
}
/*
Determines the velocity on x-axis.
*/
int determineVelocityX(bool left, bool right, float startingSpeed)
{
    if (left && !right)
    {
        startingSpeed =- SPEED;
    }
    if (right && !left)
    {
        startingSpeed =+ SPEED;
    }
    return startingSpeed;
}


/**
 Collisiondetection for moving object on X-axis. Makes sure that the objec stays within the window
 Code taken from Jonas Willén, SDL_net.zip
 */
int collisionDetectionXpos(int x_pos)
{
    if (x_pos <= 0) x_pos = 0;
    if (x_pos >= WINDOW_WIDTH - gPlayer.w) x_pos = WINDOW_WIDTH - gPlayer.w;
    return x_pos;
}
/**
Collisiondetection for moving object on Y-axis. Makes sure that the objec stays within the window
Code taken from Jonas Willén, SDL_net.zip
*/
int collisionDetectionYpos(int y_pos)
{

    if (y_pos <= 0) y_pos = 0;
    if (y_pos >= WINDOW_HEIGTH - gPlayer.h) y_pos = WINDOW_HEIGTH - gPlayer.h;
    return y_pos;
}

int BallcollisionDetectionYpos(int y_pos)
{
    if (y_pos <= 0) y_pos = 0;
    if (y_pos >= WINDOW_HEIGTH - gBall.h) y_pos = WINDOW_HEIGTH - gBall.h;
    return y_pos;

}

int BallcollisionDetectionXpos(int x_pos)
{
    if (x_pos <= 0) x_pos = 0;
    if (x_pos >= WINDOW_WIDTH - gBall.h) x_pos = WINDOW_WIDTH - gBall.h;
    return x_pos;
}


bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall){
    if(gPlayer ->y >= gBall ->y  + gBall ->h)
        return 0;
    if(gPlayer->x >= gBall ->x + gBall->w)
        return 0;
    if(gPlayer->y + gPlayer->h <= gBall->y)
        return 0;
    if(gPlayer ->x + gPlayer->w <= gBall->x)
        return 0;
    return 1;

}
/**
 Init other media
 */
bool initMedia()
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    sPlayer = IMG_Load("images/Player1.png");
    sBall = IMG_Load("images/SoccerBall.png");
 
    
    mPlayer = SDL_CreateTextureFromSurface(renderer, sPlayer);
    mBall = SDL_CreateTextureFromSurface(renderer,sBall);


    player = createPlayer(100, 100);

    b = createBall(470,260);

    gPlayer.x = getPlayerPositionX(player);
    gPlayer.y = getPlayerPositionY(player);
    gPlayer.h = getPlayerHeight();
    gPlayer.w =getPlayerWidth();

    gBall.x = getBallPositionX(b);
    gBall.y = getBallPositionY(b);
    gBall.h = getBallHeight();
    gBall.w = getBallWidth();

    if(NULL == imageSurface)
    {
        printf("\nCould not load image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    return flag;
}
/**
 Init background football field.
 Returns false if image could not be loaded or if SDL_Image could not be initialized.
 */
bool initPlayField()
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    imageSurface = IMG_Load("images/SoccerField.png");
    
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);

    sGoal_Left = IMG_Load("images/Goal.png");
    sGoal_Right = IMG_Load("images/Goal.png");
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);
    mGoal_Left = SDL_CreateTextureFromSurface(renderer, sGoal_Left);
    mGoal_Right = SDL_CreateTextureFromSurface(renderer, sGoal_Right);
    //Left goal rect init position and define width and heigth
    gGoal_Left.h=370;
    gGoal_Left.w = 50;
    gGoal_Left.x = 0;
    gGoal_Left.y = WINDOW_HEIGTH/2 - gGoal_Left.h/2;
    
    //Right goal rect init position and define width and heigth
    gGoal_Right.h=370;
    gGoal_Right.w = 50;
    gGoal_Right.x = WINDOW_WIDTH-gGoal_Right.w;
    gGoal_Right.y = WINDOW_HEIGTH/2 - gGoal_Left.h/2;

    gField.x = 0; gField.y = 0; gField.h = WINDOW_HEIGTH; gField.w = WINDOW_WIDTH;
    if(NULL == imageSurface)
    {
        printf("\nCould not load image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    return flag;
}
/**
 Rendering the background, AKA. Football field and goals.
 */
void renderBackground()
{
    SDL_RenderCopy(renderer, mField, NULL, &gField);
    SDL_RenderCopy(renderer,mGoal_Left, NULL, &gGoal_Left);
    SDL_RenderCopy(renderer, mGoal_Right, NULL, &gGoal_Right);
}

/**
 Init code taken from Jonas Willén.
 Creates window and a renderer.
 Gets windowSurface from window to present background (play field).
 Returns false if init failed.
 */
bool init()
{
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Under production", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_SHOWN);
      
    if(window == NULL)
    {
        printf("Could not create window. Error: %s ",SDL_GetError());
        printf("\n");
        test = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Could not create renderer. Error: %s",SDL_GetError());
        printf("\n");
        test = false;
    }
    return test;
}
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background, *character, *cuadro, *gameover, *start, *saved, *won;
SDL_Rect rect_background, rect_character, rect_cuadro, rect_gameover, rect_start, rect_saved, rect_won;

TTF_Font *font;
SDL_Surface *text;
SDL_Color text_color = {255, 255, 255};
SDL_Texture *textFont;

int cant = 0;
bool startScreen = true;
bool game = false;
bool winScreen = false;
bool saveScreen = false;

using namespace std;

int randomBetween(int min, int max)
{
    return (rand()%(max - min)) + min;
}

bool collisionCheck(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x > r2.x+r2.w)
        return false;
    if(r1.x+r1.w < r2.x)
        return false;
    if(r1.y > r2.y+r2.h)
        return false;
    if(r1.y+r1.h < r2.y)
        return false;
    return true;
}

void playMusic(const char* file)
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *gameSound= Mix_LoadMUS(file);
    Mix_PlayMusic(gameSound, -1);
}

const char* intToConstChar(int x)
{
    stringstream ss;
    ss << x;
    string str = ss.str();
    return str.c_str();
}

void countSnakeEat()
{
    TTF_Init();
    font = TTF_OpenFont("fonts/berlin_normal.ttf", 50);
    text = TTF_RenderText_Solid(font,intToConstChar(cant), text_color);
    textFont = SDL_CreateTextureFromSurface(renderer, text);
    int textW=0, textH=0;
    SDL_QueryTexture(textFont, NULL, NULL, &textW, &textH);
    SDL_Rect rect_font = { 350, 10, textW, textH };
}

int main(int argc, char* args[])
{
    srand(time(0));
    double startPosX = 385;
    double startPosY = 255;
    int posX = randomBetween(25, 715);
    int posY = randomBetween(93, 456);
    int dir = 3;
    int frames = 0;
    int velocity = 5;

    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Snake", 100, 100, 797,/*WIDTH*/ 554,/*HEIGHT*/ SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    TTF_Init();
    font = TTF_OpenFont("fonts/berlin_normal.ttf", 50);
    text = TTF_RenderText_Solid(font,intToConstChar(cant), text_color);
    textFont = SDL_CreateTextureFromSurface(renderer, text);
    int textW=0, textH=0;
    SDL_QueryTexture(textFont, NULL, NULL, &textW, &textH);
    SDL_Rect rect_font = { 350, 10, textW, textH };

    //Play Music
    playMusic("audio/lines_of_code.mp3");

    //Init textures
    int w=0,h=0;

    start = IMG_LoadTexture(renderer,"img/start.png");
    SDL_QueryTexture(start, NULL, NULL, &w, &h);
    rect_start.x = 0; rect_start.y = 0; rect_start.w = w; rect_start.h = h;

    background = IMG_LoadTexture(renderer,"img/tablero.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0; rect_background.y = 0; rect_background.w = w; rect_background.h = h;

    saved = IMG_LoadTexture(renderer,"img/saved.png");
    SDL_QueryTexture(saved, NULL, NULL, &w, &h);
    rect_saved.x = 0; rect_saved.y = 0; rect_saved.w = w; rect_saved.h = h;

    won = IMG_LoadTexture(renderer,"img/won.png");
    SDL_QueryTexture(won, NULL, NULL, &w, &h);
    rect_won.x = 0; rect_won.y = 0; rect_won.w = w; rect_won.h = h;

    character = IMG_LoadTexture(renderer, "img/snake.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = startPosX; rect_character.y = startPosY; rect_character.w = w; rect_character.h = h;

    cuadro = IMG_LoadTexture(renderer, "img/cuadrito.png");
    SDL_QueryTexture(cuadro, NULL, NULL, &w, &h);
    rect_cuadro.x = posX; rect_cuadro.y = posY; rect_cuadro.w = w; rect_cuadro.h = h;

    gameover = IMG_LoadTexture(renderer, "img/gameover.png");
    SDL_QueryTexture(gameover, NULL, NULL, &w, &h);
    rect_gameover.x = 0; rect_gameover.y = 0; rect_gameover.w = w; rect_gameover.h = h;

    //Main Loop
    while(true)
    {
        frames++;
        if(collisionCheck(rect_cuadro, rect_character))
        {
            velocity++;
            posX = randomBetween(25, 715);
            posY = randomBetween(93, 456);
            rect_cuadro.x = posX; rect_cuadro.y = posY;
            cant++;
            cout << cant << endl;
            countSnakeEat();
        }
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
                if(Event.key.keysym.sym == SDLK_LEFT)
                    dir = 4;
                if(Event.key.keysym.sym == SDLK_RIGHT)
                    dir = 3;
                if(Event.key.keysym.sym == SDLK_UP)
                    dir = 1;
                if(Event.key.keysym.sym == SDLK_DOWN)
                    dir = 2;
                if(Event.key.keysym.sym == SDLK_RETURN && !game && !startScreen && !saveScreen)
                {
                    dir = 3;
                    game = true;
                    winScreen = false;
                    startScreen = false;
                    velocity = 5;
                    rect_character.x = startPosX; rect_character.y = startPosY;
                    posX = randomBetween(25, 715);
                    posY = randomBetween(93, 456);
                    rect_cuadro.x = posX; rect_cuadro.y = posY;
                }
                if(Event.key.keysym.sym == SDLK_SPACE && startScreen)
                {
                    winScreen = false;
                    startScreen = false;
                    game = true;
                }
                if(Event.key.keysym.sym == SDLK_s && game)
                {
                    saveScreen = true;
                    ofstream out("SavedGames.txt");
                    out << rect_character.x << " " << rect_character.y << " " << velocity << " " << dir << " " << cant ;
                    out.close();
                    game = false;
                }
                if(Event.key.keysym.sym == SDLK_l && startScreen)
                {
                    ifstream in("SavedGames.txt");
                    in >> rect_character.x >> rect_character.y >> velocity >> dir >> cant;
                    in.close();
                    startScreen = false;
                    game = true;
                }
            }
        }
        if(frames == 100)
            {
            if(dir == 1)
            {
                rect_character.y -= velocity;
            }
            else if(dir == 2)
            {
                rect_character.y += velocity;
            }
            else if(dir == 3)
            {
                rect_character.x += velocity;
            }
            else if(dir == 4)
            {
                rect_character.x -= velocity;
            }
            frames = 0;
        }

        if(startScreen)
        {
            rect_character.x = startPosX; rect_character.y = startPosY;
            SDL_RenderCopy(renderer, start, NULL, &rect_start);
        }
        if(winScreen)
        {
            rect_character.x = startPosX; rect_character.y = startPosY;
            SDL_RenderCopy(renderer, won, NULL, &rect_won);
        }

        if((rect_character.x <= 25 || rect_character.x >= 715 ||
            rect_character.y <= 93  || rect_character.y >= 456) && game)
        {
            cant = 0;
            SDL_RenderCopy(renderer, gameover, NULL, &rect_gameover);
            game = false;
        }

        if(saveScreen)
        {
            SDL_RenderCopy(renderer, saved, NULL, &rect_saved);
            game = false;
            startScreen = false;
        }

        if(cant == 40)
        {
            cant = 0;
            winScreen = true;
            game = false;
            startScreen = false;
        }

        else if(game)
        {
            SDL_RenderCopy(renderer, background, NULL, &rect_background);
            SDL_RenderCopy(renderer, character, NULL, &rect_character);
            SDL_RenderCopy(renderer, cuadro, NULL, &rect_cuadro);
            SDL_RenderCopy(renderer, textFont, NULL, &rect_font);
        }

        SDL_RenderPresent(renderer);
    }
	return 0;
}

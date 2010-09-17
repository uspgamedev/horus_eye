/*This source code copyrighted by Lazy Foo' Productions (2004-2009) and may not
be redestributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *bigtext = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The font that's going to be used
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 0, 255, 255 };

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "TTF Test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "background.png" );

    //Open the font
    font = TTF_OpenFont( "papyrus0.ttf", 28 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( message );

    //Close the font that was used
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

SDL_Surface* renderText(string text)
{
    printf("Rendering text\n");
//    char *ctext, *split;
    string subString, temp(text);
    vector<string> lines;
    vector<string>::iterator it;
    int lineskip = TTF_FontLineSkip(font);
    int nlines=0, n=0, p=0;
    SDL_Surface* fulltext = NULL;
    SDL_Surface* linesurf = NULL;
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    //rect.w = 640;
    //rect.h = 480;
    
    /*ctext = (char*) text.c_str();
    cout << ctext << endl;
    split=strtok(ctext,"\n");*/

    fulltext = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
    SDL_FillRect(fulltext, NULL, SDL_MapRGBA(fulltext->format, 255,0,255, 255));
    SDL_SetColorKey(fulltext, SDL_SRCCOLORKEY, SDL_MapRGB(fulltext->format, 255, 0, 255)); 
   
    cout << "Created rect" << endl;

    n = temp.find("\n");
    while(n!=-1){
        subString = temp.substr(0, n);
        cout<<subString<<endl;
        lines.push_back(subString);
        temp = temp.substr(n+1, string::npos);
        n = temp.find("\n");
    }

    for(it=lines.begin(); it!=lines.end(); it++){
        cout << nlines << ":" << *it << endl;
        linesurf = TTF_RenderText_Solid( font, (*it).c_str(), textColor );
        rect.x = 0;
        rect.y = nlines*lineskip;
        SDL_BlitSurface(linesurf, NULL, fulltext, &rect);
        nlines++;
    }

    /*    while(split != NULL) {
          cout << "line" << nlines << " " << split << endl;
        lines.push_back(split);
        linesurf = TTF_RenderText_Solid( font, split, textColor );
        apply_surface(0, (50+nlines*lineskip), linesurf, screen);
        split=strtok(NULL,"\n");
        nlines++;
    }*/
    
    //cout<<*lines.begin()<<endl;
    //linesurf = TTF_RenderText_Solid( font, (*(lines.begin())).c_str(), textColor );

    return fulltext;

}

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Render the text
    message = renderText("Texto\nTexto\nTexto\nTexto\nTexto");
    bigtext = TTF_RenderText_Solid( font, "Big Text with 5 lines", textColor );

    //If there was an error in rendering the text
    if( message == NULL )
    {
        return 1;
    }
    

    //Apply the images to the screen
    apply_surface( 0, 0, background, screen );
    apply_surface( 0, 0, bigtext, screen );
    apply_surface( 0, 50, message, screen );
    
//    SDL_BlitSurface(message, NULL, screen, &rect);

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //While the user hasn't quit
    while( quit == false )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }
    }

    //Free surfaces and font then quit SDL_ttf and SDL
    clean_up();

    return 0;
}

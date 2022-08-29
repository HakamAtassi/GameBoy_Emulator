#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


static const int WIDTH = 160;
static const int HEIGHT = 144;

void InitGL(){
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -1.0, 1.0);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
}

bool createSDLWindow(){
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		return false ;
	}
	if( SDL_SetVideoMode( WIDTH, HEIGHT, 8, SDL_OPENGL ) == NULL )
	{
		return false ;
	}

	InitGL();

	SDL_WM_SetCaption( "OpenGL Test", NULL );
	return true ;
}


void renderGame(uint8_t screenData[144][160][3]){
	//EmulationLoop() ;
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glLoadIdentity();
 	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
 	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, screenData);
	SDL_GL_SwapBuffers( ) ;
}




int main(){
    createSDLWindow();
    uint8_t screenData[144][160][3];

    for(int i=0;i<144;i++){
        for(int j=0;j<160;j++){
            for(int l=0;l<3;l++){
                screenData[i][j][l]=255;
            }
        }
    }


    for(int i=0;i<144;i++){
        for(int j=0;j<160;j++){
            for(int l=0;l<3;l++){
                screenData[i][j][l]=255;
            }
        }
    }


    bool quit = false ;
    SDL_Event event;

    float fps = 59.73 ;
    float interval = 1000 ;
    interval /= fps ;

    unsigned int time2 = SDL_GetTicks( ) ;

    while (!quit)
    {
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        renderGame(screenData);

    }
    SDL_Quit() ;
    
}
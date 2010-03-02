//Generic Main.cpp file. Working with loading SDL and initializing our wonderful world.
//Tom built this initial state by copying the SDL tutorial started from here:
//http://www.lazyfoo.net/SDL_tutorials/lesson01/windows/msvsnet0508e/index.php

#include <iostream>
#include "SDL.h"
#include "SDL_opengl.h"

const int SCREEN_X = 1680;
const int SCREEN_Y = 1050;

int g_screen = 0;			//The SDL screen variable - currently global.


void init_sdl()
{
	//Initialize SDL
	SDL_Init( SDL_INIT_EVERYTHING ); 

	 
	//need to initialize OpenGL, the Screen, and make sure it works.

	//First, configure GL attributes - size of color buffers, etc.. 
	//http://www.libsdl.org/cgi/docwiki.cgi/SDL_GLattr

	//bits per color. This is 32-bit rendering.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	//enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
	//SDL_GL_SetAttribute(SDL_GL_STEREO, 1);

	if (SDL_SetVideoMode(SCREEN_X, SCREEN_Y, 32, SDL_FULLSCREEN | SDL_OPENGL) == 0)
	{
		std::cout << "Failure" << std::endl;
		SDL_Quit();	
	};

	
	//OpenGL init code that probalby needs updating.
	float aspect = (float)SCREEN_X / (float)SCREEN_Y;
  
      /* Make the viewport cover the whole window */
      glViewport(0, 0, SCREEN_X, SCREEN_Y);
  
      /* Set the camera projection matrix:
       * field of view: 90 degrees
       * near clipping plane at 0.1
       * far clipping plane at 100.0
       */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      
      gluPerspective(60.0, aspect, 0.1, 100.0);
      /* We're done with the camera, now matrix operations 
       * will affect the modelview matrix
       * */
      glMatrixMode(GL_MODELVIEW);
  
      /* set the clear color to gray */
      glClearColor(0.5, 0.5 ,0.5, 0);
      
      /* We want z-buffer tests enabled*/
      glEnable(GL_DEPTH_TEST);
	  
	  glEnable(GL_STENCIL_TEST);
  
      /* Do draw back-facing polygons*/
     glDisable(GL_CULL_FACE);

}

void exit_sdl()
{
	//Quit 	
	SDL_Quit(); 
}



   GLfloat yaw;
   GLfloat pitch;
   int     level;
   
   static void subdivide
   (
       GLfloat point0[3], 
       GLfloat point1[3], 
       GLfloat point2[3], 
       int level
   )
   {
       int coord;
       GLfloat midpoint[3][3];
       
       /* Don't subdivide any further; just draw the triangle */
       if (level==0) {
           glColor3fv(point0);
           glVertex3fv(point0);
           glColor3fv(point1);
           glVertex3fv(point1);
           glColor3fv(point2);
           glVertex3fv(point2);
           return;
       }
   
       /* Calculate a midpoint on each edge of the triangle */
       for(coord = 0; coord<3; coord++) {
           midpoint[0][coord] = (point0[coord] + point1[coord])*0.5;
           midpoint[1][coord] = (point1[coord] + point2[coord])*0.5;
           midpoint[2][coord] = (point2[coord] + point0[coord])*0.5;
       }
   
       /* Subdivide each triangle into three more */    /*     .      */
       level--;                                         /*    /X\     */
       subdivide(point0,midpoint[0],midpoint[2],level); /*   /xxx\    */
       subdivide(point1,midpoint[1],midpoint[0],level); /*  /X\ /X\   */
       subdivide(point2,midpoint[2],midpoint[1],level); /* /XXXVXXX\  */
   }
   
   
   static void repaint(bool left)
   {
       int i;
   
       /* Coordinates of the 6 vertices of the octahedron */
       static GLfloat point[6][3] = { 
           {1.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f},
           {0.0f,1.0f,0.0f},{0.0f,-1.0f,0.0f},
           {0.0f,0.0f,1.0f},{0.0f,0.0f,-1.0f}
       };
   
       /* indices of the vertices of the triangles which make up each of 
        * the 8 faces of the octahedron */
       static int triangle[8][3] = { 
           {2,4,0},{2,0,5},{2,5,1},{2,1,4},{3,0,4},{3,5,0},{3,1,5},{3,4,1}
       };
      
       /* Clear the color plane and the z-buffer */
     
   
       glLoadIdentity();
   
	   if (left) glTranslatef(.1,0,0);
       /* Move the object 2 units away from the camera */
       glTranslatef(0.0f, 0.0f, -2.0f);
   
       /* Rotate the object */
       glRotatef(pitch, 1.0f, 0.0f, 0.0f);
       glRotatef(yaw, 0.0f, 1.0f, 0.0f);

	   
       /* Draw the triangles which make up the object */
       glBegin(GL_TRIANGLES);
   
       for (i=0; i<8; i++) {
           subdivide(point[triangle[i][0]],point[triangle[i][1]],point[triangle[i][2]],level);
       }
       
       glEnd();
   
       /* increment the rotation every frame */
       yaw = yaw + 0.05;
   
       /* finally, swap the back and front buffers */
      // SDL_GL_SwapBuffers();
}

void inputcheck()
{
    SDL_Event event;

    while ( SDL_PollEvent(&event) ) {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                printf("Mouse moved by %d,%d to (%d,%d)\n", 
                       event.motion.xrel, event.motion.yrel,
                       event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("Mouse button %d pressed at (%d,%d)\n",
                       event.button.button, event.button.x, event.button.y);
                break;
			case SDL_KEYDOWN:
				exit(0);
				break;
            case SDL_QUIT:
                exit(0);
				break;
        }
    }
}
void main_loop()
{
	
	unsigned char* stencildata = new unsigned char[SCREEN_X*SCREEN_Y];
	//nuke the stencil buffer
	glClearStencil(0x0);

	for(int y = 0; y < SCREEN_Y; y++)
	{
		for(int x = 0; x < SCREEN_X; x++)
			stencildata[y*SCREEN_X + x] = y % 2; //1 for odd, 0 for even.
	}
	
	
	glDrawPixels(SCREEN_X, SCREEN_Y, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE,  stencildata);
	
	
	while(1) //infinite game loop -handle input here
	{
		//event poll.
		inputcheck();
		//render




		glDrawBuffer(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glStencilFunc(GL_EQUAL, 0x1, 0x1); //if ref & mask == stencil & mask
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //stencil buffer is read only.
		repaint(false);
		
		//move camera

		glStencilFunc(GL_EQUAL, 0x0, 0x1); //if ref & mask == stencil & mask
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //stencil buffer is read only.
		repaint(true);
		
		
		SDL_GL_SwapBuffers();


	};
};


int main(int argc, char* argv[])
{
	init_sdl();

	//Do fun game stuff here....
	//init_game();

	main_loop();
	
	exit_sdl();
	return 0;
}
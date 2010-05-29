/**********************************************************\
            Name:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\

\**********************************************************/
#include <GL/glew.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <iostream>

#include <assert.h>
#include <signal.h>
#include <cmath>
//#include <SDL/SDL_image.h>

#include "Manager\Manager.h"
//#include "Manager\TextureManager.h"
#include "GraphBasis\Vector3.h"
#include "GraphBasis\GLFont.h"
#include "GraphBasis\Shader.h"

#include "GraphBasis\FrameBufferObject.h"
#include "GraphBasis\VertexBufferObject.h"

#include "RayTracerObjects\RTTriangle.h"
#include "RayTracerObjects\RTScene.h"

#include "application.h"

using namespace std;

int    Manager :: appHeight;
int    Manager :: appWidth;
float  Manager :: timeFromLastFrame = 0;
Frames Manager :: fps;

Manager ::ResolutionsProportions Manager :: resolutionProportion;


struct AppOptions
{
   float r;
   float alpha;
   float beta;
   float angInc;
   bool wireframeEnabled;
   bool shaderEnabled;
   bool avgCalculed;
   int benchNumTimes;
   double cpuTime;
   double gpuTime;
   GLFont fontRender;
   Shader* simpleContrastShader;
   Shader* avgLuminanceCalcShader;
   GLint textureLoc;
   GLint lumTextureLoc;
   GLint lumNLoc;
   GLint lumImgSizeLoc;
   GLint textureID;
   GLint textureID2;
   GLint imgAlphaLoc;
   GLint imgAvgLuminanceLoc;
   Vector3 avgLuminanceC;
   Vector3 avgLuminanceG;
   FrameBufferObject *frameBuffer;
   //SDL_Surface* imgSDLSurface;
}app = {
   50,
   1.0,
   0,
   5.,
   false,
   true,
   false,
   10,
   0.0,
   0.0,
   GLFont(),
   NULL,
   NULL,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   Vector3(0.299, 0.587, 0.114),
   Vector3(0.299, 0.587, 0.114),
   NULL,
   //NULL,
};

///////////////
//Extra
//////////////

//////////////////
//Public
//////////////////
Manager :: Manager(int argc, char *argv[],int _appWidth, int _appHeight,string _appTitle,Color _clearColor,bool _fullScreen )
{
   appArgC = argc;
   appArgV = argv;

   appTitle = _appTitle;

   fullScreen = _fullScreen;
   if(fullScreen)
   {
      appWidth = glutGet(GLUT_SCREEN_WIDTH);
      appHeight = glutGet(GLUT_SCREEN_HEIGHT);
   }else
   {

      appWidth = _appWidth;
      appHeight = _appHeight;
   }

//   cout << "Game Resolution"<<appWidth<<"x"<< appHeight <<endl;
//   cout << "Screen Resolution"<< glutGet(GLUT_SCREEN_WIDTH)<<"x"<< glutGet(GLUT_SCREEN_HEIGHT)<<endl;

   int resProp = (int)(((float)appWidth/appHeight)*100); //Precision: 2

   if(resProp == 160)
   {
      resolutionProportion = Res16x10;
      cout << "ResolutionProportion: 16x10\n"<<endl;
   }
   else if(resProp == 177)
   {
      resolutionProportion = Res16x9;
      cout << "ResolutionProportion: 16x9"<<endl;
   }
   else if(resProp == 133)
   {
      resolutionProportion = Res4x3;
      cout << "ResolutionProportion: 4x3"<<endl;
   }
   else
   {
      resolutionProportion  = Other;
      cout << "ResolutionProportion: Other"<<endl;
   }

   clearColor = _clearColor;
}

Manager :: Manager(int argc, char *argv[],string _appTitle,Color _clearColor)
{
   appArgC = argc;
   appArgV = argv;
   appTitle = _appTitle;
   fullScreen = true;
   appWidth = glutGet(GLUT_SCREEN_WIDTH);
   appHeight = glutGet(GLUT_SCREEN_HEIGHT);

//
//   cout << "Game Resolution"<<appWidth<<"x"<< appHeight <<endl;
//   cout << "Screen Resolution"<< glutGet(GLUT_SCREEN_WIDTH)<<"x"<< glutGet(GLUT_SCREEN_HEIGHT) <<endl;

   int resProp = (int)(((float)appWidth/appHeight)*100); //Precision:   2

   if(resProp == 160)
   {
      resolutionProportion = Res16x10;
      cout << "ResolutionProportion: 16x10"<<endl;
   }
   else if(resProp == 177)
   {
      resolutionProportion = Res16x9;
      cout << "ResolutionProportion: 16x9"<<endl;
   }
   else if(resProp == 133)
   {
      resolutionProportion = Res4x3;
      cout << "ResolutionProportion: 4x3"<<endl;
   }
   else
   {
      resolutionProportion  = Other;
      cout << "ResolutionProportion: Other"<<endl;
   }


   clearColor = _clearColor;
}

Manager ::  ~Manager()
{

}


string Manager ::  resolutionPropToStr()
{
   switch(resolutionProportion)
   {
      case Res16x10:
         return "16x10 - 1280x800\\";
      case Res16x9:
         return "16x9 - 1200x675\\";
      case Res4x3:
         return "4x3 - 800x600\\";
   }
   return "";
}

int Manager :: applicationLoop()
{
   initManager();
   glutMainLoop();
   return 42;
}

////////////
//Protected
///////////
void Manager :: openGlInit()
{
   glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.0 );

	glShadeModel(GL_SMOOTH);
//	glShadeModel(GL_FLAT);

	glPolygonMode(GL_FRONT, GL_FILL);
//   glPolygonMode(GL_BACK, GL_LINE);
//   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//   glCullFace(GL_BACK);
//   glEnable(GL_CULL_FACE);

   glEnable(GL_DEPTH_TEST);

}

void Manager :: onReshape(int width, int height)
{
   const float ar = (float) width / (float) height;

   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity() ;

   appWidth = width;
   appHeight = height;
}




void Manager :: onKeyPressed(unsigned char key,int x,int y)
{
//   printf("K:%d\n",key);
   switch(key)
   {
      case K_ESC:
         exit(42);
      break;
      case '+':
         app.r -= 5;
      break;
      case '-':
         app.r += 5;
      break;


   }

//   cout << "MODIFIERS "<<glutGetModifiers()<< endl;
}

void Manager :: onKeyReleased(unsigned char key,int x,int y)
{
//   printf("KR:%d\n",key);
//   switch(key)
//   {
//      case K_ESC:
//         exit(42);
//      break;
//   }
//   topScene->onKeyReleased(key,x,y);
}


void Manager :: onSpecialKeyPressed(int key,int x,int y)
{
//   printf("SP:%d\n",key);
   int modifiers = glutGetModifiers();
   switch(key)
   {
      case 4:
         if(modifiers==K_MOD_ALT)
            exit(42);
      break;

      case K_UP_ARROW:
         app.alpha = ((int)(app.alpha + app.angInc))%360;
      break;
      case K_DOWN_ARROW:
         app.alpha = ((int)(app.alpha - app.angInc))%360;
      break;
      case K_LEFT_ARROW:
         app.beta = ((int)(app.beta - app.angInc))%360;
      break;
      case K_RIGHT_ARROW:
         app.beta = ((int)(app.beta + app.angInc))%360;
      break;

      case K_PG_UP:
      break;
      case K_PG_DOWN:
      break;

      case K_PG_HOME:
      break;
      case K_PG_END:
      break;



      case 6:
      break;
      case 7:
      break;

      case 9:

      break;
      case 10:
         app.wireframeEnabled = !app.wireframeEnabled;
         if(app.wireframeEnabled)
         {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//            glDisable(GL_CULL_FACE);
         }else
         {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//            glEnable(GL_CULL_FACE);
         }
      break;
      case 11:
      break;
      case 12:
         app.shaderEnabled = !app.shaderEnabled;
      break;

   }
}

void Manager :: onSpecialKeyReleased(int key,int x,int y)
{
//   printf("SR:%d\n",key);
//   topScene->onSpecialKeyReleased(key,x,y);
}




void Manager :: onMouseMoved(int x, int y)
{
}

void Manager :: onMousePressed(int button, int state,int x, int y)
{

//   if(button == M_LEFT_BUTTON)
//   {
//      if(mousePressed && state !=M_PRESSED)
//      {
//      }
//      mousePressed = state==M_PRESSED;
//   }

}

void Manager :: onMouseEntry(int state)
{

}

RTScene * scene;

void Manager :: render()
{
   timeFromLastFrame = fps.getTimeFromLastFrame();

   const float cFovY = 40.0;
   const float cNear = 1.0;
   const float cFar = 2000.0;
   const float cAspect = appWidth/appHeight;

   const Vector3 pos = Vector3(0,0,0);
   const Vector3 at = Vector3(0,0,0);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(cFovY, cAspect, cNear, cFar);
   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity( );


   float x = app.r*sin(DEG_TO_RAD(app.beta))*cos(DEG_TO_RAD(app.alpha));
   float y = app.r*sin(DEG_TO_RAD(app.alpha));
   float z = app.r*cos(DEG_TO_RAD(app.beta))*cos(DEG_TO_RAD(app.alpha));


   float nextAlpha =  min(app.alpha + app.angInc,360.0f);

   float ux = sin(DEG_TO_RAD(app.beta))*cos(DEG_TO_RAD(nextAlpha)) - x;
   float uy = sin(DEG_TO_RAD(nextAlpha)) - y;
   float uz = cos(DEG_TO_RAD(app.beta))*cos(DEG_TO_RAD(nextAlpha)) - z;

   gluLookAt(  x, y, z,
               at.x, at.y, at.z,
               ux, uy, uz);

   scene->configure();
   scene->render();



/*
   app.fontRender.initText();

   char alphaInfo[40];
   sprintf(alphaInfo, "Contrast Factor: %.2f", app.alpha);
   app.fontRender.print(10,appHeight*.85+10, alphaInfo, Color(.8, .8, 0.));

   char difInfo[80];
   sprintf(difInfo, "Calulation Diference: %.4f", ~(app.avgLuminanceG - app.avgLuminanceC));
   app.fontRender.print(10,appHeight*.85+35, difInfo, Color(.8, .8, 0.));

   char difTimeInfo[80];
   sprintf(difTimeInfo, "Gpu Performance Gain: %.4f", app.cpuTime/app.gpuTime);
   app.fontRender.print(10,appHeight*.85+60, difTimeInfo, Color(.8, .8, 0.));

   if(app.shaderEnabled)
      app.fontRender.print(10,appHeight*.85+85,"(F12) AvgLuminance: GPU", Color(.8, .8, 0.));
   else app.fontRender.print(10,appHeight*.85+85,"(F12) AvgLuminance: CPU", Color(.8, .8, 0.));

   app.fontRender.endText();
   /**/
}

void Manager :: createScenes()
{
//   app.simpleContrastShader = new Shader("SimpleContrast","Shaders/simpleContrast");
//   app.textureLoc = app.simpleContrastShader->getUniformLoc("text");
//   app.imgAlphaLoc = app.simpleContrastShader->getUniformLoc("alpha");
//   app.imgAvgLuminanceLoc = app.simpleContrastShader->getUniformLoc("avgLuminance");

//   string imgName = "Textures/bigImage4096.jpg";
//   app.imgSDLSurface = IMG_Load(imgName.c_str());


//   clock_t t1 = clock();
//   clock_t t2 = clock();
//   double diffTime  = (double)(t2 - t1) / CLOCKS_PER_SEC;
//   app.cpuTime = diffTime/app.benchNumTimes;
//   app.textureID = TextureManager::newTexID(imgName);

//   app.frameBuffer = new FrameBufferObject(appWidth, appHeight);
//   app.textureID2 = app.frameBuffer->attachToColorBuffer(BufferType::Texture);

   string sceneFileName = "./resources/Scenes/cavalo.rt4";
   scene = new RTScene(sceneFileName);
}




////////////
//Private
//////////

void Manager :: initManager()
{
   //Init Screen
   glutInit(&appArgC, appArgV);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (appWidth, appHeight);
	glutInitWindowPosition (300, 20);

   //Init Window
	glutCreateWindow (appTitle.c_str());

   //SetFullScreen
   if(fullScreen)
      glutFullScreen();

   //Init GL
   openGlInit();

   //CalbackFunc Definition
   glutReshapeFunc(onReshape);

	glutKeyboardFunc(onKeyPressed);
	glutKeyboardUpFunc(onKeyReleased);

	glutSpecialFunc(onSpecialKeyPressed);
	glutSpecialUpFunc(onSpecialKeyReleased);

	glutPassiveMotionFunc(onMouseMoved);
	glutMotionFunc(onMouseMoved);
	glutMouseFunc(onMousePressed);
	glutEntryFunc(onMouseEntry);

	glutDisplayFunc(display);

	glutIdleFunc(display);


	createScenes();

   // disables cursor
//   glutSetCursor(GLUT_CURSOR_NONE);
}


void Manager :: display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      render();
   glutSwapBuffers();
}


/**********************************************************\
            Name:Eduardo Ceretta Dalla Favera
\**********************************************************/
/**********************************************************\

\**********************************************************/
#ifndef __MANAGER_H_
#define __MANAGER_H_

#include <vector>

#include <string>

using namespace std;

#include "GraphBasis/Color.h"
#include "GraphBasis/Frames.h"



class Manager
{
   private:
      int appArgC;
      char **appArgV;

      string appTitle;

      Color clearColor;

   protected:
      bool fullScreen;

   public:
      static int appWidth, appHeight;
      static Frames fps;
      static float timeFromLastFrame;

      typedef enum  {Res4x3, Res16x9,Res16x10,Other} ResolutionsProportions;
      static ResolutionsProportions resolutionProportion;

      Manager(int argc, char *argv[],int appWidth, int appHeight, string appTitle="Title",Color clearColor=Color(1,1,1),bool fullScreen=false);
      Manager(int argc, char *argv[],string appTitle = "Title",Color clearColor = Color(1,1,1));
      ~Manager();

      static string resolutionPropToStr();

      int applicationLoop();

   private:
      void initManager();
      static void display();

   protected:
      void openGlInit();

      static void onReshape(int width, int height);

      static void onKeyPressed(unsigned char key,int x,int y);
      static void onKeyReleased(unsigned char key,int x,int y);

      static void onSpecialKeyPressed(int key,int x,int y);
      static void onSpecialKeyReleased(int key,int x,int y);

      static void onMouseMoved(int x, int y);
      static void onMousePressed(int button, int state,int x, int y);
      static void onMouseEntry(int state);

      static void render();

      void createScenes();
};

#endif

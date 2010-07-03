#include "main.h"

#define ANGLE_STEP 1.0
#define APP_WIDTH 800
#define APP_HEIGHT 600
#define DEG_TO_RAD(a) ((float)(a)*PI/180.0)

int lastMousePosX;
int lastMousePosY;
int mouseState;
int mouseButton;
bool step;
float camAlpha;
float camBeta;
float camR;
float camInc;

float nearPlane;
float fov;
RTScene* rtScene;
KernelMng* kernelMng;

GLenum e;

int main(int argc, char *argv[]){
	
	init(argc, argv);
	glewInit();	

	glutMainLoop();

	
}



void renderAxis(){
	glBegin(GL_LINES);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glColor3f(0, 0, 1); glVertex3f(0, 0, 1000);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glColor3f(0, 1, 0); glVertex3f(0, 1000, 0);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glColor3f(1, 0, 0); glVertex3f(1000, 0, 0);
	glEnd();

}

void init(int argc, char *argv[]){
	  

	camAlpha = 270.0;
	camBeta = 0.0;
	camR = 250;
	camInc = 5;
	lastMousePosX = 0;
	lastMousePosY = 0;
	mouseState = GLUT_UP;
	mouseButton = GLUT_RIGHT_BUTTON;
	nearPlane = 0.1f;
	fov = 60.0f;
	step = false;


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(APP_WIDTH,APP_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ray tracing");
	
	glClearColor(0.0,0.0,0.0,0.0);
	//gluOrtho2D(0,800,0,600);
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMotionFunc(mouseActive);
	glutMouseFunc(mouseButtons);
	glutReshapeFunc(reshape);
	
	glEnable(GL_DOUBLEBUFFER);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_1D);

  //glPolygonMode(GL_FRONT, GL_LINE);


	rtScene = new RTScene("./resources/scenes/cavalo.rt4");
	rtScene->configure();
	float nearPlaneHeight = 2.0f * tanf(DEG_TO_RAD(fov/2.0f)) * nearPlane;
	float nearPlaneWidth = nearPlaneHeight * ((GLfloat)APP_WIDTH/(GLfloat)APP_HEIGHT);
	kernelMng = new KernelMng(APP_WIDTH, APP_HEIGHT, rtScene, nearPlaneWidth, nearPlaneHeight);

}


void reshape(int w, int h){
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(fov, (GLfloat)w / (GLfloat)h, nearPlane, 1000000.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboardSpecial(int key, int x, int y){
	/*	
	if( key == GLUT_KEY_LEFT ) angleY+=ANGLE_STEP;
	else if( key == GLUT_KEY_RIGHT ) angleY-=ANGLE_STEP;
	else if( key == GLUT_KEY_UP ) angleX+=ANGLE_STEP;
	else if( key == GLUT_KEY_DOWN ) angleX-=ANGLE_STEP;
	*/
	if( key == GLUT_KEY_HOME)
		step = true;
	
}

void keyboard(unsigned char key, int x, int y){
  switch(key)
  {
    case 27://ESC
      exit(42);
      break;
  }
  //cout << (int)key<<endl;
  if( key == 32)
		kernelMng->generateRay();
}

void mouseButtons(int button, int state, int x, int y){
	
	mouseState = state;
	mouseButton = button;

	lastMousePosX = x;
	lastMousePosY = y;
}

void mouseActive(int x, int y){
  if(mouseButton == GLUT_LEFT_BUTTON && mouseState == GLUT_DOWN){
    float angleX = (x - lastMousePosX)*.5;
    float angleY = (y - lastMousePosY)*.5;


    camAlpha = ((int)(camAlpha + angleY))%360;
    camBeta = ((int)(camBeta + angleX))%360;
  }
  else if(mouseButton == GLUT_RIGHT_BUTTON && mouseState == GLUT_DOWN){
    camR += (y - lastMousePosY)/2.0;
  }
  lastMousePosX = x;
  lastMousePosY = y;

}





void render(){

	//if(step == false) return;

	//std::cout << camBeta << ", " << camAlpha << ", " << camInc << "\n";

	glClearColor(1.0,1.0,1.0,1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	float x = camR*sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
	float y = camR*sin(DEG_TO_RAD(camAlpha));
	float z = camR*cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));


	float nextAlpha =  min(camAlpha + camInc,360.0f);

	float ux = sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - x;
	float uy = sin(DEG_TO_RAD(nextAlpha)) - y;
	float uz = cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - z;

	gluLookAt(x,y,z, 0, 0, 0, ux, uy, uz);

	//TODO: get the values from the current MODELVIEW matrix
	////GLfloat* lookAtMatrix;
	//glGetFloatv(GL_MODELVIEW_MATRIX, lookAtMatrix);
	Vector3 f = (Vector3(0,0,0) - Vector3(x,y,z)).unitary();
	Vector3 up = Vector3(ux, uy, uz).unitary();
	Vector3 s = f ^ up;
	Vector3 u = s ^ f;
	Vector3 r = f ^ u;
	renderAxis();
	rtScene->render();
  //cout << camR << endl;

	
	kernelMng->step(GENERATERAY,
	//kernelMng->step(TRAVERSE,
	//kernelMng->step(INTERSECT,
	//kernelMng->step(SHADE,
					Vector3(x, y, z),
						f,
						u,
						r,
						nearPlane);
	kernelMng->renderKernelOutput(GENERATERAY, 3);
	//kernelMng->renderKernelOutput(TRAVERSE, 3);
	//kernelMng->renderKernelOutput(INTERSECT, 2);
	//kernelMng->renderKernelOutput(SHADE, 2);
	
 
	glutSwapBuffers();

	step = false;

}
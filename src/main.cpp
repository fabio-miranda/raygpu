#include "main.h"

#define ANGLE_STEP 1.0
#define APP_WIDTH 1280
#define APP_HEIGHT 720

float angleX;
float angleY;
float angleZ;
int lastMousePosX;
int lastMousePosY;
int mouseState;
int mouseButton;
Vector3 eyePos;
Vector3 eyeDir;
Vector3 eyeUp;
float nearPlane;
RTScene* rtScene;
KernelMng* kernelMng;


int main(int argc, char *argv[]){
	
	init(argc, argv);
	glewInit();	

	glutMainLoop();

	
}



void render(){
	
	
	glClearColor(1.0,1.0,1.0,1.0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(eyePos.x,eyePos.y,eyePos.z, 0.0, 0.0, 0.0, eyeUp.x, eyeUp.y, eyeUp.z);

	glRotatef(angleX, 1.0, 0.0, 0.0); //rotate on the x axis
    glRotatef(angleY, 0.0, 1.0, 0.0); //rotate on the y axis
    glRotatef(angleZ, 0.0, 0.0, 1.0); //rotate on the z axis

	renderAxis();
	//rtScene->render();
	kernelMng->step(GENERATERAY, eyePos, eyeDir, eyeUp, eyeDir ^ eyeUp, nearPlane);
	kernelMng->renderKernelOutput(GENERATERAY, 0);

	
	
	glFlush();

	glutSwapBuffers();

}

void renderAxis(){
	glBegin(GL_LINES);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glColor3f(0, 0, 1); glVertex3f(0, 0, 1000);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glColor3f(0, 1, 0); glVertex3f(0, 1000, 0);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glColor3f(1, 0, 0); glVertex3f(1000, 0, 0);
	glEnd();

}

void init(int argc, char *argv[]){
	

	angleZ = 0;
	angleX = 0;
	angleY = 0;
	lastMousePosX = 0;
	lastMousePosY = 0;
	mouseState = GLUT_UP;
	mouseButton = GLUT_RIGHT_BUTTON;
	eyePos = Vector3(0, 0, 5);
	eyeUp = Vector3(0, 1, 0);
	nearPlane = 0.1f;


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



	rtScene = new RTScene("./resources/scenes/cavalo.rt4");
	rtScene->configure();

	kernelMng = new KernelMng(APP_WIDTH, APP_HEIGHT, rtScene);

}


void reshape(int w, int h){
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, nearPlane, 1000000.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboardSpecial(int key, int x, int y){
	
	if( key == GLUT_KEY_LEFT ) angleY+=ANGLE_STEP;
	else if( key == GLUT_KEY_RIGHT ) angleY-=ANGLE_STEP;
	else if( key == GLUT_KEY_UP ) angleX+=ANGLE_STEP;
	else if( key == GLUT_KEY_DOWN ) angleX-=ANGLE_STEP;
}

void keyboard(unsigned char key, int x, int y){
}

void mouseButtons(int button, int state, int x, int y){
	
	mouseState = state;
	mouseButton = button;

	lastMousePosX = x;
	lastMousePosY = y;
}

void mouseActive(int x, int y){
	if(mouseButton == GLUT_LEFT_BUTTON && mouseState == GLUT_DOWN){
		angleX += (y - lastMousePosY)/5.0f;
		angleY += (x - lastMousePosX)/5.0f;
	}
	else if(mouseButton == GLUT_RIGHT_BUTTON && mouseState == GLUT_DOWN){
		eyePos.z += (y - lastMousePosY)/50.0;
	}
	

	lastMousePosX = x;
	lastMousePosY = y;

}




#include "main.h"
#include "Config.h"


int main(int argc, char *argv[]){
	initIup(argc, argv);
	init(argc, argv);
	initGL(argc, argv);

	openFile(NULL);
	//createScenes(INIT_SCENE_FILE_NAME);
	

	IupShowXY(iupDialog, IUP_CENTER, IUP_CENTER);
	//createScenes(INIT_SCENE_FILE_NAME);

	IupMainLoop();
	IupClose();
	//glutMainLoop();

}



void renderAxis(){

	glBegin(GL_LINES);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glColor3f(0, 0, 1); glVertex3f(0, 0, 1000);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glColor3f(0, 1, 0); glVertex3f(0, 1000, 0);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glColor3f(1, 0, 0); glVertex3f(1000, 0, 0);
	glEnd();
}

void init(int argc, char *argv[]){
	camAlpha = INIT_CAM_ALPHA;
	camBeta = INIT_CAM_BETA;
	camR = INIT_CAM_R;
	camInc = INIT_CAM_INC;

	lastMousePosX = INIT_LAST_MOUSE_POS_X;
	lastMousePosY = INIT_LAST_MOUSE_POS_Y;
	mouseState = GLUT_UP;
	mouseButton = GLUT_RIGHT_BUTTON;

	nearPlane = INIT_RT_NEAR_PLANE;
	fov = INIT_RT_FOV;
	
	singleStep = INIT_RT_STEP;
	continuousStep = false;

	sceneFileName = INIT_SCENE_FILE_NAME;

	rObj = INIT_RENDER_OBJ;

	for(int i=0; i<5; i++) stateToggles[i] = false;
	
}

void initGL(int argc, char *argv[]){

	IupGLMakeCurrent(canvasRenderer);

	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitWindowSize(APP_WIDTH,APP_HEIGHT);
	//glutInitWindowPosition(100, 100);
	//glutCreateWindow("Ray tracing");
	
	
	glewInit();
	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(0,800,0,600);
	//glutDisplayFunc(render);
	//glutIdleFunc(render);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(keyboardSpecial);
	//glutMotionFunc(mouseActive);
	//glutMouseFunc(mouseButtons);
	//glutReshapeFunc(reshape);
	
	glEnable(GL_DOUBLEBUFFER);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);
	
	//glPolygonMode(GL_FRONT, GL_LINE);

	reshape(canvasRenderer, 800, 600);


}




void keyboardSpecial(int key, int x, int y){
	/*	
	if( key == GLUT_KEY_LEFT ) angleY+=ANGLE_STEP;
	else if( key == GLUT_KEY_RIGHT ) angleY-=ANGLE_STEP;
	else if( key == GLUT_KEY_UP ) angleX+=ANGLE_STEP;
	else if( key == GLUT_KEY_DOWN ) angleX-=ANGLE_STEP;
	*/
	if( key == GLUT_KEY_HOME)
		singleStep = true;
	
}

void keyboard(unsigned char key, int x, int y){
  switch(key)
  {
    case 27://ESC
      exit(42);
      break;
    case 'q':
    case 'Q':
      rObj = !rObj;
      break;
    case 'w':
    case 'W':
      rObj = GL_RT_OBJECT;
    break;
  }
  //cout << (int)key<<endl;
  if( key == 32)
		kernelMng->generateRay();

  if( key == 32)
    kernelMng->generateRay();
  else if(key == '1')
    kernelMng->setCurrentState(CALCULATEVOXEL);
  else if(key == '2')
	kernelMng->setCurrentState(TRAVERSE);
  else if(key == '3')
    kernelMng->setCurrentState(INTERSECT);
  else if(key == '4')
    kernelMng->setCurrentState(SHADE);
}

void mouseButtons(Ihandle* handle, int button, int state, int x, int y){
	
	mouseState = state;
	mouseButton = button;

	lastMousePosX = x;
	lastMousePosY = y;
}

void mouseActive(Ihandle* handle, int x, int y){
	if(mouseButton == IUP_BUTTON1 && mouseState == 1){
		float angleX = (x - lastMousePosX)*.5;
		float angleY = (y - lastMousePosY)*.5;


		camAlpha = ((int)(camAlpha + angleY))%360;
		camBeta = ((int)(camBeta - angleX))%360;

	}
	else if(mouseButton == IUP_BUTTON3 && mouseState == 1){
		camR += (y - lastMousePosY)/2.0;
	}

	if(RAYTRACE_WHILE_MOVING && (lastMousePosX != x || lastMousePosY != y)
		&& mouseState == 1){
		kernelMng->generateRay();
		kernelMng->setCurrentState(GENERATERAY);
	}

	lastMousePosX = x;
	lastMousePosY = y;

}


void openFile(Ihandle* handle){

	Ihandle* fileDialog = IupFileDlg(); 
	IupSetAttribute(fileDialog, "DIRECTORY", "./");
	IupPopup(fileDialog, IUP_CENTER, IUP_CENTER); 


	switch(IupGetInt(fileDialog, "STATUS"))
	{
		case 0 :{
			createScenes(IupGetAttribute(fileDialog, "VALUE"));
			break;
			}
			

		case -1 :{
			IupMessage("IupFileDlg","Operation Canceled");
			break; 
			}
	}
}


void createScenes(std::string sceneFileName) 
{
  

  int index = sceneFileName.find_last_of(".");
  assert(index != string::npos);
  string sub = sceneFileName.substr(index, string::npos);

  rtScene = new RTScene(sceneFileName, RT_GRID_SIZE);
  rtScene->configure();
  if(sub.compare(".rt4")==0 || sub.compare(".RT4")==0)
  {
    sub = sceneFileName.substr(0, index) + ".rtb";
    rtScene->writeRTBFile(sub);
    cout << "Gerado Arquivo "<< sub << " com cena"<<endl;
  }


  float nearPlaneHeight = 2.0f * tanf(DEG_TO_RAD(fov/2.0f)) * nearPlane;
  float nearPlaneWidth = nearPlaneHeight * ((GLfloat)APP_WIDTH/(GLfloat)APP_HEIGHT);
  kernelMng = new KernelMng(APP_WIDTH, APP_HEIGHT, rtScene, nearPlaneWidth, nearPlaneHeight);
}

void initIup(int argc, char *argv[]){
	
	IupOpen(&argc, &argv);
	IupGLCanvasOpen();
	//IupSetGlobal("InterfaceIUP", (char*)this);
	IupSetFunction(IUP_IDLE_ACTION, (Icallback) render);

	//Canvas renderer
	canvasRenderer = IupGLCanvas("renderCanvas");
	IupSetCallback(canvasRenderer, "RESIZE_CB",(Icallback) reshape);
	IupSetCallback(canvasRenderer, "MOTION_CB",(Icallback)mouseActive);
	IupSetCallback(canvasRenderer, "BUTTON_CB",(Icallback)mouseButtons);
	IupSetCallback(canvasRenderer, "KEYPRESS_CB",(Icallback)keyboard);
	IupSetAttribute(canvasRenderer,IUP_RASTERSIZE, APP_SIZE);
	IupSetAttribute(canvasRenderer,IUP_BUFFER,IUP_DOUBLE);
	IupSetAttribute(canvasRenderer,IUP_TITLE,"renderCanvas");
	IupSetHandle("renderCanvas", canvasRenderer);
	IupMap(canvasRenderer);

	Ihandle* rendererBox = IupVbox(canvasRenderer, NULL);



	//Toggles states
	Ihandle* toggleGenerateRay = IupToggle("Generate Ray", NULL);
	IupSetCallback(toggleGenerateRay, "ACTION", (Icallback) setGenerateRay);

	Ihandle* toggleCalculateVoxel = IupToggle("Calculate Voxel", NULL);
	IupSetCallback(toggleCalculateVoxel, "ACTION", (Icallback) setCalculateVoxel);

	Ihandle* toggleTraverse = IupToggle("Traverse", NULL);
	IupSetCallback(toggleTraverse, "ACTION", (Icallback) setTraverse);

	Ihandle* toggleIntersect = IupToggle("Intersect", NULL);
	IupSetCallback(toggleIntersect, "ACTION", (Icallback) setIntersect);

	Ihandle* toggleShade = IupToggle("Shade", NULL);
	IupSetCallback(toggleShade, "ACTION", (Icallback) setShade);

	Ihandle* buttonSingleStep = IupButton("Step", NULL);
	IupSetCallback(buttonSingleStep, "ACTION", (Icallback) singleStepCallback);

	Ihandle* toggleContinuousStep = IupToggle("Continuous step", NULL);
	IupSetCallback(toggleContinuousStep, "ACTION", (Icallback) setContinuousStep);


	Ihandle* vboxStates = IupVbox(toggleGenerateRay, toggleCalculateVoxel, toggleTraverse, toggleIntersect, toggleShade,
									buttonSingleStep, toggleContinuousStep, NULL);
	Ihandle* frameStates = IupFrame(vboxStates);
	IupSetAttribute(frameStates, "TITLE", "States");



	//Boxes
	Ihandle* vBoxLeft = IupVbox(rendererBox, NULL);
	Ihandle* vBoxRight = IupVbox(frameStates, NULL);
	IupSetAttribute(vBoxLeft,"GAP","1");
	IupSetAttribute(vBoxRight,"GAP","5");
	
	Ihandle* hBox = IupHbox(vBoxLeft, vBoxRight, NULL);
	IupSetAttribute(hBox,"GAP","5");


	//Menu
	Ihandle* itemOpen = IupItem("Open", NULL);
	IupSetCallback(itemOpen, "ACTION", (Icallback) openFile);
	Ihandle* itemExit = IupItem("Exit", NULL);
	IupSetCallback(itemExit, "ACTION",(Icallback) exit);
	Ihandle* fileMenu = IupMenu(itemOpen, IupSeparator(), itemExit, NULL);
	Ihandle* subMenu = IupSubmenu("File", fileMenu);
	
	Ihandle* menu = IupMenu(subMenu, NULL);
	IupSetHandle("menu", menu);

	iupDialog = IupDialog(hBox);
	IupSetAttribute(iupDialog, "TITLE", "Volume Renderer");
	IupSetAttribute(iupDialog, "COMPOSITED", "NO");
	IupSetAttribute(iupDialog, "MENU", "menu");
	//IupSetAttribute(m_dialog, "FULLSCREEN", "YES");

	IupMap(iupDialog);

	

}


int setGenerateRay(Ihandle* handle, int state){
	stateToggles[0] = state;

	return IUP_DEFAULT;
}

int setCalculateVoxel(Ihandle* handle, int state){
	stateToggles[1] = state;

	return IUP_DEFAULT;
}

int setTraverse(Ihandle* handle, int state){
	stateToggles[2] = state;

	return IUP_DEFAULT;
}

int setIntersect(Ihandle* handle, int state){
	stateToggles[3] = state;

	return IUP_DEFAULT;
}

int setShade(Ihandle* handle, int state){
	stateToggles[4] = state;

	return IUP_DEFAULT;
}

int singleStepCallback(Ihandle* handle){


	singleStep = true;

	return IUP_DEFAULT;
}

int setContinuousStep(Ihandle* handle, int state){

	continuousStep = state;
	return IUP_DEFAULT;

}

int reshape(Ihandle* handle, int w, int h){
	
	IupGLMakeCurrent(handle);

	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    //glLoadIdentity ();
    gluPerspective(fov, (GLfloat)w / (GLfloat)h, nearPlane, 1000000.0);
    glMatrixMode (GL_MODELVIEW);


	return IUP_DEFAULT;
}

void renderGL(Vector3 eye, Vector3 up){

	glLoadIdentity();
	gluLookAt(eye.x,eye.y,eye.z, 0, 0, 0, up.x, up.y, up.z);

	renderAxis();

	//if(rObj!=GL_OBJECT)
		rtScene->render();

}

int render(Ihandle* handle){
	
	IupGLMakeCurrent(canvasRenderer);
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float x = camR*sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
	float y = camR*sin(DEG_TO_RAD(camAlpha));
	float z = camR*cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
	float nextAlpha =  min(camAlpha + camInc,360.0f);
	float ux = sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - x;
	float uy = sin(DEG_TO_RAD(nextAlpha)) - y;
	float uz = cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - z;

	renderGL(Vector3(x, y, z), Vector3(ux, uy, uz));

	
	if(singleStep == true || continuousStep == true){
		
		//TODO: get the values from the current MODELVIEW matrix
		////GLfloat* lookAtMatrix;
		//glGetFloatv(GL_MODELVIEW_MATRIX, lookAtMatrix);
		Vector3 f = (Vector3(0,0,0) - Vector3(x,y,z)).unitary();
		Vector3 up = Vector3(ux, uy, uz).unitary();
		Vector3 s = f ^ up;
		Vector3 u = s ^ f;
		Vector3 r = f ^ u;

		
		
		if(rObj!=1)
		{
			if(stateToggles[0]){
				kernelMng->generateRay();
				kernelMng->stepState(GENERATERAY, Vector3(x, y, z), f, u, r, nearPlane);
			}
			if(stateToggles[1])
				kernelMng->stepState(CALCULATEVOXEL, Vector3(x, y, z), f, u, r, nearPlane);
			if(stateToggles[2])
				kernelMng->stepState(TRAVERSE, Vector3(x, y, z), f, u, r, nearPlane);
			if(stateToggles[3])
				kernelMng->stepState(INTERSECT, Vector3(x, y, z), f, u, r, nearPlane);
			if(stateToggles[4]){
				if(continuousStep)
					kernelMng->stepCurrentState(TRAVERSE_PER_INTERSECT, Vector3(x, y, z), f, u, r, nearPlane);
				else
					kernelMng->stepState(SHADE, Vector3(x, y, z), f, u, r, nearPlane);
			}
		}
		
		
	}

	if(rObj!=1)
	{
		if(stateToggles[0])
			kernelMng->renderKernelOutput(false, GENERATERAY, 3);
		else if(stateToggles[1])
			kernelMng->renderKernelOutput(false, CALCULATEVOXEL, 3);
		else if(stateToggles[2])
			kernelMng->renderKernelOutput(false, TRAVERSE, 3);
		else if(stateToggles[3])
			kernelMng->renderKernelOutput(false, INTERSECT, 3);
		else if(stateToggles[4])
			kernelMng->renderKernelOutput(false, SHADE, 3);	
	}
	

	IupGLSwapBuffers(canvasRenderer);
	//glutSwapBuffers();

	singleStep = false;
	

	return IUP_DEFAULT;
}
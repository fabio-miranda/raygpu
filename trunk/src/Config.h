#ifndef Config_H
#define Config_H

#define AUTO_STATE_CHANGE false
#define AUTO_STEP false

#define RAYTRACE_WHILE_MOVING true

#define TRAVERSE_PER_INTERSECT 4


#define APP_WIDTH 800
#define APP_HEIGHT 600
#define APP_SIZE "802x602"
#define ANGLE_STEP 1.0


#define GL_OBJECT 0
#define RT_OBJECT 1
#define GL_RT_OBJECT 2


#define INIT_CAM_ALPHA 0.0
#define INIT_CAM_BETA  0.0
#define INIT_CAM_R 150
#define INIT_CAM_INC 5
#define INIT_LAST_MOUSE_POS_X 0
#define INIT_LAST_MOUSE_POS_Y 0
#define INIT_RT_NEAR_PLANE 0.1f
#define INIT_RT_FOV  60.0f
#define INIT_RT_STEP false
#define INIT_SCENE_FILE_NAME "./resources/scenes/cavalo.rtb"
#define INIT_RENDER_OBJ GL_OBJECT

#define RT_GRID_SIZE Vector3(2,2,2)

#endif 
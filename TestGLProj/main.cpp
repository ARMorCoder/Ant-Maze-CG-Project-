#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//using namespace glm;
glm::vec4 move(0.0f, 0.0f, 15.0f, 1.0f);
glm::vec4 cammove(0.0f, 0.0f, 15.0f, 1.0f);
glm::vec3 center(-14.0f, 0.0f, 0.0f);
Shader shader; // loads our vertex and fragment shaders
Model* cylinder;
Model* plane; //a plane
Model* sphere;
Model* playerModel;
Model* wall;
Model* mazes;
Model* guns;
glm::mat4 projection; // projection matrix
glm::mat4 view;
glm::mat4 headTrans;
glm::mat4 camView;// where the camera is looking
glm::mat4 walls;
glm::mat4 gun;

glm::vec4 lookatdirection = glm::vec4(0, 0, -1, 0);
glm::vec4 camdirection = glm::vec4(0, 0, -1, 0);
glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
float angle = 0;
int camToggle;
const int maze_width = 50;
const int maze_height = 50;

const float wall_width = 0.2f;
const float wall_height = 1.0f;
float player_x = 0.0f;
float player_y = 0.0f;
float player_z = 0.0f;
float prev_player_x = 0.0f;
float prev_player_z = 0.0f;
/* report GL errors, if any, to stderr */
void checkError(const char* functionName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}
char maze[maze_height][maze_width] = {
	{'t', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
	{'t', 'f', 't', 'f', 'f', 'f', 't', 'f', 'f', 'f' , 'f', 'f', 't', 'f', 'f', 'f', 't', 'f', 'f', 't','t', 'f', 't', 'f', 'f', 'f', 't', 'f', 'f', 't'},
	{'t', 'x', 't', 'f', 't', 'f','f', 'g',  'f', 't', 'f','t', 'f', 'x','t', 'f', 'f', 'f', 't', 'f', 'f', 't','t', 'f', 'x','t', 'f', 'f', 'f', 't', 'f', 'f', 't'},
	{'t', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'x','g', 'f','f', 't', 'f', 'f', 'x','f', 't', 'f', 'f', 'x','t', 'f', 't', 'f', 'f', 'f', 't', 'f', 'f', 't'},
	{'t', 't', 't', 'f', 't', 'f', 't', 'f', 't', 't','f', 't', 'f', 'f', 'f', 't', 'g', 'f', 't','t', 'f', 't', 'x', 'f', 'f', 't', 'f', 'f', 't'},
	{'t', 'f', 't', 'f', 't', 'f', 'x', 'f', 'f', 'x','g', 't','f', 't', 'f', 'f', 'f', 'x','t', 'g', 'f', 't','t', 'f', 't', 'f', 'f', 'f', 't', 'f', 'f', 't'},
	{'t', 'f', 't', 'f', 't', 't', 't', 't', 't', 't','f', 't', 'f', 'g', 'f', 't', 'f', 'f', 't','t', 'f', 'g', 'f', 'f', 'g', 't', 'f', 'f', 't'},
	{'t', 'g', 'x', 'f', 'f', 'x', 'f', 'f', 'f', 't', 't','g', 't', 'x','f', 'f', 'f', 'x', 'f', 'f', 't','t', 'f', 'x','t', 'f', 'f', 'x','f', 't', 'f', 'f', 't'},
	{'t', 'f', 't', 't', 't', 't', 't', 't', 'f', 'f','f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'x','t','t', 'f', 't', 'f', 'f', 'f', 't', 'f', 'f', 't'},
	{'t', 't', 't', 't', 't', 't', 't', 't', 't', 't','t', 't', 't', 't', 't', 't', 'f', 't', 't','t', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
};


void initShader(void)
{
	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");

	checkError("initShader");
}

void initRendering(void)
{
	glClearColor(0.5f, 0.35f, 0.05f, 0.0f); // Dodger Blue
	checkError("initRendering");
}

void init(void)
{
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 1000.0f);


	// Load identity matrix into model matrix (no initial translation or rotation)
	initShader();
	initRendering();
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	checkError("dumpInfo");
}

float rotation = 0.0f;
float camrotation = 0.0f;
float verticalrotation = 0.0f;
//glm::vec4 cMove(0.0f, 0.0f, 15.0f, 1.0f);
float vRotation = 0.0f;
float cRotation = 0.0f;
float cameradistance = 10;
/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void draw_wall(float x, float y, float z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(wall_width, wall_height, wall_width);
	glutSolidCube(1.0f);
	glPopMatrix();
}
float delta = 0;
void display(void)
{
	delta += .4;
	//glm::rot
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (camToggle % 2 == 0) {
		//view = glm::lookAt(glm::vec3(cMove - (glm::rotate(cRotation, 0.f, 1.f, 0.f) * glm::rotate(vRotation, 1.0f, 0.0f, 0.0f) * lookatdirection * cameradistance) + glm::vec4(0.0f, 0.0f, 0.f, 0.f)), glm::vec3(cMove), glm::vec3(up));
		view = glm::lookAt(glm::vec3(move - (glm::rotate(cRotation, 0.f, 1.f, 0.f) * lookatdirection * cameradistance) + glm::vec4(0.0f, 5.0f, 0.f, 0.f)), glm::vec3(move), glm::vec3(up));
	}
	else {
		view = glm::lookAt(glm::vec3(move - (glm::rotate(rotation, 0.f, 1.f, 0.f) * lookatdirection) + glm::vec4(0.0f, 0.0f, 0.f, 0.f)), glm::vec3(move), glm::vec3(up));
	}
	headTrans = glm::translate(glm::vec3(move)) * glm::rotate(rotation, 0.f, 1.f, 0.f) * glm::translate(-4.0f, -4.0f, 0.0f);
	playerModel->render(view * headTrans/* glm::translate(-2.0f, -2.0f, -2.0f) /*glm::scale(5.0f, 5.0f, 5.0f)*/, projection);
	plane->render(view * glm::translate(0.0f, -5.0f, 0.0f) * glm::scale(400.0f, 1.0f, 400.0f), projection);
	guns->render(view * headTrans * glm::translate(3.0f, -2.0f, -2.0f) /*glm::scale(5.0f, 5.0f, 5.0f)*/, projection);
	for (int row = 0; row < maze_height; row++) {
		for (int col = 0; col < maze_width; col++) {

			if (maze[row][col] == 't') {
				float x1 = row;
				float y1 = 0.0f;
				float z1 = col;
				float x2 = row + 1;
				float y2 = 0.0f;
				float z2 = col;
				float x3 = row + 1;
				float y3 = 0.0f;
				float z3 = col + 1;
				float x4 = row;
				float y4 = 0.0f;
				float z4 = col + 1;
				if (player_x > x1 && player_x < x2 &&
					player_z > z1 && player_z < z4) {
					// There is a collision, stop the player's movement
					player_x = prev_player_x;
					player_z = prev_player_z;
					prev_player_x = player_x;
					prev_player_z = player_z;
					playerModel->render(view * headTrans * glm::translate(player_x, player_y, player_z)/* glm::translate(-2.0f, -2.0f, -2.0f) /*glm::scale(5.0f, 5.0f, 5.0f)*/, projection);

				}
				glPushMatrix();
				glTranslatef(maze_width, 0, maze_height);
				cylinder->render(view * glm::translate(10.0f, 0.0f, 15.0f) * glm::translate(-(float)col * 10, 0.0f, -(float)row * 10) * glm::scale(5.0f, 200.0f, 5.0f), projection);

				glPopMatrix();
			}

			if (maze[row][col] == 'x') {
				glPushMatrix();
				glTranslatef(maze_width, 0, maze_height);
				playerModel->render(view * glm::translate(13.0f, 0.0f, 15.0f) * glm::translate(-(float)col * 10, -5.0f, -(float)row * 10) * glm::rotate(180.0f, 0.0f, 1.0f, 0.0f), projection);
				glPopMatrix();
			}
			if (maze[row][col] == 'g') {
				glPushMatrix();
				glTranslatef(maze_width, 0, maze_height);
				guns->render(view * glm::translate(10.0f, 2.0f, 15.0f) * glm::translate(-(float)col * 10, -5.0f, -(float)row * 10) * glm::rotate(delta, 0.0f, 1.0f, 0.0f), projection);
				glPopMatrix();
			}


		}

	}
	glutSwapBuffers(); // Swap the buffers.
	checkError("display");
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	glutPostRedisplay(); // create a display event. Display calls as fast as CPU will allow when put in the idle function
}

/*Called when the window is resized*/
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	checkError("reshape");
}

/*Called when a normal key is pressed*/
glm::vec4 moveatdirection = glm::vec4(0, 0, -1, 0);
void keyboard(unsigned char key, int x, int y)
{
	//testing camera
	glm::vec4 moveatdir = glm::rotate(rotation, 0.f, 1.f, 0.f) * moveatdirection;
	glm::vec4 x1 = glm::vec4(glm::normalize(glm::cross(glm::vec3(up), glm::vec3(moveatdir))), 0.f);
	glm::vec4 lookatdir = glm::rotate(cRotation, 0.f, 1.f, 0.f) * lookatdirection;
	glm::vec4 cx1 = glm::vec4(glm::normalize(glm::cross(glm::vec3(center), glm::vec3(lookatdir))), 0.0f);
	switch (key) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case 'w':
		move += moveatdir;
		angle += .1;
		break;
	case 's':
		move -= moveatdir;
		angle -= .1;
		break;
	case 'a':
		// turn left
		rotation += 2;
		break;

	case 'd':
		// turn right
		rotation -= 2;
		break;

	case 'c':
		camToggle++;
		break;
	/*case 'f':
	//	cMove += lookatdir;
	//	break;
	//case 'v':
	//	cMove -= lookatdir;
	//	break;*/
		
	}
}

int camUp = 14;
int camDown = 14;

int cameraCheck = 0;
void specialKeyBoard(int Key, int x, int y) {
	glm::vec4 lookatdir = glm::rotate(cRotation, 0.f, 1.f, 0.f) * lookatdirection;
	glm::vec4 cx1 = glm::vec4(glm::normalize(glm::cross(glm::vec3(center), glm::vec3(lookatdir))), 0.0f);
	glm::vec3 up(0, 1, 0);
	if (cameraCheck % 2 == 1) {
		printf("WARNING! Camera is in 1st person!");
	}
	else {
		switch (Key) {
		/*case GLUT_KEY_UP:
		//	if (camUp < 45) {
		//		//cMove += lookatdir + center;
		//		//center += up;
		//		vRotation += 1.f;
		//		camUp++;
		//		camDown--;
		//	}
		//	else {
		//		printf("can't go up more\n");
		//	}
		//	break;
		//case GLUT_KEY_DOWN:
		//	if (camDown < 45) {
		//		//cMove -= lookatdir + center;
		//		//center -= up;
		//		vRotation -= 1.f;
		//		camUp--;
		//		camDown++;
		//	}
		//	else {
		//		printf("can't go down more\n");
		//	}
		//	break;*/
		case GLUT_KEY_LEFT:
			//cMove += cx1;
			//center += cx1;
			cRotation += 1.f;
			//center = center + cx1 * .5f;
			break;
		case GLUT_KEY_RIGHT:
			//cMove -= cx1;
			//center -= cx1;
			cRotation -= 1.f;
			//center = center - cx1 *-.5f;
			break;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glewInit();
	dumpInfo();
	init();


	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyBoard);
	glEnable(GL_DEPTH_TEST);


	cylinder = new Model(&shader, "models/cylinder.obj");
	plane = new Model(&shader, "models/plane.obj");
	sphere = new Model(&shader, "models/dodge-challenger_model.obj", "models/"); // you must specify the material path for this to load
	playerModel = new Model(&shader, "models/player_model.obj", "models/");
	wall = new Model(&shader, "models/wall.obj", "models/");
	mazes = new Model(&shader, "models/AntFarm.obj", "models/");
	guns = new Model(&shader, "models/rile.obj", "models/");
	glutMainLoop();

	return 0;
}
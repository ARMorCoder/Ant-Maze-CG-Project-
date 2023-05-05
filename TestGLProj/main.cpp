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
glm::vec3 center(0.0f, 0.0f, 0.0f);
Shader shader; // loads our vertex and fragment shaders
Model* shelf;
Model* tv;
Model* smalltable;
Model* couch;
Model *cylinder;
Model *theHead;
Model *theBody;
Model *theLeftArm;
Model *theRightArm;
Model *theLeftLeg;
Model *theRightLeg;//a cylinder 
Model *plane; //a plane
Model *sphere;
Model *room;
Model* playerModel;
Model *lamp;//a sphere
glm::mat4 projection; // projection matrix
glm::mat4 view;
glm::mat4 headTrans;
glm::mat4 camView;// where the camera is looking
glm::mat4 rightArmR; // where the model (i.e., the myModel) is located wrt the camera
glm::mat4 leftArmR;
glm::mat4 rightLegR;
glm::mat4 leftLegR;
glm::mat4 head;
glm::mat4 body;
glm::mat4 lArm;
glm::mat4 rArm;
glm::mat4 lLeg;
glm::mat4 rLeg;
glm::vec4 lookatdirection = glm::vec4(0, 0, -1, 0);
glm::vec4 camdirection = glm::vec4(0, 0, -1, 0);
glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
float fpscameradistance = 0.9;
float fpscameraheight = 0.0;
float freedistance = 10;
float freeheight = 0;
float angle = 0;
int camToggle;

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void)
{
	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");

	checkError ("initShader");
}

void initRendering(void)
{
	glClearColor (0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError ("initRendering");
}

void init(void) 
{	
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f/600.0f, 1.0f, 1000.0f);

	
	// Load identity matrix into model matrix (no initial translation or rotation)
	

	initShader ();
	initRendering ();
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

float rotation = 0.0f;
float camrotation = 0.0f;
float verticalrotation = 0.0f;
glm::vec4 cMove(0.0f, 0.0f, 15.0f, 1.0f);
float vRotation = 0.0f;
float cRotation = 0.0f;
float cameradistance = 10;
/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void display(void)
{

	//glm::rot
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (camToggle % 2 == 0) {
		view = glm::lookAt(glm::vec3(cMove - (glm::rotate(cRotation, 0.f, 1.f, 0.f) * glm::rotate(vRotation, 1.0f, 0.0f, 0.0f) * lookatdirection * cameradistance) + glm::vec4(0.0f, 0.0f, 0.f, 0.f)), glm::vec3(cMove), glm::vec3(up));
	}
	else {
		view = glm::lookAt(glm::vec3(move - (glm::rotate(rotation, 0.f, 1.f, 0.f) * lookatdirection) + glm::vec4(0.0f, 0.0f, 0.f, 0.f)), glm::vec3(move), glm::vec3(up));
	}
	headTrans = glm::translate(glm::vec3(move)) * glm::rotate(rotation, 0.f, 1.f, 0.f) * glm::translate(0.0f, -4.0f, 0.0f);
	playerModel->render(view * headTrans/* glm::translate(-2.0f, -2.0f, -2.0f) /*glm::scale(5.0f, 5.0f, 5.0f)*/, projection);
	// sphere is a child of the cylinder
	sphere->render(view * glm::translate(10.0f, -5.9f, 0.0f) * glm::scale(5.0f, 5.0f, 5.0f), projection);
	lamp->render(view * glm::translate(-10.0f, -5.0f, -10.0f) * glm::scale(5.0f, 5.0f, 5.0f), projection);
	shelf->render(view * glm::translate(15.0f, -9.0f, 0.0f) * glm::scale(5.0f, 5.0f, 5.0f), projection);
	smalltable->render(view * glm::translate(-15.0f, -10.0f, 0.0f) * glm::scale(5.0f, 5.0f, 5.0f), projection);
	couch->render(view * glm::translate(-15.0f, -9.0f, -8.0f) * glm::scale(5.0f, 5.0f, 5.0f), projection);
	tv->render(view * glm::translate(-15.0f, -6.0f, 0.0f) * glm::scale(5.0f, 5.0f, 5.0f), projection);
	plane->render(view * glm::translate(0.0f, -5.0f, 0.0f) * glm::scale(400.0f, 1.0f, 400.0f), projection);
	glutSwapBuffers(); // Swap the buffers.
	checkError("display");
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	glutPostRedisplay(); // create a display event. Display calls as fast as CPU will allow when put in the idle function
}

/*Called when the window is resized*/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	checkError ("reshape");
}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{
	glm::vec4 lookatdir = glm::rotate(rotation, 0.f, 1.f, 0.f) * lookatdirection;
	glm::vec4 camlookatdir = glm::rotate(camrotation, 0.f, 1.f, 0.f) * lookatdirection;

	switch (key) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case 'w':
		// move forward
		move += lookatdir;
		angle += .2;
		break;
	case 's':
		// move back
		move -= lookatdir;
		angle -= .2;
		break;
	case 'a':
		// turn left
		rotation += 1.f;
		break;
	case 'd':
		// turn right
		rotation -= 1.f;
		break;
	case 'f':
		//cam forward
		cammove += camlookatdir;
		break;
	case 'v':
		//cam back
		cammove -= camlookatdir;
		break;
	case 'c':
		//toggle camera mode
		camToggle++;
		break;
	}
}

void camControls(int camKey, int x, int y) {
	
	glm::vec4 lookatdir = glm::rotate(camrotation, 0.f, 1.f, 0.f) * lookatdirection;
	glm::vec4 vertlookatdir = glm::rotate(verticalrotation, 1.f, 0.f, 0.f) * lookatdirection;
	
	switch (camKey) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case GLUT_KEY_UP:
		// look up
		verticalrotation += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		// look down
		verticalrotation -= 0.1f;
		break;
	case GLUT_KEY_LEFT:
		// look left
		camrotation += 1.f;
		break;
	case GLUT_KEY_RIGHT:
		// look right
		camrotation -= 1.f;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	glewInit();
	dumpInfo ();
	init ();
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(camControls);
	glEnable(GL_DEPTH_TEST);

	
	cylinder = new Model(&shader, "models/cylinder.obj");
	theHead = new Model(&shader, "models/cylinder.obj");
	theBody = new Model(&shader, "models/cylinder.obj");
	theRightArm = new Model(&shader, "models/cylinder.obj");
	theLeftArm = new Model(&shader, "models/cylinder.obj");
	theRightLeg = new Model(&shader, "models/cylinder.obj");
	theLeftLeg = new Model(&shader, "models/cylinder.obj");
	plane = new Model(&shader, "models/plane.obj");
	sphere = new Model(&shader, "models/dodge-challenger_model.obj", "models/"); // you must specify the material path for this to load
	playerModel = new Model(&shader, "models/player_model.obj", "models/");
	lamp = new Model(&shader, "models/lamp.obj", "models/");
	shelf = new Model(&shader, "models/shelf.obj", "models/");
	smalltable = new Model(&shader, "models/table.obj", "models/");
	tv = new Model(&shader, "models/tv.obj", "models/");
	couch = new Model(&shader, "models/couch.obj", "models/");


	glutMainLoop();

	return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/freeglut.h>
#include<time.h>
#include<pthread.h>
#include<Windows.h>

#define SCRN_WID 1000
#define SCRN_HGT 1000
#define POINT_SIZE 5
#define MAX_POINTS 1000000
#define ITERATION 10

void window_display();
void init_window();
void init_function();
void Sierpinski_init();
void Sierpinski();
void* generate_points(void* arg);//worker thread
void idle();//idle function.


//shared data
float points[MAX_POINTS][6];
int pointCount = 0;
pthread_mutex_t lock;

//this is simple tetrahedron, not equilateral.
/*
float vertices[4][3] = {
	{1,1,1}, { -1,-1,-1 }, { -1, 1, -1}, {1, -1, -1}
};
*/

float L = 2.0f;
float h = sqrt(2.0f / 3.0f) * L; // height from base to top

float vertices[4][3] = {
	{0.0f, 0.0f, h},               // top vertex
	{-L / 2, -L / (2 * sqrt(3)), 0.0f},  // base vertex 1
	{L / 2, -L / (2 * sqrt(3)), 0.0f},   // base vertex 2
	{0.0f, L / sqrt(3), 0.0f}        // base vertex 3
};

float colors[4][3] = {
	{1,0,0}, {0,1,0}, {0,0,1}, {1,1,0}
};


//initial point for chaos generation.
float x = 0.5;
float y = 0.6;
float z = 0.3;

//initial color for chaos generation.
float r = 1, g = 1, b = 1;

//angle for rotation.
float anglex = 0.0;
float angley = 0.0;
float anglez = 0.0;

int main(int argc, char** argv)
{
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);

	pthread_mutex_init(&lock, NULL);
	pthread_t worker;
	pthread_create(&worker, NULL, generate_points, NULL);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCRN_WID, SCRN_HGT);
	glutCreateWindow("Sierpinski Colored Tetrahedron");

	init_function();
	init_window();
	glutDisplayFunc(window_display);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

void* generate_points(void* arg)
{
	while (1) {
		pthread_mutex_lock(&lock);

		for (int i = 0; i < ITERATION && pointCount < MAX_POINTS; i++)
		{
			int idx = rand() % 4;
			x = (x + vertices[idx][0]) / 2;
			y = (y + vertices[idx][1]) / 2;
			z = (z + vertices[idx][2]) / 2;

			r = (r + colors[idx][0]) / 2;
			g = (g + colors[idx][1]) / 2;
			b = (b + colors[idx][2]) / 2;
			
			points[pointCount][0] = x;
			points[pointCount][1] = y;
			points[pointCount][2] = z;
			points[pointCount][3] = r;
			points[pointCount][4] = g;
			points[pointCount][5] = b;

			pointCount++;
		}
		pthread_mutex_unlock(&lock);

		Sleep(16);
	}
	return NULL;
}

void Sierpinski()
{
	pthread_mutex_lock(&lock);
	glBegin(GL_POINTS);
	for (int i = 0; i < pointCount; i++) {
		glColor3f(points[i][3], points[i][4], points[i][5]);
		glVertex3f(points[i][0], points[i][1], points[i][2]);
	}
	glEnd();
	pthread_mutex_unlock(&lock);
}

void Sierpinski_init() 
{
	
	for (int i = 0; i < 4; i++)
	{
		glColor3f(colors[i][0], colors[i][1], colors[i][2]);
		glPointSize(POINT_SIZE);
		glBegin(GL_POINTS);
		glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
		glEnd();
	}
}

void window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();//save current matrix.
	glRotatef(anglex, 0.0, 1.0, 0.0);
	glRotatef(angley, 1.0, 0.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);

	Sierpinski_init();
	Sierpinski();

	glPopMatrix();//restore original matrix.
	glFlush();
}

void init_window()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)SCRN_WID / (double)SCRN_HGT, 1.0, 20.0);
	//fov = 60 degrees, aspect ratio = screen ratio, near = 1, far = 20.
	
	glViewport(0, 0, SCRN_WID, SCRN_HGT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0,//eye position
		      0.0, 0.0, 0.0,//look at origin
		      0.0, 1.0, 0.0);//up vector

	glClearColor(0, 0, 0, 1.0);
	glEnable(GL_DEPTH_TEST);//allow 3d depth handling.
	glFlush();
}

void init_function()
{
	glReadBuffer(GL_FRONT);
	glDrawBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void idle()
{
	anglex += 0.3;
	angley += 0.2;
	anglez += 0.4;
	if (anglex > 360.0) anglex -= 360.0;
	if (angley > 360.0) angley -= 360.0;
	if (anglez > 360.0) anglez -= 360.0;

	glutPostRedisplay();
}
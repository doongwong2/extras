#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/freeglut.h>
#include<time.h>
#include<pthread.h>
#include<Windows.h>

#define SCRN_WID 1000
#define SCRN_HGT 1000
#define POINT_SIZE 1
#define MAX_POINTS 1000000
#define ITERATION 10

void window_display();
void init_window();
void init_function();
void Sierpinski_init();
void Sierpinski();
void* generate_points(void* arg);//worker thread

//shared data
float points[MAX_POINTS][2];
int pointCount = 0;
pthread_mutex_t lock;

int vertices[2][3] = { 426,294 ,560 ,
						181,457 ,459 };
int x = 200; //initial point for chaos generation.
int y = 200;

int main(int argc, char **argv)
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
	glutCreateWindow("Sierpinki's Triangle Chaos");

	init_function();
	init_window();
	glutDisplayFunc(window_display);//init viewport and window before drawing.
	glutIdleFunc(glutPostRedisplay);

	glutMainLoop();

	return 0;
}

void init_function()
{
	glReadBuffer(GL_FRONT);
	glDrawBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void init_window()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)SCRN_WID, 0.0, (double)SCRN_HGT);
	glViewport(0, 0, SCRN_WID, SCRN_HGT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void window_display()
{
	Sierpinski_init();
	Sierpinski();
	glFlush();
}

void Sierpinski_init()
{
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < 3; i++)
	{
		glPointSize(POINT_SIZE);
		glBegin(GL_POINTS);
		glVertex2f(vertices[0][i], SCRN_HGT - vertices[1][i]);
		glEnd();
		fprintf(stderr,"point drawn!\n");
	}

}

void Sierpinski()
{

	glColor3f(1.0, 1.0, 0.0);
	glPointSize(1.0);

	pthread_mutex_lock(&lock);
	glBegin(GL_POINTS);
	for (int i = 0; i < pointCount; i++) {
		glVertex2f(points[i][0], SCRN_HGT - points[i][1]);
	}
	glEnd();
	pthread_mutex_unlock(&lock);

}

void* generate_points(void* arg) {
	while (1) {
		pthread_mutex_lock(&lock);

		for (int i = 0; i < ITERATION && pointCount < MAX_POINTS; i++)
		{
			int idx = rand() % 3;
			x = (x + vertices[0][idx]) / 2;
			y = (y + vertices[1][idx]) / 2;

			points[pointCount][0] = x;
			points[pointCount][1] = y;
			pointCount++;
		}
		pthread_mutex_unlock(&lock);

		Sleep(16);//adjust iteration calculation rate by stopping this for longer.
	}
	return NULL;
}

#include<stdio.h>
#include<GL/freeglut.h>
#include<windows.h>
#include<math.h>

HWND hwnd;//handle to glut window.
int winWidth = 400, winHeight = 400;
float angle = 0.0f;


void getWindowHandle();//get HWND of the glut window.
void display();
void timer(int value);

//player position
float px = 0.0f, py = 0.0f;
float speed = 0.1f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Moving Window");

	getWindowHandle();

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();

	return 0;
}

void getWindowHandle() 
{
		HDC hdc = wglGetCurrentDC();
		hwnd = WindowFromDC(hdc);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void timer(int value)
{
	if (hwnd)
	{
		int x = 300 + (int)(200 * cos(angle));
		int y = 300 + (int)(200 * sin(angle));
		MoveWindow(hwnd, x, y, winWidth, winHeight, TRUE);

		angle += 0.05f;
	}

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

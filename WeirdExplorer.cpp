#include<stdio.h>
#include<GL/freeglut.h>
#include<windows.h>
#include<math.h>
#define PI 3.14159265f
#define WORLD_WID 1920
#define WORLD_HGT 1080

HWND hwnd;//handle to glut window.

void getWindowHandle();//get HWND of the glut window.
void display();
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

float px = 500.0f, py = 500.0f;	//player position
float speed = 5.0f;			//pixels per frame
int winWidth = 400, winHeight = 400;
float angle = 0.0f;
float dir = 0;				//in radians.
float rotSpeed = 0.1;		//0.1 radians per action.

//controls.
bool W = false, A = false, S = false, D = false;

//treasure.
struct Treasure { float x, y; bool found; };
Treasure treasures[5] = {
	{600, 500, false},
	{1500, 800, false},
	{1200, 1000, false},
	{300, 1000, false},
	{1800, 900, false},
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Moving Window");

	getWindowHandle();

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
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
	glPushMatrix();
	glTranslatef(winWidth / 2.0f -px, winHeight / 2.0f -py, 0.0f); //center position.

	for (int i = 0; i < 5; i++)
	{
		float dx = treasures[i].x;
		float dy = treasures[i].y;

		if (fabs(dx -px) <= winWidth / 2 && fabs(dy - py) <= winHeight / 2)
		{
			glColor3f(1, 1, 0);
			glBegin(GL_QUADS);
			glVertex2f(dx - 5, dy - 5);
			glVertex2f(dx + 5, dy - 5);
			glVertex2f(dx + 5, dy + 5);
			glVertex2f(dx - 5, dy + 5);
			glEnd();
		}
	}

	glPopMatrix();
	glPushMatrix();
	glTranslatef(winWidth / 2.0f, winHeight / 2.0f, 0.0f); //center position.
	glRotatef(dir * 180.0f / PI, 0, 0, 1);
	glColor3f(1, 0, 0);
	
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 20);
	glVertex2f(-10, -10);
	glVertex2f(10, -10);
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void timer(int value)
{
	float moveDir = dir + PI / 2;
	if (W) { px += cos(moveDir) * speed; py -= sin(moveDir) * speed; }
	if (S) { px -= cos(moveDir) * speed; py += sin(moveDir) * speed; }
	if (A) { dir += rotSpeed; }
	if (D) { dir -= rotSpeed; }

	if (px < 0) px = 0;
	if (px > WORLD_WID) px = WORLD_WID;
	if (py < 0) py = 0;
	if (py > WORLD_HGT) py = WORLD_HGT;

	if (hwnd)
	{
		MoveWindow(hwnd, (int)px,(int)py, winWidth, winHeight, TRUE);
	}

	glutPostRedisplay();		//refresh drawing.
	glutTimerFunc(16, timer, 0);//schedule next frame(about 60fps)
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': W = true; break;
	case 's': S = true; break;
	case 'a': A = true; break;
	case 'd': D = true; break;
	case 27: exit(0);//esc key.
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': W = false; break;
	case 's': S = false; break;
	case 'a': A = false; break;
	case 'd': D = false; break;
	}
}
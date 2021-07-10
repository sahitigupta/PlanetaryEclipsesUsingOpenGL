#include<GL/glut.h>
#include<stdlib.h>

void home();
void drawString(void*, float, float, char*);
void mainDisplay(void);
void mainReshape(int, int);
void mainKeyboard(unsigned char, int, int);
void eclipse();
void subDisplay(void);
void subReshape(int, int);
void lunarEclipse();
void solarEclipse();
void rightClickMenu(int);

GLuint main_window, sub_window;
int l = 0, s = 0, sub_menu, e_flag = 1;
float angle_mn = 0.0, angle_ec = 0.0;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 680);
	main_window = glutCreateWindow("Planetary Eclipse");
	home();
	glutMainLoop();
	return 0;
}

void home()
{
	glutSetWindow(main_window);
	glutDisplayFunc(mainDisplay);
	glutReshapeFunc(mainReshape);
	glutKeyboardFunc(mainKeyboard);
}

void mainDisplay(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawString(GLUT_BITMAP_TIMES_ROMAN_24, 400.0, 600.0, "PLANETARY ECLIPSES");
	drawString(GLUT_BITMAP_HELVETICA_18, 400.0, 300.0, "Press L/l key for Lunar Eclipse");
	drawString(GLUT_BITMAP_HELVETICA_18, 400.0, 200.0, "Press S/s key for Solar Eclipse");
	drawString(GLUT_BITMAP_HELVETICA_18, 440.0, 100.0, "Press E/e key for Exit");
	glutSwapBuffers();
}

void drawString(void* font, float x, float y, char* str)
{
	char* ch;
	glRasterPos3f(x, y, 0.0);
	for (ch = str; *ch; ch++)
		glutBitmapCharacter(font, (int)*ch);
}

void mainReshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 680.0);
	glMatrixMode(GL_MODELVIEW);
}

void mainKeyboard(unsigned char key, int x, int y)
{
	if (key == 'L' || key == 'l')
	{
		l = 1;
		sub_window = glutCreateSubWindow(main_window, 0, 0, 1000, 680);
		eclipse();
	}
	else if (key == 'S' || key == 's')
	{
		s = 1;
		sub_window = glutCreateSubWindow(main_window, 0, 0, 1000, 680);
		eclipse();
	}
	else if (key == 'E' || key == 'e')
	{
		exit(0);
	}
}

void eclipse()
{
	glutDisplayFunc(subDisplay);
	glutReshapeFunc(subReshape);
	sub_menu = glutCreateMenu(rightClickMenu);
	glutAddMenuEntry("Home", 1);
	glutAddMenuEntry("Stop Eclipse", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void subDisplay(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (l == 1)
	{
		angle_mn += 0.07;
		angle_ec += 0.08;
		lunarEclipse();
		glutSetMenu(sub_menu);
	}
	else if (s == 1)
	{
		angle_ec += 0.1;
		angle_mn += 0.06;
		solarEclipse();
		glutSetMenu(sub_menu);
	}
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void subReshape(int w, int h)
{
	const float ar = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5 / ar, 5 / ar, -20, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void rightClickMenu(int op)
{
	if (op == 1)
	{
		glutDestroyWindow(sub_window);
		if (e_flag == 0)
			e_flag = 1;
		l = 0;
		s = 0;
		angle_mn = 0;
		angle_ec = 0;
		home();
	}
	else if (op == 2)
	{
		e_flag = 0;
		angle_ec = 0;
	}
	else if (op == 3)
	{
		exit(0);
	}
}

void lunarEclipse()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat light_position_sun[] = { 3.0f, -3.0f, 3.0f, 0.0f };
	GLfloat light_position_earth[] = { -9.0f, 9.0f, 9.0f, 0.0f };
	GLfloat light_position_moon[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat high_shininess[] = { 100.0f };

	//sun
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(-1.8, 1.8, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_sun);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glutSolidSphere(1, 25, 25);
	glPopMatrix();

	//earth
	glPushMatrix();
	glColor3f(0.0, 0.4, 1.0);
	glTranslatef(2.0, -1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_earth);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glutSolidSphere(0.65, 20, 20);
	glPopMatrix();

	//moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(2.0, -1.0, 0.0);
	glRotatef(angle_mn, 0.0, 0.0, 1.0);
	glTranslatef(1.0, -0.8, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_moon);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glutSolidSphere(0.3, 20, 20);
	glPopMatrix();

	//lunar eclipse
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glColor3f(0.0, 0.0, 0.0);
	if (e_flag == 1)
	{
		glTranslatef(3.0, -1.8, 0.0);
	}
	glutSolidSphere(0.4, 20, 20);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void solarEclipse()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	GLfloat light_position_sun[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light_position_earth[] = { -1.0f, -1.0f, 1.0f, 0.0f };
	GLfloat light_position_moon[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat high_shininess[] = { 100.0f };

	//sun
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(-3.0, -2.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_sun);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glutSolidSphere(1, 25, 25);
	glPopMatrix();

	//earth
	glPushMatrix();
	glColor3f(0.0, 0.4, 1.0);
	glTranslatef(3.0, 1.5, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_earth);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glutSolidSphere(0.65, 20, 20);
	glPopMatrix();

	//moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(3.0, 1.5, 0.0);
	glRotatef(angle_mn, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_moon);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glutSolidSphere(0.3, 20, 20);
	glPopMatrix();

	//solar eclipse
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	if (e_flag == 1)
	{
		glTranslatef(1.0, 0.5, 0.0);
		glRotatef(angle_ec, 1.0, 1.0, 0.0);
		glTranslatef(2.0, 0.1, 0.0);
	}
	glutSolidSphere(0.25, 20, 20);
	glPopMatrix();
}
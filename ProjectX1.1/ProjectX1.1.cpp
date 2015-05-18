#include "stdafx.h"
#include<stdlib.h>
#include<fstream>
#include "GL/glut.h"
#include "GL/glaux.h"
#include <cstdio>
#include "Image.h"
#include <Windows.h>
#include<iostream>
#include <cstring>
#define GLUT_DISABLE_ATEXIT_HACK
#define eps 1e-3 
#define PI 3.14159265358979323
#define ENTER_VAL 13
#define WELCOME_WINDOW 1
#define SELECT_WINDOW 2
#define PLAY_WINDOW 3
#define WIN_WINDOW 4
#define LOOSE_WINDOW 5
using namespace std;
char orig_str[10][600], tempo[100][100], cur_ch;
Image *img1 , *img2 ;
bool stop;
int p1, p2;// indecies of the current letter
int time_left;//time left
int wrong_inp, right_inp;//no of typos
int lines_count;//number of lines rendered part of paragraph
int current_window = 1;  // current window to display
int  text_x = -400;
int text_y = 150;
int no_of_levels = 3;
char best_score[10][4];
/*********************************input expected***************************************/
int lvl_no = 1;//expecting lvl to be one based
char uname[100]; int uname_len = 0;//hardcoded expecting inp from the login scren
/************************************************************************/
int done = 0, flag = 1, cnt, f = -1, myXPos[25] = { 270, -270, -570, 570, 300, -300, -540, 540, 330, -330, 510, -510, -360, 360, -480, 480, 390, -390, 450, -450, -420, 420 };
GLfloat xx = 0, yy = 0, xx2 = -250, yy2 = 0, COLORS[6][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 1, 0, 1 }, { 1, 1, 0 }, { 0, 0, 1 }, { 0, 1, 1 } };
GLfloat WHITE[] = { 0.0, 0.0, 0, 0 };
/*********************************************************************/
char welcoming[100];
char level_data[] = "level *";
char welcome_str[100], wrong_str[100], timer_str[100],score_str[100];//eventually contains "Welcome, Samir sabri"
/*
discription: renders char* at certain coordinates
returns: void
*/
void drawBitmapText(char *str, float x, float y)
{
	char *c;
	glRasterPos2f(x, y);
	glColor3f(1.0, 1.0, 0.0);
	for (c = str; *c != NULL; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);//GLUT_BITMAP_TIMES_ROMAN_24
}
/*
discription: initializes my style
returns: void
*/
/*description: update all the scores of the players*/
void update_scores()
{
	FILE * outp = fopen("scores.txt", "r+");
	for (int i = 1; i <= no_of_levels; i++){
		fprintf(outp, "%s\n", best_score[i]);
	}
	fclose(outp);
}
void myStyleInit()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);//background color+ alpha object blending parameter
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-600, 600, -600, 600, 0, -1);
}
/*
discription: renders the title bar and writees on it a char*
returns: void
*/
void render_titlebar(char *user_name)
{
	*welcome_str = NULL;
	strcat(welcome_str, "Welcome, ");
	glColor3f(1.0, 1.0, 1.0);//Choosing the color of the displayed objects on the screen
	glPushMatrix();
	glBegin(GL_POLYGON);
	glVertex2f(-600, 550);
	glVertex2f(-600, 600);
	glVertex2f(600, 600);
	glVertex2f(600, 550);
	glEnd();
	glPopMatrix();
	glColor3f(0.0, 0.0, 0.0);//Text color Black
	glLoadIdentity();
	strcat(welcome_str, user_name);
	strcat(welcome_str, ".");
	drawBitmapText(welcome_str, -600 + 11, 600 - 35);
	glColor3f(0.0, 0.0, 0.0);//Text color Black
	*wrong_str = NULL;
	strcat(wrong_str, "Typo  ");
	wrong_str[5] = wrong_inp + '0';
	strcat(wrong_str, "/5");
	drawBitmapText(wrong_str, 400 + 11, 600 - 35);
	glColor3f(1.0, 0.0, 0.0);//Text color Black
	*timer_str = NULL;
	strcat(timer_str, "Time left 00:  ");
	timer_str[13] = time_left / 10 + '0';
	timer_str[14] = time_left % 10 + '0';
	drawBitmapText(timer_str, 0 + 11, 600 - 35);

	glColor3f(0.0, 0.0, 0.0);//Text color Black
	*score_str = NULL;
	strcat(score_str, "best score: ");
	strcat(score_str, best_score[lvl_no]);
	drawBitmapText(score_str, -250, 600 - 35);

	glutSwapBuffers();
}
/*
discription: renders the paragraph
returns: void
*/
void render_paragraph()
{
	bool space = 0;
	char *c;
	int i, j, x, y = 300;
	for (i = 0; i <= lines_count; i++){
		x = -500;
		c = tempo[i];
		for (j = 0; j<40; j++, c++, x += 20)
		{
			glColor3f(1.0, 1.0, 0.0);
			if (j == p2 && i == p1){
				glColor3f(1.0, 0.0, 0.0);
				if (*c == ' ')
					*c = '_', space = 1;
			}
			glRasterPos2f(x, y);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);//GLUT_BITMAP_TIMES_ROMAN_24
			if (space)
				*c = ' ', space = 0;
			if (*c == '.')
				break;
		}
		y -= 50;
		//glFlush () ;
	}
}


void render_background()
{
	string background;
	background = "wood.bmp";
	free(img2);
	img2 = new Image(background.c_str());
	glPushMatrix(); printf("X\n");

	glScalef(2, 2, 1);
	//glTranslatef(0, 0, 0);
	img2->display(-300, -300); 
	//img2->display(0, 0);
	glPopMatrix();
}

void render_keyboard()
{
	string temp_img;
	temp_img = "KeyBoard/";
	if (cur_ch == ' ')
		temp_img += "space";
	else if (cur_ch == '.')
		temp_img += "dot";
	else if (cur_ch == ';')
		temp_img += "semi";
	else if (cur_ch == ',')
		temp_img += "comma";
	else if (cur_ch == '\'')
		temp_img += "apos";
	else
		temp_img += cur_ch;
	temp_img += ".bmp";
	free(img1);
	img1 = new Image(temp_img.c_str());
	glPushMatrix();
	glScalef(2.4, 2.4, 1);
	img1->display(-200, -250);
	glPopMatrix();
}
/*
description : splits a char* into char** of size 40 each.
retruns : number of splitted char*
*/
int split_str(char *temp)
{
	int i, j, len;
	char *p = temp;
	//printf("%s\n", temp);
	for (i = 0, len = 0;; i++, len++)
	if (temp[i] == '.' || temp[i] == NULL)
		break;
	len++;
	//printf("len = %d\n", len);
	//printf("%s\n", p);
	i = 0;
	while (len)
	{
		for (j = 0; j<40 && len>0; j++, p++, len--)
			tempo[i][j] = *p;
		tempo[i][j] = '\0';
		i++;
	}
	return (i - 1);
}
/*
discription: chooses certain paragraph according to level
returns: void
*/
char* init(int lvl)
{
	int i = 0, j;
	lvl--;
	ifstream file("quotes.txt");
	string line;
	while (getline(file, line))
	{
		//cout << line << endl;
		for (j = 0; j<(int)line.size(); j++)
		{
			orig_str[i][j] = line[j];
		}
		orig_str[i++][j] = '\0';
	}
	//printf("%s\n", orig_str[lvl]);
	return orig_str[lvl];
}
void myTimer(int val)
{
	if (time_left <= 0)
		current_window = LOOSE_WINDOW;
	time_left--;
	glutPostRedisplay();
	glutTimerFunc(1000, myTimer, 0);
}
class Balloons {
public:
	double radius;
	GLfloat* color;
	double y;
	int rand1;
	double x;
	char c;
	int idle;
	Balloons() {
		radius = 30;
		y = -700;
		x = -1.0;
		c = 'a';
		idle = 1;
	}
	void create(int X)
	{
		x = X;
		c = '0' + right_inp / 5;
	}
	void update(int X)
	{
		float xC = COLORS[X % 6][0], yC = COLORS[X % 6][1], zC = COLORS[X % 6][2];

		if (!idle)
		{
			y += 10;
			int i;
			int triangleAmount = 20; //# of triangles used to draw circle

			GLfloat twicePi = 2.0f * PI;

			glColor3f(xC, yC, zC);
			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y); // center of circle
			for (i = 0; i <= triangleAmount; i++) {
				glVertex2f(
					x + (radius * cos(i *  twicePi / triangleAmount)),
					y + (radius * sin(i * twicePi / triangleAmount))
					);
			}
			glEnd();
			glColor3f(0, 0, 0);
			glRasterPos2d(x, y);
			glColor3f(0.0f, 0.0f, 0.0f);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
			glutPostRedisplay();

		}
		else
		{
			y = -700 + 85;
			int i;
			int triangleAmount = 20; //# of triangles used to draw circle

			//GLfloat radius = 0.8f; //radius
			GLfloat twicePi = 2.0f * PI;

			glColor3f(xC, yC, zC);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x, y); // center of circle

			for (i = 0; i <= triangleAmount; i++) {
				glVertex2f(
					x + (radius * cos(i *  twicePi / triangleAmount)),
					y + (radius * sin(i * twicePi / triangleAmount))
					);
			}
			glEnd();
			glColor3f(0, 0, 0);
			glRasterPos2d(x, y);
			glColor3f(0.0f, 0.0f, 0.0f);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
			glutPostRedisplay();
		}

		if (y > 700)
		{
			idle = 1;
			y = -700;
		}
	}
};
Balloons balloons[100];
void bgm(void) {
	PlaySound(TEXT("mySound.wav"), NULL, SND_ASYNC);
}
/*
discription: My display function
returns: void
*/
void clear_balloons()
{
	for (int i = 0; i<100; i++){
		balloons[i].radius = 30;
		balloons[i].y = -700;
		balloons[i].x = -1.0;
		balloons[i].c = 'a';
		balloons[i].idle = 1;
	}
}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPointSize(4.0);
	if (current_window == PLAY_WINDOW){
		glutSetCursor(GLUT_CURSOR_BOTTOM_LEFT_CORNER);
	//	render_background();
		render_titlebar(uname);
		render_paragraph();
		render_keyboard();

		for (int i = 0; i < 100; i++)
		{
			if (done && balloons[i].idle)
			{
				int X = myXPos[cnt % 22];
				balloons[i].create(X);
				balloons[i].idle = 0;
				balloons[i].update(i);
				cnt++;
				done = 0;
			}
			else if (!balloons[i].idle)
				balloons[i].update(i);
		}
	}
	else if (current_window == WELCOME_WINDOW){
		glColor3f(0, 0, 0);
		drawBitmapText("Enter your name:", text_x, text_y);
		drawBitmapText(uname, text_x + 400, text_y);
	}
	else if (current_window == SELECT_WINDOW){
		printf("%s\n%s\n%s\n", best_score[1], best_score[2], best_score[3]);
		int xval = -200, topy = 400;
		strcpy(welcoming, "Welcome ");
		strcat(welcoming, uname);
		strcat(welcoming, " !");
		drawBitmapText(welcoming, xval, topy);
		topy -= 50;
		for (int i = 1; i <= no_of_levels; i++){
			topy -= 150;
			glRasterPos2i(xval, topy);
			if (lvl_no == i)
				glColor3f(0, 1, 0);
			else
				if (best_score[i][0] == '0'&&best_score[i][1] == '0')
					glColor3f(0.752941176, 0.752941176, 0.752941176); // color of polygon
				else
					glColor3f(0, 0, 1);

			glBegin(GL_POLYGON);
			glVertex2i(xval, topy);
			glVertex2i(xval, topy + 100);
			glVertex2i(xval + 350, topy + 100);
			glVertex2i(xval + 350, topy);
			glEnd();
			glColor3f(0, 0, 0);
			level_data[6] = i + '0';
			drawBitmapText(level_data, xval + 110, topy + 30);
			time_left = 59;
		}
	}
	else if (current_window == WIN_WINDOW){
		int cur_score = time_left * 5 / 3.0;
		int max_score = best_score[lvl_no][1] - '0' + (best_score[lvl_no][0] - '0') * 10;
//		printf("%d %d %d\n", cur_score, time_left,max_score);

		if (cur_score > max_score){
			best_score[lvl_no][1] = (cur_score % 10) + '0', best_score[lvl_no][0] = cur_score / 10 + '0';
			update_scores();
		}
		int c = 0;
		int xval = -250, topy = 400; // decrease topy
		char message[100];
		strcpy(message, "Congratulations ");
		strcat(message, uname);
		strcat(message, " !");
		drawBitmapText(message, xval, topy);
		strcpy(message, "You've passed level * with score ");
		for (; message[c] != '*'; c++);
		message[c] = lvl_no + '0';
		strcat(message, best_score[lvl_no]);
		drawBitmapText(message, xval - 100, topy - 100);
	}
	else if (current_window == LOOSE_WINDOW){
		int xval = -150, topy = 200;
		char message[100];
		strcpy(message, "You Lost");
		drawBitmapText(message, xval, topy);
		strcpy(message, "Try again !");
		drawBitmapText(message, xval, topy - 100);
	}

	glFlush();
}
bool name_flag;
static void keyBoard(unsigned char ch, int x, int y)
{
	if (current_window == PLAY_WINDOW){
		if (ch == tempo[p1][p2])
		{
			p2++;
			if (p2 == 40)
				p2 = 0, p1++;
			if (ch == '.')
				stop = 1;
			right_inp++;
			if (right_inp % 5 == 0)
				done = 1;
		}
		else
			wrong_inp++;
		if (wrong_inp == 5)
			current_window = LOOSE_WINDOW;
		if (stop)
			current_window = WIN_WINDOW;
		cur_ch = tempo[p1][p2];
	}
	else if (current_window == WELCOME_WINDOW){
		if (ch == 8){ // backspace
			if (uname_len != 0)
				uname[--uname_len] = '\0';
		}
		else if (ch == 13){ // enter
			name_flag = true;
			if (uname_len < 3)name_flag = false;
			for (int i = 0; i < uname_len;i++)
			if (!isalpha(uname[i]) && uname[i] != ' ')
				name_flag = false;
			if (name_flag)
				current_window = SELECT_WINDOW;
		}
		else
			uname[uname_len++] = ch, uname[uname_len] = '\0';
		//printf("%s\n", uname);
	}
	else if (current_window == SELECT_WINDOW){
		if (ch == 'w')
			lvl_no++;
		else if (ch == 's')
			lvl_no--;
		if (lvl_no == 0)
			lvl_no = 3;
		else if (lvl_no == 4)
			lvl_no = 1;
		//		printf("%d\n", lvl_no);
		char* inp = init(lvl_no);
		lines_count = split_str(inp);
		cur_ch = tempo[0][0];
		if (ch == ENTER_VAL)
			current_window = PLAY_WINDOW;
		stop = 0;
		time_left = 59;
		wrong_inp = 0;
		p1 = 0; p2 = 0;
		clear_balloons();
	}
	else if (current_window == WIN_WINDOW){
		if (ch == ENTER_VAL){
			current_window = SELECT_WINDOW;
		}

	}
	else if (current_window == LOOSE_WINDOW){
		if (ch == ENTER_VAL){
			uname_len = 0;
			uname[uname_len] = '\0';
			current_window = SELECT_WINDOW;
		}

	}
	myDisplay();
}
/*description: read scores of the players for each level*/
void read_scores()
{
	FILE * inpt = fopen ("scores.txt", "r");
	for (int i = 1; i <= no_of_levels; i++){
		fscanf(inpt, "%s", best_score[i]);
		printf("%s\n", best_score[i]);
	}
	fclose(inpt);
}
/*******************************************/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(750, 50);
	glutCreateWindow("Welcome to our funky game!");
	myStyleInit();
	time_left = 60;
	read_scores();
	glutDisplayFunc(myDisplay);
	glutTimerFunc(1000, myTimer, 0);
	glutKeyboardFunc(keyBoard);
	bgm();
	glutMainLoop();
	return 0;
}

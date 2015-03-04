#ifndef FPS_H
#define FPS_H
#include<iostream>
#include<windows.h>
#include<string>
#include<GL\glut.h>
#include<fstream>

class fps
{
	float Fps;
	float current_time;
	float previous_time;
	GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;
	float frame;
public:
	fps()
	{
		current_time = 0.0;
		previous_time = 0.0;
		frame = 0.0;
		Fps = 0.0f;
	}

	void calculate_fps();
	void draw_fps();
private:
	void printw(float x, float y, float z, char* format, ...);
};



#endif

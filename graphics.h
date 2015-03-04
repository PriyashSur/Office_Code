#ifndef GRAPHICS_H
#define GRAPHICS_H
#include<iostream>
#include<windows.h>
#include<string>
#include<fstream>
#include<GL\glut.h>
#include"loader.h"
#include"fps.h"

class graphics
{
	HDC hdc;
	HWND mhwnd;
	HGLRC renderContext;
	Loader* loader;
	string file;
	fps* f;
	GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;;
public:
	graphics()
	{
		loader = Model::getPLYLoader();
		f = new fps();

	}
	~graphics()
	{

	}
	void Init(HWND hwnd);
	void Render();
	void clean();
	void Print_Debug(int x, int y, int z, char* format, ...);
};




#endif

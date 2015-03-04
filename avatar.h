#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL\gl.h>
#include <GL\glu.h>

class Avatar
{
public:
	float vertices[20000][3];
	float normals[20000][3];
	float textures[20000][2];
	int faces[40000][3][3];
	int vertI, normI, texI,faceI;
	float maxY, minY;
	GLuint texture;
	int texW, texH;
public:
	Avatar();
	void readData(LPCSTR filename);
	~Avatar();
	void drawAvatar();
	void loadTexture();
};


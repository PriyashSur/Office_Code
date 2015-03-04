#include "afx.h"

#include "Avatar.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include "Display.h"

Avatar::Avatar()
{
	vertI = 0;
	normI = 0;
	texI = 0;
	faceI = 0;
	maxY = -999.0;
	minY = 999.0;
	texW = 2048;
	texH = 2048;
}
void Avatar::readData(LPCSTR filename)
{
		CString s;
	std::ifstream myReadFile;
	myReadFile.open("E:\\Code\\data\\Avatars\\AvatarsT.obj");
	std::string line, tmp, v1, v2, v3;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {


			std::getline(myReadFile, line);
			if (line[0] == 'v' && line[1] == ' '){
				std::istringstream elems(line);
				elems >> tmp >> vertices[vertI][0] >> vertices[vertI][1] >> vertices[vertI][2];
				vertices[vertI][1] -= 10;
				
				if (maxY < vertices[vertI][1])  {
					maxY = vertices[vertI][1];
				}
				if (minY > vertices[vertI][1]){
					minY = vertices[vertI][1];
				}
				vertI++;
			}
			if (line[0] == 'v' && line[1] == 't'){
				std::istringstream elems(line);
				elems >> tmp >> textures[texI][0] >> textures[texI][1] ;
				texI++;
			}
			if (line[0] == 'f'){
				std::istringstream elems(line);
				elems >> tmp >> v1 >> v2 >> v3;
				std::istringstream ev1(v1);
				std::istringstream ev2(v2);
				std::istringstream ev3(v3);

				ev1 >> faces[faceI][0][0] >> faces[faceI][0][1] >> faces[faceI][0][2];
				ev2 >> faces[faceI][1][0] >> faces[faceI][1][1] >> faces[faceI][1][2];
				ev3 >> faces[faceI][2][0] >> faces[faceI][2][1] >> faces[faceI][2][2];

				faceI++;
			}


		}

		s.Format(_T("maxY: %f    minY:%f\n"), maxY,minY);
		OutputDebugString((LPCWSTR)s);
	}
	loadTexture();
	//s.Format(_T("%i\n"), i);
	//OutputDebugString((LPCWSTR)s);
}


Avatar::~Avatar()
{
}

void Avatar::loadTexture()
{
	CString s;

	Display disp;
	texture = disp.texture_load("E:\\Code\\data\\Avatars\\textures\\texture.ppm", texW, texH);
	if (texture == 0)
	{
		OutputDebugString((LPCWSTR)L"\nTexture Load Failed\n");
	}
	else
	{
		OutputDebugString((LPCWSTR)L"\nTexture Load Successful\n");
//		s.Format(_T("%s\n"), texture);
//		OutputDebugString((LPCWSTR)s);
	}
}

void Avatar::drawAvatar()
{

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glPushMatrix();
	//glTranslatef(0.0, (maxY - minY) / 2.0, 0.0);
	glScalef(0.95,0.95,0.95);
	for (int i = 0; i < faceI; i++)
	{
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++){
		
//			glColor3f((GLfloat)textures[faces[i][j][0]][0], (GLfloat)textures[faces[i][j][0]][1], (GLfloat)textures[faces[i][j][0]][2]);
			glTexCoord2f(textures[faces[i][j][2] - 1][0], textures[faces[i][j][2] - 1][1]);

			glVertex3f(vertices[faces[i][j][0]-1][0], vertices[faces[i][j][0]-1][1], vertices[faces[i][j][0]-1][2]);
			/*CString s;
			s.Format(_T("%f %f %f\n"), vertices[faces[i][j][0] - 1][0], vertices[faces[i][j][0] - 1][1], vertices[faces[i][j][0] - 1][2]);
			OutputDebugString((LPCWSTR)s);*/
		}
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

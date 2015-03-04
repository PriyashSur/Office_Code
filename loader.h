#ifndef LOADER_H
#define LOADER_H
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<string>
#include<vector>
#include"rply.h"
#include<stdio.h>
#include<GL\glut.h>
#include<GL\GLU.h>
#include<GL\GL.h>

using namespace std;


struct vec3
{
	float x;
	float y;
	float z;
};

struct vec2
{
	float u;
	float v;
};

class Loader
{
public:

	//common between loaders
	virtual void load(string filename) = 0;
	virtual void render() = 0;
	virtual void clean() = 0;

	//----------------------OBJ Loader-------------------------------------------------//
	//dummy variable
	vector<vec2> v2;
	vector<vec3> v3;
	vector<vector<int>>v;

	//vertices
	virtual void addVertices(vec3 t){};
	virtual vector<vec3>getVertices(){ return v3; }
	//vertice normals
	virtual void addVerticeNormals(vec3 t){};
	virtual vector<vec3> getVerticeNormals(){ return v3; }
	//Material File
	virtual string getMaterialFile(){ return " "; }
	//FACES :- you could take unordered_map to fetch and store the exact snapshot of the face parameters
	virtual vector <vector<int>> getFaces(){ return v; }
	//vertex texture 
	virtual void addVertexTexture(vec2 t){}
	virtual vector<vec2>getVertexTexture(){ return v2; }

	//-----------------------END OBJ Loader------------------------------------------//

	//--------------------------rply loader----------------------------------------//


	virtual int getHeader(){ return 0; }
	vector<GLfloat>ver, ind;
	virtual vector<GLfloat>getPlyVertices()const{ return ver; }
	virtual vector<GLfloat>getPlyIndices()const{ return ind; }
};

class OBJLoader :public Loader
{

	vector<vec3>v;
	vector<vec3>vn;
	vector<vec2>vt;
	vector<vec3>f;
	ifstream reader;
	string material_file;
	vector<vector<int>>faces;
	int face_grouping;
	bool hasVertex;
	bool hasVertexNormal;
	bool hasVertexTexture;
public:
	OBJLoader()
	{
		hasVertex = false;
		hasVertexNormal = false;
		hasVertexTexture = false;
		face_grouping = 0;
	}

	void load(string filename);
	void render(){}
	void clean(){}
	//vertices
	void addVertices(vec3 t){ v.push_back(t); }
	vector<vec3>getVertices(){ return v; }
	//vertex textures
	void addVertexTexture(vec2 t){ vt.push_back(t); }
	vector<vec2>getVertexTexture(){ return vt; }
	//pre-computed normals
	void addVerticeNormals(vec3 t){ vn.push_back(t); }
	vector<vec3>getVerticeNormals(){ return vn; }
	//material
	string getMaterialFile(){ return material_file; }
	//faces
	vector <vector<int>> getFaces(){ return faces; }

};


class PLYLoader :public Loader
{
	p_ply ply;
	int header;
	vector<GLfloat>p_vertices;
	vector<GLfloat>p_indices;
    static int vertex_cb(p_ply_argument arg);
	static int face_cb(p_ply_argument arg);

public:
	PLYLoader()
	{

	}
	void load(string filename);
	void render();
	void clean();
	int getHeader(){ return header; }
	vector<GLfloat>getPlyVertices()const{ return p_vertices; }
	vector<GLfloat>getPlyIndices()const{ return p_indices; }
};

class Model
{

	Loader* loader;
public:
	Model(){}
	static Loader* getOBJLoader(){ return new OBJLoader(); }
	static Loader* getPLYLoader(){ return new PLYLoader(); }
};

#endif

#include"Loader.h"

void OBJLoader::load(string filename)
{

	reader.open(filename.c_str(), ios::in | ios::binary);
	string line;
	int mat_length = strlen("mtllib");
	int vlen = strlen("v");
	int vnlen = strlen("vn");
	int vtlen = strlen("vt");
	int flen = strlen("f");
	while (getline(reader, line))
	{
		if (line[0] == '#' | line[0] == '\r')continue;

		if (line.substr(0, mat_length) == "mtllib")
		{
			istringstream s(line.substr(mat_length + 1));
			s >> material_file;
		}
		else if (line[0] == 'v'&&line[1] == ' ')
		{
			//adding vertices
			istringstream s(line.substr(vlen + 1));
			vec3 vert;
			s >> vert.x >> vert.y >> vert.z;
			addVertices(vert);
			hasVertex = true;
		}
		else if (line[0] == 'v'&&line[1] == 'n')
		{
			//adding precomputed normals
			istringstream s(line.substr(vnlen + 1));
			vec3 _vn;
			s >> _vn.x >> _vn.y >> _vn.z;
			addVerticeNormals(_vn);
			hasVertexNormal = true;
		}
		else if (line[0] == 'v'&&line[1] == 't')
		{
			//adding vertex texture
			istringstream s(line.substr(vtlen + 1));
			vec2 _vt;
			s >> _vt.u >> _vt.v;
			addVertexTexture(_vt);
			hasVertexTexture = true;
		}
		else if (line.substr(0, flen) == "f")
		{
			//adding faces which consist vertex index and vertex normals combination
			line = line.substr(line.find_first_of(' ') + 1);
			char* ch = strdup(line.c_str());
			char* pch = strtok(ch, "// ");
			vector<int>out;
			while (pch != NULL)
			{
				istringstream s(pch);
				int l;
				//this while loop is important otherwise it will print the last digit again
				while (s >> l)
				{
					out.push_back(l);
				}
				pch = strtok(NULL, "// ");
			}
			faces.push_back(out);
			out.clear();
		}
	}
}


static vector<GLfloat>vert;
static vector<GLfloat>indices;
int PLYLoader::vertex_cb(p_ply_argument arg)
{
	long eol;
	ply_get_argument_user_data(arg, NULL, &eol);
	//cout<<ply_get_argument_value(arg);
	vert.push_back(ply_get_argument_value(arg));
/*	if (eol)
		cout << endl;
	else
		cout << " ";*/
	return 1;
}

int PLYLoader::face_cb(p_ply_argument arg)
{
	long length, index;
	ply_get_argument_property(arg, NULL, &length, &index);
	switch (index)
	{
	case 0:
	case 1:
		//cout << ply_get_argument_value(arg);
		indices.push_back(ply_get_argument_value(arg));
		break;
	case 2:
		//cout << ply_get_argument_value(arg);
		indices.push_back(ply_get_argument_value(arg));
		break;
	default:
		break;
	}

	return 1;
}

void PLYLoader::load(string filename)
{
	ply=ply_open(filename.c_str(), NULL, 0, NULL);
	if (!ply)return;
	header = ply_read_header(ply);

	int nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
	/*ply_set_read_cb(ply, "vertex", "nx", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "ny", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "nz", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "red", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "green", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "blue", vertex_cb, NULL, 1);*/
	int ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
	if (!ply_read(ply))return;
	p_vertices = vert;
	p_indices = indices;
	vert.clear();
	indices.clear();
	
}



void PLYLoader::render()
{
	glLoadIdentity();
	glScalef(0.004f, 0.004f, 0.004f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < p_vertices.size(); i=i+3)
	{
		glVertex3f(p_vertices[i], p_vertices[i + 1], p_vertices[i + 2]);
		
	}
    glEnd();
	
}


void PLYLoader::clean()
{
	ply_close(ply);
}

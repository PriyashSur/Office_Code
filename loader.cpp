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
static vector<GLfloat>normals;
static vector<GLfloat>colors;
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

int PLYLoader::normal_cb(p_ply_argument arg)
{
	long eol;
	ply_get_argument_user_data(arg, NULL, &eol);
	normals.push_back(ply_get_argument_value(arg));
	return 1;
}


int PLYLoader::color_cb(p_ply_argument arg)
{
	long eol;
	ply_get_argument_user_data(arg, NULL, &eol);
	colors.push_back(ply_get_argument_value(arg)/255);
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
	ply_set_read_cb(ply, "vertex", "nx", normal_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "ny", normal_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "nz", normal_cb, NULL, 1);
	ply_set_read_cb(ply, "vertex", "red", color_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "green", color_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "blue", color_cb, NULL, 1);
	int ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
	if (!ply_read(ply))return;
	p_vertices = vert;
	p_indices = indices;
	p_normals = normals;
	p_colors = colors;
	vert.clear();
	indices.clear();
	normals.clear();
	colors.clear();
	
}



void PLYLoader::render()
{
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT,0, &p_vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &p_normals[0]);
	glColorPointer(3, GL_FLOAT, 0, &p_colors[0]);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.5, 0.5, 0.5, 5.0, 5.0, 5.0, 20, -40, 100);
	
	glScalef(0.006, 0.006, 0.006);
	glDrawArrays(GL_QUADS, 0, p_vertices.size()/3);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}


void PLYLoader::clean()
{
	ply_close(ply);
}

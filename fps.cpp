#include"fps.h"


void fps::calculate_fps()
{
	frame++;
	current_time = glutGet(GLUT_ELAPSED_TIME);
	float delta = current_time - previous_time;
	if (delta > 1000)
	{
		Fps = frame / (float)(delta / 1000.0f);
		previous_time = current_time;
		frame = 0;
	}
}

void fps::printw(float x, float y, float z, char* format, ...)
{
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list 
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(x, y, z);
	
	//  Draw the characters one by one
	
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}

void fps::draw_fps()
{
	glLoadIdentity();
	glColor3f(1.0f, 0, 0);
	printw(-0.98f,0.93f,0, "FPS : %4.2f", Fps);
	glLoadIdentity();
}

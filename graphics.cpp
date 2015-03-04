#include"graphics.h"


void graphics::Print_Debug(int x, int y, int z, char* format, ...)
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


void graphics::Init(HWND hwnd)
{
	mhwnd = hwnd;
	hdc = GetDC(mhwnd);
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(&pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, format, &pfd);

	//Creating render context
	renderContext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, renderContext);
	loader->load("C:\\Users\\admin\\Desktop\\ReconstructMe_Output\\Software_Results\\test12_stand_rotate_1m_selfie_mode.ply");
}

void graphics::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//call the draw function 
	f->calculate_fps();
	f->draw_fps(); 
	loader->render();
	SwapBuffers(hdc);
}



void graphics::clean()
{
	loader->clean();
	if (renderContext)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(renderContext);
	}

	if (mhwnd&&hdc)
	{
		ReleaseDC(mhwnd, hdc);
	}
	mhwnd = NULL;
	hdc = NULL;
	renderContext = NULL;
}

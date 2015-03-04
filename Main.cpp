#include"system.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
	System* sys = new System(L"Window_App", 1580, 880);
	sys->Init(hInstance);
	sys->run();
	return 0;
}

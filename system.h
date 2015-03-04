#ifndef SYSTEM_H
#define SYSTEM_H
#include<windows.h>
#include<iostream>
#include"graphics.h"
#include<reconstructmesdk\reme.h>
#include<fstream>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HINSTANCE _hinstance;
class System
{
	
	LPCWSTR app_name;
	int screenWidth;
	int screenHeight;
	HWND main_hwnd;
	HWND child1_hwnd;
	HWND button_hwnd;
	HWND snapshot_hwnd;
	HWND load_hwnd;
	HWND apply;
	int posx, posy;
	graphics* gr;

private:
	void shutdown();
	void renderScene();
	static System* instances;
	
public:
	System(LPCWSTR app_name = L"Default_App_Name", int screenWidth = 0, int screenHeight = 0)
	{
		_hinstance = NULL;
		main_hwnd = NULL;
		child1_hwnd = NULL;
		button_hwnd = NULL;
		gr = new graphics();
		this->app_name = app_name;
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		posx = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	int getScreenWidth(){ return screenWidth; }
	int getScreenHeight(){ return screenHeight; }
	static System* getInstances(){ return instances; }
	
	~System()
	{
	}

	void Init(HINSTANCE);
	void run();
	
};
void start_scan();
void finalize_ouput(const char* filename);
void load(HWND hwnd);
#endif

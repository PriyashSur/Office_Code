#include"system.h"
#define BUTTON_ID 101
#define BUTTON_ID_2 102
#define BUTTON_ID_3 103


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BUTTON_ID:
			start_scan();
			break;
		}
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}



void System::Init(HINSTANCE hinst)
{
	_hinstance = hinst;
	WNDCLASSEX wnd = {};
	ZeroMemory(&wnd, sizeof(wnd));
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd.hCursor = LoadCursor(_hinstance, IDC_ARROW);
	wnd.hIcon = LoadIcon(_hinstance, IDI_WINLOGO);
	wnd.hIconSm = 0;
	wnd.hInstance = _hinstance;
	wnd.lpfnWndProc = (WNDPROC)WndProc;
	wnd.lpszClassName = app_name;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	//Register for the window class
	RegisterClassEx(&wnd);
	//Create the class
	main_hwnd = CreateWindowEx(NULL, app_name, L"ReconstructMe_App", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 5, 5, screenWidth, screenHeight, NULL, NULL, _hinstance, NULL);
	if (!main_hwnd)
	{
		MessageBox(NULL, L"Error_Creating_Window", L"Window_Error", MB_OK);
		return;
	}

	ShowWindow(main_hwnd,SW_SHOW);
	UpdateWindow(main_hwnd);

	//child window #1
	child1_hwnd = CreateWindowEx(NULL, app_name, L"RenderWindow", WS_CHILD | WS_VISIBLE | WS_DLGFRAME, 220, 0, screenWidth - 200, screenHeight - 200, main_hwnd, NULL, _hinstance, NULL);
	ShowWindow(child1_hwnd, SW_SHOW);
	UpdateWindow(child1_hwnd);
	//Initialize
	gr->Init(child1_hwnd);

	//child snap show window #2
	snapshot_hwnd = CreateWindowEx(NULL, app_name, L"SnapShot", WS_CHILD | WS_VISIBLE | WS_DLGFRAME, 220, screenHeight - 199, screenWidth - 200, 195, main_hwnd, NULL, _hinstance, NULL);
	ShowWindow(snapshot_hwnd, SW_SHOW);
	UpdateWindow(snapshot_hwnd);

	//child button window #3
	button_hwnd = CreateWindowEx(NULL, L"Button", L"Scan", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 10, 100, 50, main_hwnd, (HMENU)BUTTON_ID, _hinstance, NULL);
	ShowWindow(button_hwnd, SW_SHOW);
	UpdateWindow(button_hwnd);
	
	//child load window #4
	load_hwnd = CreateWindowEx(NULL, L"Button", L"Load", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 150, 100, 50, main_hwnd, (HMENU)BUTTON_ID_2, _hinstance, NULL);
	ShowWindow(load_hwnd, SW_SHOW);
	UpdateWindow(load_hwnd);

	//child apply window #5
	apply = CreateWindowEx(NULL, L"Button", L"Apply", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 280, 100, 50, main_hwnd, (HMENU)BUTTON_ID_3, _hinstance, NULL);
	ShowWindow(apply, SW_SHOW);
	UpdateWindow(apply);

}

void finalize_ouput(const char* filename)
{
	reme_context_t c;
	reme_context_create(&c);
	reme_volume_t v;
	reme_volume_create(c, &v);
	reme_surface_t m;
	reme_surface_create(c, &m);
	reme_surface_load_from_file(c, m,filename);
	reme_surface_colorize_vertices(c, m, v);
	reme_surface_inpaint_vertices(c, m);
	reme_surface_save_to_file(c, m, filename);
	// Print pending errors
	reme_context_print_errors(c);
}

void start_scan()
{
	const char* filename = "C:\\Users\\admin\\Desktop\\ReconstructMe_Output\\UI_Win32\\final_output3.ply";
	reme_context_t c;
	reme_context_create(&c);
	// Create a license object
	reme_license_t l;
	reme_license_create(c, &l);
	reme_error_t e = reme_license_authenticate(c, l, "license.txt.sgn");
	// Create options
	reme_options_t o;
	reme_options_create(c, &o);
	// Create a new sensor.
	reme_sensor_t s;
	reme_sensor_create(c, "openni;mskinect;file", true, &s);
	reme_sensor_bind_camera_options(c, s, o);
	// Ensure AUX stream is aligned with the DEPTH stream
	reme_options_set_bool(c, o, "enable_align_viewpoints", true);
	reme_sensor_open(c, s);
	reme_context_bind_reconstruction_options(c, o);
	// Make sure that we enable color support in the program settings.
	reme_options_set_bool(c, o, "data_integration.use_colors", true);
	// Use the tuning to derive a couple of related settings automatically.
	reme_context_tune_reconstruction_options(c, REME_TUNE_PROFILE_MID_QUALITY);
	// Compile for OpenCL device using defaults
	reme_context_compile(c);
	// Create a new volume
	reme_volume_t v;
	reme_volume_create(c, &v);
	// Position sensor outside of volume
	reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
	reme_sensor_bind_render_options(c, s, o);
	// Use color in rendering volume image
	reme_options_set(c, o, "shade_mode", "SHADE_COLORS");
	// Apply
	reme_sensor_apply_render_options(c, s, o);
	reme_viewer_t viewer;
	reme_viewer_create_image(c, "This is ReconstructMe SDK", &viewer);
	reme_image_t volume, aux;
	reme_image_create(c, &volume);
	reme_image_create(c, &aux);
	reme_viewer_add_image(c, viewer, aux);
	reme_viewer_add_image(c, viewer, volume);
	// Perform reconstruction until no more frames are left
	int time = 0;
	while (time < 200 && REME_SUCCESS(reme_sensor_grab(c, s))) {
		reme_sensor_prepare_images(c, s);
		if (REME_SUCCESS(reme_sensor_track_position(c, s))) {
			reme_sensor_update_volume(c, s);
		}
		reme_sensor_get_image(c, s, REME_IMAGE_AUX, aux);
		reme_sensor_get_image(c, s, REME_IMAGE_VOLUME, volume);
		reme_viewer_update(c, viewer);
		time += 1;
	}
	// Close and destroy the sensor, it is not needed anymore
	reme_sensor_close(c, s);
	reme_sensor_destroy(c, &s);
	// Create a new surface
	reme_surface_t m;
	reme_surface_create(c, &m);
	reme_surface_generate(c, m, v);
	// Colorize surface vertices.
	reme_surface_colorize_vertices(c, m, v);
	// Inpaint vertices with invalid colors.
	reme_surface_inpaint_vertices(c, m);
	// Remesh the surface to generate an isotropic tessellation
	reme_surface_bind_remesh_options(c, m, o);
	// The following properties define the allowed edge length range
	// The tesselation will adhere to these values when splitting/collapsing
	// edges according to local color and geometry change.
	reme_options_set_real(c, o, "minimum_edge_length", 3);
	reme_options_set_real(c, o, "maximum_edge_length", 20);
	// Perform remeshing
	reme_surface_remesh(c, m);
	// Remeshing as decimation might change vertex positions,
	// we should update the color information
	reme_surface_colorize_vertices(c, m, v);
	reme_surface_inpaint_vertices(c, m);
	// Save to file including colored vertices.
	reme_surface_save_to_file(c, m, filename);
	reme_viewer_destroy(c, &v);
	//final generated mesh
	finalize_ouput(filename);
	
}





void System::run()
{
	MSG msg = {};
	float previous_time = glutGet(GLUT_ELAPSED_TIME);
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float current_time = glutGet(GLUT_ELAPSED_TIME);
			float dt = (float)(current_time - previous_time) / 1000.0f;
			previous_time = current_time;
			gr->Render();
			
		}
	}

	//shutdown
	shutdown();
	return;
}


void System::shutdown()
{
	gr->clean();
	UnregisterClass(app_name, _hinstance);
	CoUninitialize();
	main_hwnd = NULL;
	_hinstance = NULL;
}

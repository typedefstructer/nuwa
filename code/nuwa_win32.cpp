#include<windows.h>

#define global static
#define internal static

#include "nuwa_game.cpp"

struct win32_window_dimension
{
	int width;
	int height;
};

struct win32_bitmap_buffer
{
	BITMAPINFO info;
	int width;
	int height;
	void *memory;
};

global bool global_app_running;
global win32_bitmap_buffer global_bitmap_buffer;
global i64 global_performance_frequency;

internal void
win32blitbitmap(HDC device_context, win32_bitmap_buffer *buffer, int window_width, int window_height)
{
	int offset_x = (window_width - buffer->width)/2;
	int offset_y=  (window_height - buffer->height)/2;

	PatBlt(device_context, 0, 0, offset_x, window_height, WHITENESS);
	PatBlt(device_context, offset_x + buffer->width, 0, window_width, window_width, WHITENESS);
	PatBlt(device_context, 0, offset_y + buffer->height, window_width, window_height, WHITENESS);
	PatBlt(device_context, 0, 0, window_width, offset_y, WHITENESS);

	StretchDIBits(device_context,
				  offset_x, offset_y, buffer->width, buffer->height,
				  0,0, buffer->width, buffer->height,
				  buffer->memory, &buffer->info,
				  DIB_RGB_COLORS, SRCCOPY);
}

internal void
win32allocbitmapbuffer(win32_bitmap_buffer *buffer, int width, int height)
{
	if(buffer->memory)
	{
		VirtualFree(buffer->memory, 0, MEM_RELEASE);
	}

	buffer->info = {};
	buffer->info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	buffer->info.bmiHeader.biWidth = width;
	buffer->info.bmiHeader.biHeight = -height;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = 32;
	buffer->info.bmiHeader.biCompression = BI_RGB;

	buffer->width = width;
	buffer->height = height;

	int buffer_size = width*height*4;
	buffer->memory = VirtualAlloc(0, buffer_size, MEM_COMMIT, PAGE_READWRITE);
}

internal win32_window_dimension
win32windowdimension(HWND window)
{
	win32_window_dimension result;
	RECT rect;
	GetClientRect(window, &rect);

	result.width  = rect.right  - rect.left;
	result.height = rect.bottom - rect.top;

	return result;
}

internal void
win32handleevents(game_controller *keyboard_controller)
{
	MSG message;
	while(PeekMessage(&message, 0, 0, 0, PM_REMOVE) > 0)
	{
		switch(message.message)
		{
			case WM_KEYUP:
			case WM_KEYDOWN:
			case WM_SYSKEYUP:
			case WM_SYSKEYDOWN:
			{
				b32 was_down = ((message.lParam & 1<<30) != 0);
				b32 is_down  = ((message.lParam & 1<<31) == 0);
				DWORD vk_code = message.wParam;

				if(is_down != was_down)
				{
					switch(vk_code)
					{
						case VK_UP:
						case 'W':
						{
							keyboard_controller->moveup = is_down;
						} break;

						case VK_DOWN:
						case 'S':
						{
							keyboard_controller->movedown = is_down;
						} break;

						case VK_LEFT:
						case 'A':
						{
							keyboard_controller->moveleft = is_down;
						} break;

						case VK_RIGHT:
						case 'D':
						{
							keyboard_controller->moveright = is_down;
						} break;
					}
				}

			} break;

			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			} break;
		}
	}
}

internal LRESULT CALLBACK
win32windowprocdedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

	switch(message)
	{
		case WM_CLOSE:
		{
			global_app_running = false;
		} break;

		default:
		{
			result = DefWindowProc(window, message, wparam, lparam);
		}
	}

	return result;
}

internal i64
win32performancecounter()
{
	i64 result = 0;
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	result = counter.QuadPart;
	return result;
}

internal r32
win32gettimeinmillis(i64 start, i64 end)
{
	r32 result = 0;
	result = ((r32)(end-start)/(r32)(global_performance_frequency));
	result *= 1000.0f;

	return result;
}

internal int CALLBACK
WinMain(HINSTANCE instance,
		HINSTANCE previnstance,
		LPSTR cmdline,
		int cmdshow)
{
	WNDCLASSEX windowclass = {};
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.style = CS_HREDRAW | CS_VREDRAW;
	windowclass.lpfnWndProc = win32windowprocdedure;
	windowclass.hInstance = instance;
	windowclass.hCursor = LoadCursor(0, IDC_ARROW);
	windowclass.hIcon = 0; // TODO
	windowclass.lpszClassName = "nlife-snake";

	RegisterClassEx(&windowclass);
	HWND window = CreateWindowEx(0, windowclass.lpszClassName, "nlife | snake | immortal",
								 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								 CW_USEDEFAULT, CW_USEDEFAULT,
								 CW_USEDEFAULT, CW_USEDEFAULT,
								 0, 0, instance, 0);

	/*TODO*/

	RECT rect;
	rect.left = 0;
	rect.right = 960;
	rect.top = 0;
	rect.bottom = 540;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(window, 0, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE);

	/**/

	win32allocbitmapbuffer(&global_bitmap_buffer, 960, 540);

	game_memory gamememory = {};
	gamememory.permanent_storage_size = MB(100);
	gamememory.permanent_storage = VirtualAlloc(0, gamememory.permanent_storage_size,
												 MEM_COMMIT, PAGE_READWRITE);

	game_bitmap_buffer  gamebitmapbuffer= {};
	gamebitmapbuffer.width = global_bitmap_buffer.width;
	gamebitmapbuffer.height = global_bitmap_buffer.height;
	gamebitmapbuffer.memory = global_bitmap_buffer.memory;

	game_input gameinput = {};
	game_controller *keyboard_controller = &gameinput.controllers[0];

	r32 fps = 30.0f;
	r32 blit_interval = 1000.0f / fps;

	/*TODO*/
	LARGE_INTEGER performance_frequency;
	QueryPerformanceFrequency(&performance_frequency);
	global_performance_frequency = performance_frequency.QuadPart;
	/**/

	i64 frame_start_time = win32performancecounter();
	i64 frame_end_time;
	r32 frame_elapsed_time;

	global_app_running = true;
	while(global_app_running)
	{
		win32handleevents(keyboard_controller);
		gameupdateandrender(&gamebitmapbuffer, &gamememory, &gameinput);

		frame_end_time = win32performancecounter();
		frame_elapsed_time = win32gettimeinmillis(frame_start_time, frame_end_time);

		if(frame_elapsed_time < blit_interval)
		{
			Sleep(blit_interval-frame_elapsed_time);
		}

		HDC device_context = GetDC(window);
		{
			win32_window_dimension dimension = win32windowdimension(window);
			win32blitbitmap(device_context, &global_bitmap_buffer, dimension.width, dimension.height);
		}
		ReleaseDC(window, device_context);
		frame_start_time = win32performancecounter();
	}

	return 0;
}

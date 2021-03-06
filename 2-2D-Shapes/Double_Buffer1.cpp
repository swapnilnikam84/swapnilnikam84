// Headers   // NEW CODE 
#include<windows.h>
#include<stdio.h>
#include"Resource.h"
#include<gl/GL.h>
#include<gl/GLU.h>
//#include<math.h>
#pragma comment(lib, "Opengl32.lib")		//lib-import library
#pragma comment(lib,"GLU32.lib")
#define Win_Width 800
#define Win_Height 600
//#define GL_PI 3.1415f

											// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
FILE* gpFile = NULL;


// global Variable declarations
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool gbFullscreen = false;
bool gbActiveWindow = false;
bool Bdone = false;
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;



// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function declarations 
	void Initialize(void);
	void Display(void);


	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");
	bool BDone = false;

	if (fopen_s(&gpFile, "RADLog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Can Not Open Desired File"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "LOG File Successfully Created.Program Started SuccessFully");
	}
	// code
// intialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));


	// register above class
	RegisterClassEx(&wndclass);

	//create window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Swapnil Nikam"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		100,
		100,
		Win_Width,
		Win_Height,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;
	Initialize();
	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Message loop // Game loop
	while (BDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				BDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		else
		{
			if (gbActiveWindow == true)
			{
				//HERE YOU SHOULD CALL UPDATE FUNCTION FOR OPENGL RENDERING.........
				//HERE YOU SHOULD CALL DISPLAY FUNCTION FOR OPENGL RENDERING...........
				Display();
			}
		}

	}
	return((int)msg.wParam);

}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	// local function declaration
	void ToggleFullScreen(void);
	void Resize(int, int);
	void unintialize(void);
	//void Display(void);
// local variable declarations

	TCHAR str[] = TEXT("Hello OpenGL!!!");

	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_SIZE:
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;
		//case WM_PAINT:
			//Display();
			//break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		case 0x46:										  // Capital 'F' hexadecimal value of 70
		case 0x66:										  // Small 'f' hexadecimal value of 102
			ToggleFullScreen();
			break;
		default:
			break;
		}
		break;
		//case WM_CLOSE:
			//DestroyWindow(hwnd);
			//break;
	case WM_DESTROY:
		unintialize();
		PostQuitMessage(0);
		break;

		//}//break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	//function declaration
	void Resize(int, int);

	// local Variable declaration
	MONITORINFO mi{ sizeof(MONITORINFO) };

	// code
	if (gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);

			}
		}
		ShowCursor(FALSE);
		gbFullscreen = true;
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullscreen = false;
	}

}
void Initialize(void)
{
	//function declaration
	void Resize(int, int);

	//variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ghdc = GetDC(ghwnd);
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "ChoosePixelFormate Function Failed");
		DestroyWindow(ghwnd);

	}
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat Function Failed");
		DestroyWindow(ghwnd);
	}
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext Function Failed");
		DestroyWindow(ghwnd);

	}
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, ("wglMakeCurrent Function Failed"));
		DestroyWindow(ghwnd);
	}
	//set clearcolor

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Resize(Win_Width, Win_Height);		// warmup call to resize
}
void Resize(int width, int height)
{
	//code
	if (height == 0)
	{
		height = 1;

	}
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gluPerspective(45.0f,(GLfloat)width / (GLfloat)height,0.1f,100.0f);


}

void Display(void)
{
	//code

	//static GLfloat angle = 0.5f;        // spnning triangle
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);			 //for Perspective
	glLoadIdentity();					 //for Perspective
	
	glTranslatef(-1.0f,0.0f,-3.0f);		  //for Perspective i.e -6.0f

	//glRotatef(angle, 0.0f, 1.0f, 0.0f);        // spnning triangle

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
	//angle = angle + 0.5f;            // spnning triangle

	glMatrixMode(GL_MODELVIEW);			 //for Perspective
	glLoadIdentity();					 //for Perspective

	glTranslatef(1.0f, 0.0f, -3.0f);		  //for Perspective i.e -6.0f

	//glRotatef(angle, 1.0f, 0.0f, 0.0f);        // spnning triangle

	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glEnd();

	SwapBuffers(ghdc);				//Comment/Remove Single buffer 


}

void unintialize(void)
{
	//code
	if (gbFullscreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);

	}
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;

	}
	if (gpFile)
	{
		fprintf(gpFile, "LOG File Successfully Closed.Program Ended SuccessFully");
		fclose(gpFile);
		gpFile = NULL;
		//return(0);
	}
}










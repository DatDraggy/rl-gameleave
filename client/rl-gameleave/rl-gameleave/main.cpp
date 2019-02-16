#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include <cstdio>
#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

/*variables*/
UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("Snoopy.. \n Kicks Ass!");
char szClassName[] = "Tray Demo";



/*procedures  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void minimize();
void restore();
void InitNotifyIconData();


int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	/* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */
	WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
	wincl.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	wincl.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	Hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		szClassName,       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		544,                 /* The programs width */
		375,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);
	/*Initialize the NOTIFYICONDATA structure only once*/
	InitNotifyIconData();
	/* Make the window visible on the screen */
	ShowWindow(Hwnd, nCmdShow);

	if (RegisterHotKey(
		Hwnd,
		1,
		MOD_ALT | MOD_NOREPEAT,
		0x58))  //0x42 is 'b'
	{
		OutputDebugString(_T("Hotkey 'ALT+b' registered, using MOD_NOREPEAT flag\n"));
	}

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Translate virtual-key messages into character messages */
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		DispatchMessage(&messages);
	}

	return messages.wParam;
}

/*int SendKeys(const std::string & msg) {
	int taille = msg.size();

	std::vector<INPUT> inputs(taille);

	for (int i = 0; i < taille; ++i)
	{
		INPUT &input = inputs[i];
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = 0;
		input.ki.wScan = msg.at(i);
		input.ki.dwFlags = KEYEVENTF_UNICODE;
		input.ki.time = 0;
		input.ki.dwExtraInfo = GetMessageExtraInfo();

	}
	SendInput(inputs.size(), &inputs[0], sizeof(INPUT));
	return 1;
}*/

int SendKeys(const std::string & msg) {

	for (int i = 0; i < msg.size(); ++i)
	{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = 0;
		input.ki.wScan = msg.at(i);
		input.ki.dwFlags = KEYEVENTF_UNICODE;
		input.ki.time = 0;
		input.ki.dwExtraInfo = GetMessageExtraInfo();

		SendInput(1, &input, sizeof(INPUT));

		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
	return 1;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (message == WM_TASKBAR && !IsWindowVisible(Hwnd))
	{
		minimize();
		return 0;
	}

	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Test Text");
	TCHAR hotkeytext[] = _T("Hotkey Test");
	INPUT ip;

	switch (message)                  /* handle the messages */
	{
	case WM_ACTIVATE:
		Shell_NotifyIcon(NIM_ADD, &notifyIconData);
		break;
	case WM_CREATE:

		ShowWindow(Hwnd, SW_HIDE);
		Hmenu = CreatePopupMenu();
		AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));

		break;
	case WM_HOTKEY:
		Sleep(1000);
		while(GetAsyncKeyState(18))
		{
			
		}

		//Wait till ALT not pressed anymore
		SendKeys("test string double char gg oo ee <<");
		/*
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0; // hardware scan code for key
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		// Press the "A" key
		ip.ki.wVk = 0x41; // virtual-key code for the "a" key
		ip.ki.dwFlags = 0; // 0 for key press

		//Send the press
		SendInput(1, &ip, sizeof(INPUT));

		//Prepare a keyup event
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));*/
		OutputDebugString(_T("Test"));
		break;
	case WM_PAINT:
		hdc = BeginPaint(Hwnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"
		// in the top left corner.
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		// End application-specific layout section.

		EndPaint(Hwnd, &ps);
		break;
	case WM_SYSCOMMAND:
		/*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter
		are used internally by the system. To obtain the correct result when testing the value of wParam,
		an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/

		switch (wParam & 0xFFF0)
		{
		case SC_MINIMIZE:
		case SC_CLOSE:
			minimize();
			return 0;
			break;
		}
		break;


		// Our user defined WM_SYSICON message.
	case WM_SYSICON:
	{

		switch (wParam)
		{
		case ID_TRAY_APP_ICON:
			SetForegroundWindow(Hwnd);

			break;
		}


		if (lParam == WM_LBUTTONUP)
		{

			restore();
		}
		else if (lParam == WM_RBUTTONDOWN)
		{
			// Get current mouse position.
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(Hwnd);

			// TrackPopupMenu blocks the app until TrackPopupMenu returns

			UINT clicked = TrackPopupMenu(Hmenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hwnd, NULL);



			SendMessage(hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
			if (clicked == ID_TRAY_EXIT)
			{
				// quit the application.
				Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
				PostQuitMessage(0);
			}
		}
	}
	break;

	// intercept the hittest message..
	case WM_NCHITTEST:
	{
		UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
		if (uHitTest == HTCLIENT)
			return HTCAPTION;
		else
			return uHitTest;
	}

	case WM_CLOSE:

		minimize();
		return 0;
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


void minimize()
{
	// hide the main window
	ShowWindow(Hwnd, SW_HIDE);
}


void restore()
{
	ShowWindow(Hwnd, SW_SHOW);
}

void InitNotifyIconData()
{
	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = Hwnd;
	notifyIconData.uID = ID_TRAY_APP_ICON;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
	notifyIconData.hIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
	strncpy_s(notifyIconData.szTip, szTIP, sizeof(szTIP));
}

#include <Windows.h>
#include <tchar.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment( lib, "comctl32.lib")  
//#include "resource.h"


#define COPY 181
#define INSERT 182
#define QUEST 183
#define FONTA 200
#define FONTB 201
#define FONTC 202
#define FONTD 203
#define FONTE 204
#define FONTSIZEA 300
#define FONTSIZEB 301
#define FONTSIZEC 302
#define FONTSIZED 303
#define FONTSIZEF 304

TBBUTTON tbb[] = {
					 { 1, 181,  TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
					 { 2, 182,  TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
					 {11, 183,  TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);

WNDPROC OldHandler;

TCHAR WinName[] = _T("MainFrame");
TCHAR EditName[] = _T("Edit");

void ToClipboard(TCHAR* text)
{
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		TCHAR* clip_data = (TCHAR*)(GlobalAlloc(GMEM_FIXED, MAX_PATH));
		lstrcpy(clip_data, text);
		SetClipboardData(CF_UNICODETEXT, (HANDLE)(clip_data));
		LCID* lcid = (DWORD*)(GlobalAlloc(GMEM_FIXED, sizeof(clip_data)));
		*lcid = MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_NEUTRAL), SORT_DEFAULT);
		SetClipboardData(CF_LOCALE, (HANDLE)(lcid));
		CloseClipboard();
	}
}

void FromClipboard(HWND hWnd)
{
	if (OpenClipboard(0))
	{
		static HGLOBAL hglbClipBuf;
		LPWSTR lpClipBuf;
		TCHAR text0[MAXCHAR], text[MAXCHAR];
		hglbClipBuf = GetClipboardData(CF_UNICODETEXT);
		if (hglbClipBuf != NULL)
		{
			lpClipBuf = (LPWSTR)GlobalLock(hglbClipBuf);
			if (lpClipBuf != NULL)
			{
				lstrcpy(text, lpClipBuf);
				SendMessage(hWnd, WM_GETTEXT, MAXCHAR, (LPARAM)text0);
				SendMessage(hWnd, WM_SETTEXT, 1, (LPARAM)lstrcat(text0, text));
			}
			GlobalUnlock(hglbClipBuf);
		}
		CloseClipboard();
	}
}

int WINAPI _tWinMain(HINSTANCE This,
	HINSTANCE Prev,
	LPTSTR cmd,
	int mode)
{
	setlocale(0, "ru");
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;

	// Îïðåäåëåíèå êëàññà îêíà 
	wc.hInstance = This;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(0x00FFFFFF);

	if (!RegisterClass(&wc)) return 0;

	hWnd = CreateWindow(WinName,
		_T("Font Program"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		HWND_DESKTOP,
		NULL,
		This,
		NULL);

	ShowWindow(hWnd, mode);

	// Öèêë îáðàáîòêè ñîîáùåíèé 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
static HWND StatusWindow;
static int font_size = 16;
static std::wstring  font_name = { L"Font name: Broadway | font_size: " };
static std::wstring tmp;
LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case FONTSIZEA:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(16, 6, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, lFont.lfFaceName);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_size = 16;
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTSIZEB:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(20, 7, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, lFont.lfFaceName);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_size = 20;
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTSIZEC:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(28, 9, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, lFont.lfFaceName);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_size = 28;
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTSIZED:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(36, 13, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, lFont.lfFaceName);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_size = 36;
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTSIZEF:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(100, 30, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, lFont.lfFaceName);
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_size = 100;
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTA:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(lFont.lfHeight, lFont.lfWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Broadway");
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_name = L"Font name: Broadway | font_size: ";
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTB:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(lFont.lfHeight, lFont.lfWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Times New Roman");
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_name = L"Font name: Times new Roman | font_size: ";
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTC:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(lFont.lfHeight, lFont.lfWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Calibri");
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_name = L"Font name: Calibri | font_size: ";
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTD:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(lFont.lfHeight, lFont.lfWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Impact");
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_name = L"Font name: Impact | font_size: ";
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case FONTE:
		{
			SendMessage(hWnd, WM_GETFONT, (WPARAM)hFont, NULL);
			LOGFONT lFont;
			GetObject(hFont, sizeof(LOGFONT), &lFont);
			hFont = CreateFont(lFont.lfHeight, lFont.lfWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Cyberpunk");
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, NULL);
			font_name = L"Font name: Cyberpunk | font_size: ";
			tmp = font_name + std::to_wstring(font_size);
			SendMessage(StatusWindow, SB_SETTEXT, 0, (LPARAM)tmp.c_str());
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		default:
			return CallWindowProc((WNDPROC)OldHandler, hWnd, message, wParam, lParam);
		}
	}
	case WM_RBUTTONDOWN:
	{
		HMENU hMenu = CreatePopupMenu();
		HMENU hSubFontMenu = CreatePopupMenu();
		HMENU hSubFontSizeMenu = CreatePopupMenu();
		RECT rc;
		GetWindowRect(hWnd, &rc);
		int x, y;
		x = LOWORD(lParam) + rc.left;
		y = HIWORD(lParam) + rc.top;
		if (hMenu) DestroyMenu(hMenu);
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MFT_STRING | MF_POPUP, (UINT_PTR)hSubFontMenu, L"&Font");
		AppendMenu(hSubFontMenu, MFT_STRING | MF_POPUP, FONTA, L"&Broadway");
		AppendMenu(hSubFontMenu, MFT_STRING | MF_POPUP, FONTB, L"&Times New Roman");
		AppendMenu(hSubFontMenu, MFT_STRING | MF_POPUP, FONTC, L"&Calibri");
		AppendMenu(hSubFontMenu, MFT_STRING | MF_POPUP, FONTD, L"&Impact");
		AppendMenu(hSubFontMenu, MFT_STRING | MF_POPUP, FONTE, L"&Cyberpunk");
		AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MFT_STRING | MF_POPUP, (UINT_PTR)hSubFontSizeMenu, L"&Size");
		AppendMenu(hSubFontSizeMenu, MFT_STRING | MF_POPUP, FONTSIZEA, L"&16");
		AppendMenu(hSubFontSizeMenu, MFT_STRING | MF_POPUP, FONTSIZEB, L"&20");
		AppendMenu(hSubFontSizeMenu, MFT_STRING | MF_POPUP, FONTSIZEC, L"&28");
		AppendMenu(hSubFontSizeMenu, MFT_STRING | MF_POPUP, FONTSIZED, L"&36");
		AppendMenu(hSubFontSizeMenu, MFT_STRING | MF_POPUP, FONTSIZEF, L"&100");
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON |
			TPM_TOPALIGN |
			TPM_LEFTALIGN,
			x,
			y, 0, hWnd, NULL);
		return 0;
	}
	default:
		return CallWindowProc((WNDPROC)OldHandler, hWnd, message, wParam, lParam);
	}
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit, hToolBar;
	switch (message)		 
	{
	case WM_CREATE:
	{
		HMENU hMenuBar = CreateMenu(), hFile = CreateMenu();
		StatusWindow = CreateStatusWindow(
			WS_CHILD | WS_VISIBLE,
			L"",
			hWnd, //родитель, его получаем при вызове hwnd= CreateWindow();
			1000);
		
		SetMenu(hWnd, hMenuBar);

		hEdit = CreateWindow(WC_EDIT, NULL, WS_VSCROLL | WS_HSCROLL | WS_CHILD | WS_VISIBLE | SBS_SIZEBOXBOTTOMRIGHTALIGN | SBS_SIZEBOX,
			0, 0, 100, 100, hWnd, NULL, HINST_COMMCTRL, 0); 
		OldHandler = (WNDPROC)GetClassLong(hEdit, GCL_WNDPROC);
		SetWindowLong(hEdit, GWL_WNDPROC, LONG(EditProc));
		hToolBar = CreateToolbarEx(hWnd, WS_CHILD | WS_BORDER | WS_VISIBLE | TBSTYLE_TOOLTIPS | CCS_ADJUSTABLE,
			180,
			3,
			HINST_COMMCTRL,
			IDB_STD_SMALL_COLOR,
			(LPCTBBUTTON)& tbb,
			3,
			0, 0,
			16, 16,
			sizeof(TBBUTTON));
	}
	break;
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		GetWindowRect(hWnd, &rc);
		SetWindowPos(hEdit, NULL, 0, 29, rc.right - rc.left - 15, rc.bottom - rc.top - 89, SWP_NOZORDER);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case COPY:
		{
			TCHAR text[MAXCHAR];
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)text);
			ToClipboard(text);
		}
		break;
		case INSERT:
			FromClipboard(hEdit);
			break;
		case QUEST:
			MessageBox(NULL, TEXT("Use right mouse button to change fonts and sizes"), (LPWSTR)TEXT("INFO"), MB_OK);
			break;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT PaintStruct;
		HDC hDC = BeginPaint(hWnd, &PaintStruct);
		HBRUSH hBrush;
		EndPaint(hWnd, &PaintStruct);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

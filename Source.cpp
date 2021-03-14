
#include <Windows.h>
#include <tchar.h>
#include <string>
#define NORMAL_WINDOW_HEIGHT 720
#define NORMAL_WINDOW_WIDTH 1280
#define EDIT_1 1
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

void PrintText(int fHeight, int fWidth, LPCWSTR LfName,
	std::wstring fText, HDC& hdc, HFONT& fontObj, HWND& hWnd) {
	HFONT newFont = CreateFont(36, 20, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT(fName));
	HFONT oldFont = (HFONT)SelectObject(hdc, newFont);
	RECT Rect;
	GetClientRect(hWnd, &Rect);
	DrawText(hdc, fText.c_str(), -1, &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

struct Color {
	int r = 0;
	int g = 0;
	int b = 0;
};

int WINAPI _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;

	wc.hInstance = This;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClass(&wc)) return 0;

	hWnd = CreateWindow(WinName, _T("Программа Font"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		NORMAL_WINDOW_WIDTH, NORMAL_WINDOW_HEIGHT,
		HWND_DESKTOP, NULL, This, NULL);

	ShowWindow(hWnd, mode);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

static std::wstring text = L"";
static std::wstring fontName = L"Impact";
static wchar_t* c = new wchar_t[20];
static wchar_t* c2 = new wchar_t[20];
#define cb1_id 2
HDC hdc;
HFONT font;
PAINTSTRUCT ps;
HWND CB;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		HWND text_enter;
		text_enter = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, NORMAL_WINDOW_WIDTH - 300, 200, 200, 20, hWnd, (HMENU)(EDIT_1), NULL, NULL);
		
		CB = CreateWindow(L"combobox", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 210, 10, 200, 150, hWnd, (HMENU)cb1_id, NULL, NULL);
		SendMessage(CB, CB_ADDSTRING, 0, (LPARAM)L"Arial");
		SendMessage(CB, CB_ADDSTRING, 0, (LPARAM)L"Impact");
		SendMessage(CB, CB_ADDSTRING, 0, (LPARAM)L"Calibri");
		SendMessage(CB, CB_ADDSTRING, 0, (LPARAM)L"Century");
		SendMessage(CB, CB_ADDSTRING, 0, (LPARAM)L"Comic Sans MS");
		SendMessage(CB, CB_ADDSTRING, 0, (LPARAM)L"Alef");
		SendMessage(CB, CB_SETCURSEL, 1, 0);
		return 0;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		PrintText(36, 20, (LPCWSTR)fontName.c_str(), text, hdc, font, hWnd);
		return 0;
		EndPaint(hWnd, &ps);
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_TIMER:
	{
		return 0;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == EDIT_1) {
			int nc = HIWORD(wParam);
			if (nc == EN_UPDATE) {
				GetWindowText((HWND)lParam, c, 20);
				text = c;
				InvalidateRect(hWnd, NULL, 1);
			}
		}
		if (LOWORD(wParam) == cb1_id) {
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int id = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				wchar_t buffer[20];
				SendMessage((HWND)lParam, CB_GETLBTEXT, id, (LPARAM)buffer);
				fontName = buffer;
				InvalidateRect(hWnd, NULL, 1);
			}
		}
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		InvalidateRect(hWnd, NULL, 1);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
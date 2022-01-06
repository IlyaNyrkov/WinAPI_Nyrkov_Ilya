#include <Windows.h>
#include <tchar.h>
#include <string>
#define TIMER1 1 //square
#define TIMER2 2 //text

#define NORMAL_WINDOW_HEIGHT 720
#define NORMAL_WINDOW_WIDTH 1280
#define BUTTN_1 1
#define EDIT_1 2
//Первый поток выводит в левую половину окна фигуру
//плавно перемещающуюся по вертикали от верхнего края 
//окна до нижнего, скачком возвращается назад и повторяет движение вниз.
//В каждом шаге y - координату фигуры изменяйте на 1 пиксел.
//-------------------------------------------
//Второй поток выводит в правую половину окна вводимый текст.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI FirstThread(LPVOID param);
DWORD WINAPI SecondThread(LPVOID param);
HANDLE MutexHwnd = CreateMutex(0, 0, 0);

TCHAR WinName[] = _T("MainFrame");

struct Color {
	int r = 0;
	int g = 0;
	int b = 0;
};

void PrintText(HDC hdc, const std::wstring text, int x, int y) {
	TextOut(hdc, x, y, text.c_str(), text.size());
}

void PrintFigure(int x_pos, int y_pos, int x_size, int y_size,
	HWND hWnd, Color color, HBRUSH& brush, HDC& hdc) {
	SelectObject(hdc, brush);
	Rectangle(hdc, x_pos, y_pos, x_size, y_size);
}

int WINAPI _tWinMain(HINSTANCE This,		 // Дескриптор текущего приложения 
	HINSTANCE Prev, 	// В современных системах всегда 0 
	LPTSTR cmd, 		// Командная строка 
	int mode) 		// Режим отображения окна
{
	HWND hWnd;		// Дескриптор главного окна программы 
	MSG msg; 		// Структура для хранения сообщения 
	WNDCLASS wc; 	// Класс окна
	// Определение класса окна 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 				// Имя класса окна 
	wc.lpfnWndProc = WndProc; 					// Функция окна 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// Стиль окна 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 		// Стандартная иконка 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// Стандартный курсор 
	wc.lpszMenuName = NULL; 					// Нет меню 
	wc.cbClsExtra = 0; 						// Нет дополнительных данных класса 
	wc.cbWndExtra = 0; 						// Нет дополнительных данных окна 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); 	// Заполнение окна белым цветом 


	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна 
	hWnd = CreateWindow(WinName,			// Имя класса окна 
		_T("Каркас Windows-приложения"), 		// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		NORMAL_WINDOW_WIDTH, 				// width 
		NORMAL_WINDOW_HEIGHT, 				// Height 
		HWND_DESKTOP, 				// Дескриптор родительского окна 
		NULL, 						// Нет меню 
		This, 						// Дескриптор приложения 
		NULL); 					// Дополнительной информации нет 

	ShowWindow(hWnd, mode); 

	// Цикл обработки сообщений 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 	
		DispatchMessage(&msg); 		
	}
	return 0;
}


static std::wstring result = L"text";
static bool is_text_enter = true;
static bool is_square_moving = true;
static int y_pos = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)		 // Обработчик сообщений
	{
	HINSTANCE hInst;
	static HWND hEdt1;
	static wchar_t c[21];
	static bool is_working;
	is_working = false;

	case WM_PAINT:
	{

		if (is_text_enter) {
			is_text_enter = false;
			CreateThread(NULL, 0, SecondThread, hWnd, 0, NULL);
		}
		if (is_square_moving) {
			is_square_moving = false;
			CreateThread(NULL, 0, FirstThread, hWnd, 0, NULL);
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_TIMER:
	{
		if (LOWORD(wParam) == TIMER1) {
			if (NORMAL_WINDOW_HEIGHT - 130 <= y_pos) {
				y_pos = 0;
			}
			y_pos++;
			is_square_moving = true;
			RECT rectangle;
			rectangle.top = 0;
			rectangle.right = 100;
			rectangle.bottom = NORMAL_WINDOW_HEIGHT;
			rectangle.left = 0;
			RECT* lpRect = &rectangle;
			InvalidateRect(hWnd, lpRect, TRUE);
		}
		else if (LOWORD(wParam) == TIMER2) {
			is_text_enter = true;
			InvalidateRect(hWnd, NULL, 0);
		}
		return 0;
	}
	case WM_CREATE:
	{
		HWND text_enter;
		text_enter = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT, NORMAL_WINDOW_WIDTH - 300, 200, 200, 20, hWnd, (HMENU)(EDIT_1), NULL, NULL);
		SetTimer(hWnd, TIMER1, 1, NULL);
		SetTimer(hWnd, TIMER2, 60, NULL);
		return 0;
	}
	case WM_COMMAND:
	{
		
		GetWindowText((HWND)lParam, c, 21);
		result = c;
		return 0;
	}
	case WM_SIZE:
	{
		is_square_moving = true;
		is_text_enter = true;
		return 0;
	}
	default: 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

DWORD __stdcall FirstThread(LPVOID param) {
	HWND hWnd = static_cast<HWND>(param);
	PAINTSTRUCT ps;
	Color figure_color = { 0, 255 };
	HBRUSH hbrush = CreateSolidBrush(RGB(figure_color.r, figure_color.g, figure_color.b));
	WaitForSingleObject(MutexHwnd, INFINITY);
	HDC hdc = BeginPaint(hWnd, &ps);
	PrintFigure(0, y_pos, 100, 100 + y_pos, hWnd, figure_color, hbrush, hdc);
	EndPaint(hWnd, &ps);
	ReleaseMutex(MutexHwnd);
	return 0;
}

DWORD __stdcall SecondThread(LPVOID param) {
	HWND hWnd = static_cast<HWND>(param);
	PAINTSTRUCT ps;
	WaitForSingleObject(MutexHwnd, INFINITY);
	HDC hdc = BeginPaint(hWnd, &ps);
	TextOut(hdc, 200, 200, result.c_str(),result.size());
	EndPaint(hWnd, &ps);
	ReleaseMutex(MutexHwnd);
	return 0;
}

#include <Windows.h>
#include <tchar.h>
#include <ctime>
#include <vector>
#define TIMER1 1
#define TIMER2 2
//задание: Три вложенных друг в друга квадрата с размерами сторон 50, 100 и 150 пикселов,
//соприкасающиеся левыми верхними углами. По щелчку левой кнопки мыши цвет i-го квадрата
//становиться цветом (i+1)-го. Щелчок мыши (правая клавиша) - Закрыть окно через 15 сек


void PrintSquare(int size, std::vector<int> colors, HDC & hdc, HBRUSH& Hbrush) {
	Hbrush = CreateSolidBrush(RGB(colors[0], colors[1], colors[2]));
	SelectObject(hdc, Hbrush);
	Rectangle(hdc, 0, 0, size, size);
}


void MoveElems(std::vector<std::vector<int>>& colors) {
	std::vector<std::vector<int>> tmp = { colors[2], colors[0], colors[1] };
	colors = tmp;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


TCHAR WinName[] = _T("MainFrame");

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
		_T("Первая лаба по шиндовс от Линуса Торвальдса"), 		// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		CW_USEDEFAULT, 				// width 
		CW_USEDEFAULT, 				// Height 
		HWND_DESKTOP, 				// Дескриптор родительского окна 
		NULL, 						// Нет меню 
		This, 						// Дескриптор приложения 
		NULL); 					// Дополнительной информации нет 

	ShowWindow(hWnd, mode); 				// Показать окно
	UpdateWindow(hWnd);
	// Цикл обработки сообщений 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// Функция трансляции кодов нажатой клавиши 
		DispatchMessage(&msg); 		// Посылает сообщение функции WndProc() 
	}
	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::vector<std::vector<int>> colors = { {0, 0, 255}, {255, 0, 0}, {0, 255, 0} };
	switch (message)		 // Обработчик сообщений
	{
		PAINTSTRUCT ps;
		HDC hdc;
		HBRUSH Hbrush;
		int square_size;
	case WM_CREATE:
	{
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0; 			// Завершение программы 
	}
	case WM_PAINT:
	{
		UpdateWindow(hWnd);
		hdc = BeginPaint(hWnd, &ps);
		square_size = 50;
		PrintSquare(square_size * 3, colors[0], hdc, Hbrush);
		PrintSquare(square_size * 2, colors[1], hdc, Hbrush);
		PrintSquare(square_size * 1, colors[2], hdc, Hbrush);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		SetTimer(hWnd, TIMER1, 500, NULL);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		KillTimer(hWnd, TIMER1);
		return 0;
	}
	case WM_TIMER:
	{
		if (wParam == TIMER1) {
			MoveElems(colors);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	}
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
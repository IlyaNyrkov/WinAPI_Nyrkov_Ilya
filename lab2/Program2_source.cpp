#include <Windows.h>
#include <tchar.h>
#include <string>
#include <cstdio>
#pragma warning (disable : 4996)
//лаба 2, задание: 
//Сформировать два приложения, которые открывают по одному окну.
//В окне 1 по щелчку левой клавиши мыши: при помощи FindWindow() найти дескриптор окна 2. 
//Выдать сообщение об этом. Если операция неудачная закрыть приложение 1.
//При помощи функции SendMessage()  и поля WPARAM  передать свой дескриптор второму окну.
//Выдать сообщение об этом. В окне 2: при получении сообщения WM_USER+1 (левая клавиша)
//выдать список запущенных приложений. При нажатии правой клавиши в первом приложении - 
//сообщение о получении.


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("Part_2");

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
		_T("Вторая лаба от Линуса торвальдса"), 		// Заголовок окна 
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
	switch (message)		 // Обработчик сообщений
	{
		static HWND hwnd_2;
		HDC hdc;
		PAINTSTRUCT Ps;
	case WM_LBUTTONDOWN: {
		if (hwnd_2 != NULL) {
			SendMessage(hwnd_2, WM_USER, wParam, NULL);
		}
		else {
			MessageBox(hWnd, L"Cant'send apps list, no 1st window handle", L"Error", NULL);
		}
		return 0;
	}
	case WM_PAINT :
	{
		if (hwnd_2 != NULL) {
			hdc = BeginPaint(hWnd, &Ps);
			wchar_t buffer[10];
			_stprintf(buffer, _T("%p"), hwnd_2);
			TextOut(hdc, 100, 100, buffer, 9);
			EndPaint(hWnd, &Ps);
		}
		return 0;
	}
	case WM_USER + 1:
	{
		//MessageBox(hWnd, L"Handle accepted", L"Message Acceptance", NULL);
		hwnd_2 = (HWND)wParam;
		InvalidateRect(hWnd, NULL, NULL);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}// Завершение программы 
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

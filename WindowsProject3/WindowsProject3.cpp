// WindowsProject3.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject3.h"
#include "math.h"
#define MAX_LOADSTRING 100
#define WIDTH 3
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
 typedef struct  {
    int x;
    int y;
    int end_x;
    int end_y;
}Point;
 Point point;
 int line_recording = 0;
 int n = 0;
 void drawLevy(int x1, int y1, int x2, int y2, int n, HDC hdc) {
     if (n == 0) {
         HPEN hPen = CreatePen(PS_SOLID, WIDTH, RGB(0, 0, 0));
         SelectObject(hdc, hPen);
         MoveToEx(hdc, x1, y1, NULL);
         LineTo(hdc, x2, y2);
         DeleteObject(hPen);        
     }
     else {
         int xx = (x1 + x2) / 2 - (y2 - y1) / 2;
         int yy = (y1 + y2) / 2 + (x2 - x1) / 2;
         drawLevy(x1, y1, xx, yy, n - 1, hdc);
         drawLevy(xx, yy, x2, y2, n - 1, hdc);
     }
 }
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
             hdc = BeginPaint(hWnd, &ps);
             if (line_recording != 2) {
                 HPEN hPen = CreatePen(PS_SOLID, WIDTH, RGB(0, 0, 0));
                 SelectObject(hdc, hPen);
                 MoveToEx(hdc, point.x, point.y, NULL);
                 LineTo(hdc, point.end_x, point.end_y);
                 DeleteObject(hPen);
             }
             else if (line_recording == 2) {
                 drawLevy(point.x, point.y, point.end_x, point.end_y,n, hdc);
             }


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
    {
        if ( line_recording == 1) {
            point.end_x = LOWORD(lParam);
            point.end_y = HIWORD(lParam);
            line_recording = 2;
        }
        else
        {
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);           
            line_recording = 1;
            n = 0;
        }

    }
    break;
    case WM_MOUSEMOVE:
    {
        if (line_recording == 1)
        {
            point.end_x = LOWORD(lParam);
            point.end_y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
        }

    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_LEFT: {
            if (n > 0) {
                n--;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
        case VK_RIGHT: {
            n++;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_ESCAPE: {
            exit(0);
            break;
        }
        }
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

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

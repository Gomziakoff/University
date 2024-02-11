#include <windows.h>
#include "stdafx.h"
#include "lab1.h"
// Глобальные переменные
HWND g_hWnd;
bool g_bWindowHidden = false;

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Точка входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CustomWindowClass";
    RegisterClass(&wc);

    // Создание окна
    g_hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "CustomWindowClass",
        "My Custom Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 200,
        NULL, NULL, hInstance, NULL);

    if (g_hWnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Отображение окна
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// Обработчик оконных сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_QUIT:
        MessageBox(NULL, "Received WM_QUIT message!", "Message", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_LBUTTONDOWN:
        // Переключение видимости окна по нажатию левой клавиши мыши
        if (g_bWindowHidden)
        {
            ShowWindow(g_hWnd, SW_SHOW);
            g_bWindowHidden = false;
        }
        else
        {
            ShowWindow(g_hWnd, SW_HIDE);
            g_bWindowHidden = true;
        }
        break;
    case WM_KEYDOWN:
        // Переключение видимости окна по нажатию клавиши на клавиатуре
        if (g_bWindowHidden && wParam == VK_SPACE)
        {
            ShowWindow(g_hWnd, SW_SHOW);
            g_bWindowHidden = false;
        }
        else if (!g_bWindowHidden && wParam == VK_SPACE)
        {
            ShowWindow(g_hWnd, SW_HIDE);
            g_bWindowHidden = true;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

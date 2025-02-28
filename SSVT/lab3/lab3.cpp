#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Msimg32.lib")  // Подключаем библиотеку для TransparentBlt


LPDIRECTDRAW7 dd = nullptr;
LPDIRECTDRAWSURFACE7 primarySurface = nullptr;
HBITMAP hBitmapBackground = nullptr;
HBITMAP hBitmapOverlay = nullptr;

// Функция загрузки BMP-изображения
HBITMAP LoadBitmapFromFile(LPCWSTR filename) {
    return (HBITMAP)LoadImageW(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

// Оконная процедура
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);

        // Рисуем фон
        if (hBitmapBackground) {
            SelectObject(hdcMem, hBitmapBackground);
            BitBlt(hdc, 0, 0, 800, 600, hdcMem, 0, 0, SRCCOPY);
        }

        // Рисуем изображение поверх
        if (hBitmapOverlay) {
            SelectObject(hdcMem, hBitmapOverlay);
            TransparentBlt(hdc, 0, 0, 800, 600, hdcMem, 0, 0, 800, 600, RGB(0, 0, 0));
        }

        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
        return 0;
    }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectDrawWindow";
    RegisterClass(&wc);

    HWND hWnd = CreateWindowW(L"DirectDrawWindow", L"DirectDraw в окне", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (FAILED(DirectDrawCreateEx(NULL, (void**)&dd, IID_IDirectDraw7, NULL))) {
        MessageBoxW(NULL, L"Ошибка DirectDraw!", L"Ошибка", MB_ICONERROR);
        return 0;
    }

    if (FAILED(dd->SetCooperativeLevel(hWnd, DDSCL_NORMAL))) {
        MessageBoxW(NULL, L"Ошибка SetCooperativeLevel!", L"Ошибка", MB_ICONERROR);
        return 0;
    }

    // Загружаем изображения
    hBitmapBackground = LoadBitmapFromFile(L"1.bmp");
    hBitmapOverlay = LoadBitmapFromFile(L"2.bmp");

    if (!hBitmapBackground || !hBitmapOverlay) {
        MessageBoxW(NULL, L"Ошибка загрузки изображений!", L"Ошибка", MB_ICONERROR);
        return 0;
    }

    InvalidateRect(hWnd, NULL, FALSE); // Перерисовать окно

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


#include <windows.h>
#include <ddraw.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <tchar.h>

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

const int WIDTH = 800; 
const int HEIGHT = 600;
const int STARS = 1000;
const int STAR_LIFETIME = 100;

struct Star {
    int x, y;          
    COLORREF color;    
    int lifetime;    
};

LPDIRECTDRAW7 pDD = nullptr;          
LPDIRECTDRAWSURFACE7 pPrimary = nullptr;
LPDIRECTDRAWSURFACE7 pBackBuffer = nullptr;
std::vector<Star> stars;           

COLORREF RandomColor() {
    return RGB(rand() % 256, rand() % 256, rand() % 256);
}

bool InitDirectDraw(HWND hwnd) {
    HRESULT hr;

    hr = DirectDrawCreateEx(nullptr, (LPVOID*)&pDD, IID_IDirectDraw7, nullptr);
    if (FAILED(hr)) {
        MessageBox(hwnd, TEXT("Ошибка создания DirectDraw!"), TEXT("Ошибка"), MB_ICONERROR);
        return false;
    }

    hr = pDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (FAILED(hr)) {
        MessageBox(hwnd, TEXT("Ошибка установки режима DirectDraw!"), TEXT("Ошибка"), MB_ICONERROR);
        return false;
    }

    hr = pDD->SetDisplayMode(WIDTH, HEIGHT, 32, 0, 0);
    if (FAILED(hr)) {
        MessageBox(hwnd, TEXT("Ошибка установки разрешения!"), TEXT("Ошибка"), MB_ICONERROR);
        return false;
    }

    DDSURFACEDESC2 ddsd = {};
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;

    hr = pDD->CreateSurface(&ddsd, &pPrimary, nullptr);
    if (FAILED(hr)) {
        MessageBox(hwnd, TEXT("Ошибка создания первичной поверхности!"), TEXT("Ошибка"), MB_ICONERROR);
        return false;
    }

    DDSCAPS2 ddscaps = {};
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    hr = pPrimary->GetAttachedSurface(&ddscaps, &pBackBuffer);
    if (FAILED(hr)) {
        MessageBox(hwnd, TEXT("Ошибка получения заднего буфера!"), TEXT("Ошибка"), MB_ICONERROR);
        return false;
    }

    return true;
}

void DrawStars() {
    DDSURFACEDESC2 ddsd = {};
    ddsd.dwSize = sizeof(ddsd);
    pBackBuffer->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr);

    BYTE* buffer = (BYTE*)ddsd.lpSurface;
    int pitch = ddsd.lPitch;

    memset(buffer, 0, pitch * HEIGHT);

    for (const auto& star : stars) {
        BYTE* pixel = buffer + star.y * pitch + star.x * 4;
        *(COLORREF*)pixel = star.color;
    }

    pBackBuffer->Unlock(nullptr);
}

void UpdateStars() {
    for (auto& star : stars) {
        star.lifetime--;
        if (star.lifetime <= 0) {
            star.x = rand() % WIDTH;
            star.y = rand() % HEIGHT;
            star.color = RandomColor();
            star.lifetime = STAR_LIFETIME;
        }
    }
}

void Render(HWND hwnd) {
    if (!pPrimary || !pBackBuffer) return;

    UpdateStars();

    DrawStars();

    pPrimary->Flip(nullptr, DDFLIP_WAIT);
}

void Cleanup() {
    if (pBackBuffer) pBackBuffer->Release();
    if (pPrimary) pPrimary->Release();
    if (pDD) pDD->Release();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < STARS; ++i) {
            stars.push_back({ rand() % WIDTH, rand() % HEIGHT, RandomColor(), rand() % STAR_LIFETIME });
        }

        if (!InitDirectDraw(hwnd)) {
            PostQuitMessage(0);
        }

        SetTimer(hwnd, 1, 16, nullptr); 
        break;

    case WM_TIMER:
        Render(hwnd);
        break;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        Cleanup();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const TCHAR* CLASS_NAME = TEXT("StarWindowClass");

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, TEXT("Звездное небо (DirectDraw)"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
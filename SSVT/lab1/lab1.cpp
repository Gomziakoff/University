#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <dinput.h>
#include <vector>
#include <string>
#include <shellapi.h> // Для ShellExecute

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "shell32.lib") // Для ShellExecute

LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 keyboard;
LPDIRECTINPUTDEVICE8 mouse;
char keyBuffer[256];
DIMOUSESTATE mouseState;
std::vector<std::wstring> devices;
POINT clickPosition = { -1, -1 };
bool messageShown = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateInput(HWND hwnd);
void EnumerateDevices();
void OpenApplication();

bool InitDirectInput(HINSTANCE hInstance, HWND hwnd) {
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, NULL)))
        return false;

    if (FAILED(dinput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL)))
        return false;
    keyboard->SetDataFormat(&c_dfDIKeyboard);
    keyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    keyboard->Acquire();

    if (FAILED(dinput->CreateDevice(GUID_SysMouse, &mouse, NULL)))
        return false;
    mouse->SetDataFormat(&c_dfDIMouse);
    mouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    mouse->Acquire();

    EnumerateDevices();
    return true;
}

BOOL CALLBACK EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
    std::vector<std::wstring>* deviceList = reinterpret_cast<std::vector<std::wstring>*>(pvRef);
    deviceList->push_back(lpddi->tszProductName);
    return DIENUM_CONTINUE;
}

void EnumerateDevices() {
    devices.clear();
    dinput->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCallback, &devices, DIEDFL_ATTACHEDONLY);
}

void OpenApplication() {
    ShellExecute(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOWNORMAL);
}

void UpdateInput(HWND hwnd) {
    HRESULT hr = keyboard->GetDeviceState(sizeof(keyBuffer), (LPVOID)&keyBuffer);
    if (FAILED(hr)) {
        keyboard->Acquire();
        return;
    }

    hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
    if (FAILED(hr)) {
        mouse->Acquire();
        return;
    }

    if ((keyBuffer[DIK_A] & 0x80) &&
        (keyBuffer[DIK_S] & 0x80) &&
        (keyBuffer[DIK_D] & 0x80) &&
        (keyBuffer[DIK_F] & 0x80) &&
        (keyBuffer[DIK_G] & 0x80)) {
        OpenApplication();
    }
}

void Render(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    wchar_t buffer[512];

    if (clickPosition.x != -1 && clickPosition.y != -1) {
        swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"Click at: X=%ld, Y=%ld", clickPosition.x, clickPosition.y);
        TextOut(hdc, 10, 30, buffer, wcslen(buffer));
    }

    int y = 50;
    for (const auto& device : devices) {
        TextOut(hdc, 10, y, device.c_str(), device.size());
        y += 20;
    }

    EndPaint(hwnd, &ps);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectInputWindow";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(L"DirectInputWindow", L"DirectInput Example",
        WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    if (!InitDirectInput(hInstance, hwnd)) return 0;

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UpdateInput(hwnd);
    }

    keyboard->Unacquire();
    keyboard->Release();
    mouse->Unacquire();
    mouse->Release();
    dinput->Release();
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONDOWN:
        clickPosition.x = LOWORD(lParam);
        clickPosition.y = HIWORD(lParam);
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case WM_PAINT:
        Render(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

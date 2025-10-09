#include <windows.h>
#include <ddraw.h>
#include <iostream>

#pragma comment(lib, "ddraw.lib")

// Вручную определяем IID_IDirectDraw7, если он не определен в заголовочных файлах
const IID IID_IDirectDraw7 = { 0x15e65ec0, 0x3b9c, 0x11d2, {0xb9, 0x2f, 0x00, 0x60, 0x97, 0x97, 0xea, 0x5b} };

int main() {
    HRESULT hr;
    LPDIRECTDRAW7 pDD = nullptr;
    LPDIRECTDRAWENUMERATEEXA pDDEnum = nullptr;

    // Инициализация DirectDraw
    hr = DirectDrawCreateEx(nullptr, (LPVOID*)&pDD, IID_IDirectDraw7, nullptr);
    if (FAILED(hr)) {
        std::cerr << "Ошибка инициализации DirectDraw: " << hr << std::endl;
        return 1;
    }

    // Получение информации о драйвере
    DDDEVICEIDENTIFIER2 ddIdentifier;
    hr = pDD->GetDeviceIdentifier(&ddIdentifier, 0);
    if (FAILED(hr)) {
        std::cerr << "Ошибка получения информации о драйвере: " << hr << std::endl;
        pDD->Release();
        return 1;
    }

    // Вывод информации о драйвере
    std::cout << "driver description: " << ddIdentifier.szDescription << std::endl;
    std::cout << "driver name: " << ddIdentifier.szDriver << std::endl;
    std::cout << "driver version: " << HIWORD(ddIdentifier.liDriverVersion.HighPart) << "."
        << LOWORD(ddIdentifier.liDriverVersion.HighPart) << "."
        << HIWORD(ddIdentifier.liDriverVersion.LowPart) << "."
        << LOWORD(ddIdentifier.liDriverVersion.LowPart) << std::endl;

    // Освобождение ресурсов
    pDD->Release();

    return 0;
}
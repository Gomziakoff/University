#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <algorithm> // для std::remove

#pragma comment(lib, "Ws2_32.lib")

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void handle_client(SOCKET server_socket, sockaddr_in& client_addr, int client_len) {
    char buffer[BUFFER_SIZE];
    std::string request;

    // Получаем данные от клиента через recvfrom
    int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&client_addr, &client_len);

    if (bytes_received == SOCKET_ERROR) {
        std::cerr << "Ошибка при получении данных от клиента." << std::endl;
        return;
    }

    buffer[bytes_received] = '\0';
    request = buffer;

    std::cout << "Запрос от клиента: " << request << std::endl;

    if (request.length() >= 5 && request.substr(0, 4) == "load") {
        std::string filename = request.substr(5);
        filename.erase(std::remove(filename.begin(), filename.end(), '\n'), filename.end());
        filename.erase(std::remove(filename.begin(), filename.end(), '\r'), filename.end());

        std::cout << "Запрошенный файл: " << filename << std::endl;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                line += "\n";
                // Отправляем каждую строку клиенту через sendto
                sendto(server_socket, line.c_str(), line.size(), 0, (struct sockaddr*)&client_addr, client_len);
            }
            file.close();
        }
        else {
            std::string error_message = "Файл не найден\n";
            sendto(server_socket, error_message.c_str(), error_message.size(), 0, (struct sockaddr*)&client_addr, client_len);
        }
    }
    else {
        std::string error_message = "Неверный формат запроса\n";
        sendto(server_socket, error_message.c_str(), error_message.size(), 0, (struct sockaddr*)&client_addr, client_len);
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock." << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, 0);  // Создаем UDP сокет
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "UDP сервер запущен. Ожидание запросов..." << std::endl;

    while (true) {
        sockaddr_in client_addr;
        int client_len = sizeof(client_addr);

        // Обрабатываем запрос от клиента
        handle_client(server_socket, client_addr, client_len);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

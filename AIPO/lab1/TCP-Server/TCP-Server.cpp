#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    std::string request;

    while (true) {
        
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received == 0) {
           
            std::cout << "Клиент завершил соединение." << std::endl;
            break;
        }
        else if (bytes_received < 0) {
           
            std::cerr << "Ошибка при получении данных от клиента." << std::endl;
            break;
        }

        buffer[bytes_received] = '\0';
        request += buffer;
  
        if (request.find('\n') != std::string::npos) {
            break;
        }
    }

    std::cout << "Запрос от клиента: " << request << std::endl;

    if (request.length() >= 5 && request.substr(0, 4) == "load") {
        std::string filename = request.substr(5);
        filename.erase(std::remove(filename.begin(), filename.end(), '\n'), filename.end());
        filename.erase(std::remove(filename.begin(), filename.end(), '\r'), filename.end());

        std::cout << filename;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                line += "\n";
                send(client_socket, line.c_str(), line.size(), 0);
            }
            file.close();
        }
        else {
            std::string error_message = "Файл не найден\n";
            send(client_socket, error_message.c_str(), error_message.size(), 0);
            
        }
    }
    else {
        std::string error_message = "Неверный формат запроса\n";
        send(client_socket, error_message.c_str(), error_message.size(), 0);
    }

    closesocket(client_socket);
}



int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock." << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
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

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        std::cerr << "Ошибка при прослушивании." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

    while (true) {
        sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Ошибка при принятии подключения." << std::endl;
            continue;
        }

        std::cout << "Подключен клиент: " << inet_ntoa(client_addr.sin_addr) << std::endl;

        handle_client(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

#include <iostream>
#include <sstream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h> 
#include <chrono>   
#include <ctime>

#pragma comment(lib, "ws2_32.lib") 
#define BUFFER_SIZE 1024

std::string custom_input() {
    std::string input_text;
    char ch;
    bool pgdn_pressed = false;

    while (true) {
        ch = _getch();

        if (ch == 0x00 || ch == 0xE0) {
            ch = _getch();
            if (ch == 0x51) {
                pgdn_pressed = true;
                break;
            }
        }
        else if (ch == '@') {
            break;
        }
        else if (ch == 0x08) {
            if (!input_text.empty()) {
                input_text.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        }
        else {
            input_text += ch;
            std::cout << ch << std::flush;
        }
    }

    if (pgdn_pressed) {
        std::cout << "\nНажата клавиша Page Down.\n";
    }

    std::cout << std::endl;
    return input_text;
}

std::string int_to_string(int number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

void log_event(const std::string& message) {
    std::ofstream log_file("protocol_log.txt", std::ios::app);
    if (log_file.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        char time_str[26];
        ctime_s(time_str, sizeof(time_str), &now_time);

        log_file << time_str << message << std::endl;
    }
}

void send_command(const std::string& server_ip, int port, const std::string& command) {
    WSADATA wsaData;
    SOCKET SendSocket = INVALID_SOCKET;
    struct addrinfo* result = nullptr,
        * ptr = nullptr,
        hints;

    auto start_time = std::chrono::system_clock::now();
    log_event("Начало отправки");

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка WSAStartup: " << WSAGetLastError() << std::endl;
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if (getaddrinfo(server_ip.c_str(), int_to_string(port).c_str(), &hints, &result) != 0) {
        std::cerr << "Ошибка getaddrinfo: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    SendSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (SendSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    std::string message = command + "\n";
    auto send_time = std::chrono::system_clock::now();
    std::time_t send_time_t = std::chrono::system_clock::to_time_t(send_time);

    char send_time_str[26];
    ctime_s(send_time_str, sizeof(send_time_str), &send_time_t);

    log_event("Отправка команды: " + command + " в " + send_time_str);

    int bytes_sent = sendto(SendSocket, message.c_str(), (int)message.length(), 0, result->ai_addr, (int)result->ai_addrlen);
    if (bytes_sent == SOCKET_ERROR) {
        std::cerr << "Ошибка отправки: " << WSAGetLastError() << std::endl;
        closesocket(SendSocket);
        WSACleanup();
        return;
    }

    char recvbuf[BUFFER_SIZE];
    sockaddr_in recv_addr;
    int recv_addr_len = sizeof(recv_addr);
    int bytes_received = recvfrom(SendSocket, recvbuf, BUFFER_SIZE - 1, 0, (struct sockaddr*)&recv_addr, &recv_addr_len);
    if (bytes_received > 0) {
        recvbuf[bytes_received] = '\0';
        auto receive_time = std::chrono::system_clock::now();
        std::time_t receive_time_t = std::chrono::system_clock::to_time_t(receive_time);

        char receive_time_str[26];
        ctime_s(receive_time_str, sizeof(receive_time_str), &receive_time_t);

        log_event("Получено от сервера: " + std::string(recvbuf) + " в " + receive_time_str);
        std::cout << "Получено от сервера: " << recvbuf << std::endl;
    }
    else if (bytes_received == 0) {
        std::cout << "Соединение закрыто" << std::endl;
    }
    else {
        std::cerr << "Ошибка получения: " << WSAGetLastError() << std::endl;
    }

    closesocket(SendSocket);
    WSACleanup();

    auto end_time = std::chrono::system_clock::now();
    log_event("Окончание отправки");
}

int main() {
    std::string server_ip = "127.0.0.1";
    int port = 8080;

    std::cout << "Нажмите Page Down для отправки команды.\n";

    while (true) {
        std::cout << "\nВведите команду для отправки на сервер: ";
        std::string command = custom_input();
        send_command(server_ip, port, command);
    }

    return 0;
}

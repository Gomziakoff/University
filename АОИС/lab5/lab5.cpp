#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <map>
#include <atomic> // Для использования атомарных переменных

std::mutex mtx;//наш мьютекс для решения всех проблем
std::condition_variable cv;//для проверки условия выполнения хода всех игроков
std::map<int, int> playerMove;//ходики
std::map<int, int> playerWins{ {0,0}, { 1,0 }, {2,0}, {3,0} };//победы
bool movesReady = false;//закончили ли ходить игроки

std::atomic<int> threadsFinished(0); // Счетчик завершенных потоков
const int numThreads = 3; // Количество потоков

//мега генератор случайных значений (украл в инете)
int generateMove() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);
    return dis(gen); // Возвращаем случайное число от 1 до 3
}

// игрок делает выбор
void player(int player_num, std::ofstream& file) {
    int move = generateMove();
    std::unique_lock<std::mutex> guard(mtx); // не дает выполняться дальше если другой поток считает кто выйграл другой поток
    cv.wait(guard, [] { return !movesReady; });
    playerMove[player_num] = move;
    std::cout << "Игрок " << player_num << " сделал ход: " << playerMove[player_num] << std::endl;
    file << "Игрок " << player_num << " сделал ход: " << playerMove[player_num] << std::endl;
    ++threadsFinished;
}

//определяем победителя
void winner(std::ofstream& file) {
    std::unique_lock<std::mutex> lock(mtx);//лочим мьютекс
    cv.wait(lock, [] { return movesReady; });//ждем пока скажут можно
    movesReady = false; 
    //1-камень 2-ножницы 3-бумага
    //жесточайшая логика определения победителя
    if ((playerMove[1] == 1 && playerMove[2] == 2 && playerMove[3] == 2) ||
        (playerMove[1] == 2 && playerMove[2] == 3 && playerMove[3] == 3) ||
        (playerMove[1] == 3 && playerMove[2] == 1 && playerMove[3] == 1)) {
        std::cout << "Игрок 1 победил!" << std::endl;
        file << "Игрок 1 победил!" << std::endl;
        playerWins[1]++;
    }
    else if ((playerMove[2] == 1 && playerMove[1] == 2 && playerMove[3] == 2) ||
        (playerMove[2] == 2 && playerMove[1] == 3 && playerMove[3] == 3) ||
        (playerMove[2] == 3 && playerMove[1] == 1 && playerMove[3] == 1)) {
        std::cout << "Игрок 2 победил!" << std::endl;
        file << "Игрок 2 победил!" << std::endl;
        playerWins[2]++;
    }
    else if ((playerMove[3] == 1 && playerMove[2] == 2 && playerMove[1] == 2) ||
        (playerMove[3] == 2 && playerMove[2] == 3 && playerMove[1] == 3) ||
        (playerMove[3] == 3 && playerMove[2] == 1 && playerMove[1] == 1)) {
        std::cout << "Игрок 3 победил!" << std::endl;
        file << "Игрок 3 победил!" << std::endl;
        playerWins[3]++;
    }
    else if ((playerMove[2] == 3 && playerMove[1] == 2 && playerMove[3] == 2) ||
        (playerMove[2] == 1 && playerMove[1] == 3 && playerMove[3] == 3) ||
        (playerMove[2] == 2 && playerMove[1] == 1 && playerMove[3] == 1)) {
        std::cout << "Игроки 1,3 победили!" << std::endl;
        file << "Игроки 1,3 победили!" << std::endl;
        playerWins[1]++;
        playerWins[3]++;
    }
    else if ((playerMove[3] == 3 && playerMove[1] == 2 && playerMove[2] == 2) ||
        (playerMove[3] == 1 && playerMove[1] == 3 && playerMove[2] == 3) ||
        (playerMove[3] == 2 && playerMove[1] == 1 && playerMove[2] == 1)) {
        std::cout << "Игроки 1,2 победили!" << std::endl;
        file << "Игроки 1,2 победили!" << std::endl;
        playerWins[1]++;
        playerWins[2]++;
    }
    else if ((playerMove[1] == 3 && playerMove[3] == 2 && playerMove[2] == 2) ||
        (playerMove[1] == 1 && playerMove[3] == 3 && playerMove[2] == 3) ||
        (playerMove[1] == 2 && playerMove[3] == 1 && playerMove[2] == 1)) {
        std::cout << "Игроки 2,3 победили!" << std::endl;
        file << "Игроки 2,3 победили!" << std::endl;
        playerWins[2]++;
        playerWins[3]++;
    }
    else {
        std::cout << "Ничья!" << std::endl;
        file << "Ничья!" << std::endl;
        playerWins[0]++;
    }
    std::cout <<  std::endl;
    file <<  std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(rand()%10));
}

int main() {
    const int numRounds = 10; // Количество раундов
    std::ofstream file("results.txt");
    if (file.is_open()) {
        for (int i = 0; i < numRounds; ++i) {
            threadsFinished = 0;
            std::thread t1(player, 1, std::ref(file));
            std::thread t2(player, 2, std::ref(file));
            std::thread t3(player, 3, std::ref(file));
            //присоединяем
            t1.join();
            t2.join();
            t3.join();
            
            std::thread t4(winner, std::ref(file));
            
            if (threadsFinished % 3 == 0) {
                movesReady = true;
            }
            cv.notify_one();//говорим можно
            t4.join();
        }
        std::cout << "Игрок 1 выйграл " << playerWins[1] <<" игр\n";
        std::cout << "Игрок 2 выйграл " << playerWins[2] << " игр\n";
        std::cout << "Игрок 3 выйграл " << playerWins[3] << " игр\n";
        std::cout << "Ничейных " << playerWins[0] << " игр\n";
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
        return 1;
    }

    return 0;
}

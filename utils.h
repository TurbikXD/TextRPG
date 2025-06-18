#pragma once
#include <windows.h>
#undef max
#undef min
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <thread>
#include <chrono>

inline void SetColor(int textColor, int bgColor = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

inline void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline std::string getStringInput() {
    std::string input;
    while (true) {
        if (std::getline(std::cin, input)) {
            if (!input.empty()) {
                return input;
            }
        }
        std::cout << "Введите команду: ";
    }
}

inline int getIntInput(int min, int max) {
    int choice = 0;
    
    while (true) {
        std::string input;

        std::ostream::sync_with_stdio(true);

        std::cout.flush();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if (std::getline(std::cin, input)) {
            input.erase(0, input.find_first_not_of(" \t\r\n"));
            input.erase(input.find_last_not_of(" \t\r\n") + 1);
            
            if (input.empty()) {
                std::cout << "Введите число (" << min << "-" << max << "): ";
                std::cout.flush();
                continue;
            }
            
            try {
                choice = std::stoi(input);
                if (choice >= min && choice <= max) {
                    return choice;
                }
                std::cout << "Число должно быть от " << min << " до " << max << ". Введите снова: ";
                std::cout.flush();
            } catch (const std::exception&) {
                std::cout << "Введите корректное число (" << min << "-" << max << "): ";
                std::cout.flush();
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Попробуйте снова: ";
            std::cout.flush();
        }
    }
}

inline char getCharInput() {
    std::string input;
    while (true) {
        if (std::getline(std::cin, input)) {
            if (!input.empty()) {
                return input[0];
            }
        }
        std::cout << "Введите y или n: ";
    }
}

inline void pauseConsole() {
    std::cout << "Нажмите Enter, чтобы продолжить...";
    std::cout.flush();

    std::ostream::sync_with_stdio(true);

    std::string dummy;
    while (true) {
        if (std::getline(std::cin, dummy)) {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
} 
#include <iostream>    // Подключаем библиотеку для ввода-вывода
#include <fstream>     // Подключаем библиотеку для работы с файлами
#include <string>      // Подключаем библиотеку для работы со строками
#include <map>         // Подключаем библиотеку для использования ассоциативных массивов (словари)
#include <vector>      // Подключаем библиотеку для работы с векторами
#include <cmath>       // Подключаем библиотеку для математических функций
#include <algorithm>   // Подключаем библиотеку для алгоритмов (например, сортировки)

// Функция для записи текста в файл
void writeToFile(const std::string& filename, const std::string& text) {
    std::ofstream file(filename); // Открываем файл для записи
    if (file.is_open()) {          // Проверяем, удалось ли открыть файл
        file << text;              // Записываем текст в файл
        file.close();              // Закрываем файл
    } else {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl; // Сообщаем об ошибке
    }
}

// Функция для чтения текста из файла
std::string readFromFile(const std::string& filename) {
    std::ifstream file(filename);  // Открываем файл для чтения
    std::string content;            // Переменная для хранения содержимого файла

    if (file.is_open()) {           // Проверяем, удалось ли открыть файл
        std::string line;           // Переменная для хранения текущей строки
        while (std::getline(file, line)) { // Читаем файл построчно
            content += line + '\n';  // Добавляем строку к содержимому, добавляя перенос строки
        }
        file.close();               // Закрываем файл
    } else {
        std::cerr << "Не удалось открыть файл для чтения!" << std::endl; // Сообщаем об ошибке
    }

    return content;                // Возвращаем содержимое файла
}

// Функция для подсчета частот блоков
std::map<std::string, int> countBlocks(const std::string& text, int blockSize) {
    std::map<std::string, int> blockCount; // Словарь для хранения блоков и их частот
    int textLength = text.length();         // Получаем длину текста

    for (int i = 0; i <= textLength - blockSize; ++i) { // Проходим по тексту до его конца минус размер блока
        std::string block = text.substr(i, blockSize);  // Извлекаем блок заданного размера
        blockCount[block]++;                     // Увеличиваем счетчик этого блока в словаре
    }

    return blockCount;                          // Возвращаем словарь с частотами блоков
}

// Функция для вычисления энтропии
double calculateEntropy(const std::map<std::string, int>& blockCount, int totalBlocks) {
    double entropy = 0.0;                       // Переменная для хранения значения энтропии

    for (const auto& pair : blockCount) {      // Проходим по всем парам (блок и его частота)
        double probability = static_cast<double>(pair.second) / totalBlocks; // Вычисляем вероятность блока
        if (probability > 0) {                   // Проверяем, что вероятность больше нуля
            entropy -= probability * log2(probability);  // Обновляем значение энтропии по формуле Шеннона
        }
    }

    return entropy;                             // Возвращаем рассчитанную энтропию
}

// Функция для расчета избыточности
double calculateRedundancy(double entropy, int blockSize) {
    return (blockSize - entropy) / blockSize;  // Вычисляем избыточность по формуле: (размер блока - энтропия) / размер блока
}

int main() {
    const std::string filename = "text.txt";  // Имя файла для записи текста
    const std::string text = "ABBACACC";  // Текст для анализа

    // Шаг 3.1: Запись текста в файл
    writeToFile(filename, text);  // Вызываем функцию записи текста в файл

    // Шаг 3.2: Чтение содержимого файла
    std::string fileContent = readFromFile(filename);  // Читаем содержимое файла и сохраняем его в переменную
    std::cout << "Содержимое файла:\n" << fileContent << std::endl;  // Выводим содержимое файла на экран

    // Шаги 3.3 и 3.4: Анализ блоков
    for (int blockSize = 1; blockSize <= 5; ++blockSize) {   // Проходим по размерам блоков от 1 до 5 символов включительно
        std::map<std::string, int> blockCount = countBlocks(fileContent, blockSize);  // Подсчитываем частоты блоков заданного размера
        
        int totalBlocks = 0;             // Переменная для хранения общего количества блоков
        
        // Подсчет общего количества блоков
        for (const auto& pair : blockCount) {   // Проходим по всем парам в словаре частот блоков
            totalBlocks += pair.second;          // Суммируем частоты блоков для получения общего количества блоков
        }

        double entropy = calculateEntropy(blockCount, totalBlocks);   // Рассчитываем энтропию на основе частот блоков и их общего количества
        double redundancy = calculateRedundancy(entropy, blockSize);   // Рассчитываем избыточность на основе энтропии и размера блока

        std::cout << "Блоки длиной " << blockSize << " символов:\n";   // Выводим информацию о текущем размере блока 
        std::cout << "Энтропия: " << entropy << std::endl;            // Выводим рассчитанную энтропию 
        std::cout << "Избыточность: " << redundancy << std::endl;      // Выводим рассчитанную избыточность 
    }

    return 0;   // Завершаем программу с кодом возврата 0 (успех)
}
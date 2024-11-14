// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>


// class COMBO {
//     public:
//     std::string characters;

//     COMBO(const std::string& chars) : characters(chars) {};
// };


// void Proverka(std::ifstream& in, std::vector<char>& symbols) {
    
//     std::string word;

//     if(in.is_open()){
//         std::cout << "Файл открывается" << std::endl << std::endl;
        
//         char ch;
        
//         while (in.get(ch))
//         {
//             symbols.push_back(ch);
//         }
        

//         std::cout << "Содержимое файла: ";

//         for (const auto& symbol : symbols) {
//             std::cout << symbol << " "; // Выводим каждое слово
//         }
//     }
//     else{
//         std::cout << "Файл не открывается" << std::endl << std::endl;
//     }
// }


// std::vector<COMBO> Combo(const std::vector<char>& symbols) {
//     std::vector<COMBO> combos;

//     for(size_t i = 0; i < symbols.size(); ++i) {
//         for(size_t j = 0; j < symbols.size(); ++j) {
//             for(size_t k = 0; k < symbols.size(); ++k) {
//                 std::string block = {symbols[i] , symbols[j] , symbols[k]};

//                 combos.emplace_back(block);
//             }
//         }
//     }

//     for(const auto& combo : combos) {
//         std::cout << "combo: " << combo.characters << std::endl;
//     }

//     return combos;
// }


// int main(){
//     setlocale(LC_ALL, "RUS");
//     std::vector<char> symbols;

//     std::ifstream in("text.txt");
//     // std::ofstream in("AAABBCCCCCCC");

//     Proverka(in, symbols);

//     in.close();
    
//     Combo(symbols);


//     return 0;
// }


// // #include <iostream>
// // #include <string>

// // const int MAX_N = 5; // Максимальная длина n-грамм
// // const int MAX_GRAMS = 10000; // Максимальное количество уникальных n-грамм

// // // Структура для хранения n-грамм и их частоты
// // struct NGram {
// //     std::string ngram;
// //     int count;
// // };

// // // Функция для подсчета частоты многознаковых комбинаций
// // void countNGrams(const std::string& text, int n, NGram* ngrams, int& totalNGrams) {
// //     totalNGrams = 0;

// //     for (size_t i = 0; i <= text.length() - n; ++i) {
// //         std::string ngram = text.substr(i, n);
        
// //         // Проверяем, существует ли уже эта n-грамма
// //         bool found = false;
// //         for (int j = 0; j < totalNGrams; ++j) {
// //             if (ngrams[j].ngram == ngram) {
// //                 ngrams[j].count++;
// //                 found = true;
// //                 break;
// //             }
// //         }

// //         // Если не нашли, добавляем новую n-грамму
// //         if (!found && totalNGrams < MAX_GRAMS) {
// //             ngrams[totalNGrams].ngram = ngram;
// //             ngrams[totalNGrams].count = 1;
// //             totalNGrams++;
// //         }
// //     }
// // }

// // // Функция для вывода наиболее частых n-грамм
// // void printTopNGrams(NGram* ngrams, int totalNGrams, int topN) {
// //     for (int i = 0; i < totalNGrams; ++i) {
// //         for (int j = i + 1; j < totalNGrams; ++j) {
// //             if (ngrams[i].count < ngrams[j].count) {
// //                 // Меняем местами
// //                 NGram temp = ngrams[i];
// //                 ngrams[i] = ngrams[j];
// //                 ngrams[j] = temp;
// //             }
// //         }
// //     }

// //     // Выводим топ N n-грамм
// //     for (int i = 0; i < topN && i < totalNGrams; ++i) {
// //         std::cout << ngrams[i].ngram << ": " << ngrams[i].count << std::endl;
// //     }
// // }

// // int main() {
// //     std::string text;

// //     // Ввод текста
// //     std::cout << "Введите текст: ";
// //     std::getline(std::cin, text);

// //     // Массив для хранения n-грамм
// //     NGram ngrams[MAX_GRAMS];
// //     int totalNGrams;

// //     // Подсчет и вывод частоты для каждой длины от 1 до MAX_N
// //     for (int n = 1; n <= MAX_N; ++n) {
// //         std::cout << "\nЧастота " << n << "-грамм:\n";
// //         countNGrams(text, n, ngrams, totalNGrams);
// //         printTopNGrams(ngrams, totalNGrams, 10); // Выводим топ-10 n-грамм
// //     }

// //     return 0;
// // }



#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

// Функция для записи текста в файл
void writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << content;
        outFile.close();
    } else {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
    }
}

// Функция для чтения из файла и возврата его содержимого
std::string readFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        std::string content((std::istreambuf_iterator<char>(inFile)), 
                            std::istreambuf_iterator<char>());
        inFile.close();
        return content;
    } else {
        std::cerr << "Ошибка открытия файла для чтения!" << std::endl;
        return "";
    }
}

// Функция для вычисления частот и энтропии блоков символов
void calculateBlockFrequencies(const std::string& text) {
    std::map<std::string, int> frequencyMap;
    int totalBlocks = 0;

    // Вычисляем частоты блоков длиной до 5 символов
    for (size_t length = 1; length <= 5; ++length) {
        for (size_t i = 0; i <= text.length() - length; ++i) {
            std::string block = text.substr(i, length);
            frequencyMap[block]++;
            totalBlocks++;
        }
    }

    // Вычисляем вероятности и энтропию
    double entropy = 0.0;
    for (const auto& pair : frequencyMap) {
        double probability = static_cast<double>(pair.second) / totalBlocks;
        entropy -= probability * log2(probability);
        std::cout << "Блок: " << pair.first 
                  << ", Частота: " << pair.second 
                  << ", Вероятность: " << probability << std::endl;
    }

    std::cout << "Всего блоков: " << totalBlocks 
              << ", Энтропия: " << entropy << std::endl;
}

int main() {
    const std::string filename = "text.txt";
    
    // Шаг 1: Записываем произвольный текст в файл
    writeToFile(filename, "ABBBAAACCCCCC.");

    // Шаг 2: Читаем из файла
    std::string content = readFromFile(filename);
    
    // Проверяем, успешно ли прочитано содержимое перед продолжением
    if (!content.empty()) {
        // Шаг 3: Вычисляем частоты блоков и энтропию
        calculateBlockFrequencies(content);
    }

    return 0;
}
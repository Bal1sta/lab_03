#include <iostream>
#include <fstream>
#include <string>


void Proverka(std::ifstream& in){
    

    if(!in){
        std::cout << "Файл не открывается" << std::endl << std::endl;
    }
    else{
        std::cout << "Файл открывается" << std::endl << std::endl;
    }
}


void Print(std::ifstream& in){
    std::string strInput;
    while(in >> strInput){
        std::cout << strInput << std::endl;
    }
}


int main(){
    setlocale(LC_ALL, "RUS");

    std::ifstream in("text.txt");
    Proverka(in);
    Print(in);
}
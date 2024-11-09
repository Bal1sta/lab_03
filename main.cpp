#include <iostream>

template <typename T> // Определяем шаблонный класс DynamicArray, который может хранить элементы любого типа T
class DynamicArray {
private:
    T* data; // Указатель на массив элементов типа T
    size_t capacity; // Максимальное количество элементов, которое может быть сохранено без изменения размера
    size_t length; // Текущее количество элементов в массиве

    void resize(size_t new_capacity) { // Метод для изменения размера массива
        T* new_data = new T[new_capacity]; // Создаем новый массив с новой емкостью
        for (size_t i = 0; i < length; ++i) { // Копируем элементы из старого массива в новый
            new_data[i] = std::move(data[i]); // Перемещаем элементы для оптимизации.
        }// Использование std::move является оптимизацией, которая позволяет избежать ненужных копий, передавая владение ресурсами.
        delete[] data; // Освобождаем память старого массива
        data = new_data; // Устанавливаем указатель на новый массив
        capacity = new_capacity; // Обновляем емкость
    }
// Динамический контейнер
/////////////////////////////////////////////////////////////////////////////////////
public:
    DynamicArray(size_t initial_capacity = 10) // Конструктор, инициализирующий массив с начальной емкостью
        : capacity(initial_capacity), length(0), data(new T[initial_capacity]) {} // Инициализация членов класса

    ~DynamicArray() { // Деструктор для освобождения памяти
        delete[] data; // Освобождаем память, занятую массивом
    }

    // Конструктор копирования
    DynamicArray(const DynamicArray& other) // Конструктор, создающий копию другого массива
        : capacity(other.capacity), length(other.length), data(new T[other.capacity]) { // Инициализация членов класса
        for (size_t i = 0; i < length; ++i) { // Копируем элементы из другого массива
            data[i] = other.data[i]; // Присваиваем значения из другого массива
        }
    }

    // Конструктор перемещения
    DynamicArray(DynamicArray&& other) noexcept // Конструктор перемещения для оптимизации передачи ресурсов
        : capacity(other.capacity), length(other.length), data(other.data) { // Инициализация членов класса
        other.data = nullptr; // Обнуляем указатель у перемещаемого объекта, чтобы избежать двойного освобождения памяти
        other.length = 0; // Обнуляем длину перемещаемого объекта
        other.capacity = 0; // Обнуляем емкость перемещаемого объекта
    }

    // Оператор присваивания копирования
    DynamicArray& operator=(const DynamicArray& other) { // Оператор присваивания копирования для копирования содержимого другого массива
        if (this == &other) return *this; // Проверка на самоприсваивание, если это так, ничего не делаем
        delete[] data; // Освобождаем память старого массива
        capacity = other.capacity; // Устанавливаем новую емкость
        length = other.length; // Устанавливаем новую длину
        data = new T[capacity]; // Создаем новый массив с новой емкостью
        for (size_t i = 0; i < length; ++i) { // Копируем элементы из другого массива в новый
            data[i] = other.data[i]; 
        }
        return *this; // Возвращаем текущий объект для цепочки присваиваний
    }

    // Оператор присваивания перемещения
    DynamicArray& operator=(DynamicArray&& other) noexcept { // Оператор присваивания перемещения для оптимизации передачи ресурсов
        if (this == &other) return *this; // Проверка на самоприсваивание, если это так, ничего не делаем
        delete[] data; // Освобождаем память старого массива
        capacity = other.capacity; // Устанавливаем новую емкость из перемещаемого объекта
        length = other.length; // Устанавливаем новую длину из перемещаемого объекта
        data = other.data; // Перемещаем указатель на данные

        other.data = nullptr; // Обнуляем указатель у перемещаемого объекта, чтобы избежать двойного освобождения памяти
        other.length = 0; 
        other.capacity = 0;

        return *this; 
    }

    void push_back(const T& value) { // Метод для добавления элемента в конец массива (по ссылке)
        if (length == capacity) { // Если массив заполнен, увеличиваем его размер
            resize(capacity + capacity / 2); 
        }
        data[length++] = value; // Добавляем элемент и увеличиваем длину массива на 1
    }

    void push_back(T&& value) { // Метод для добавления элемента в конец массива (по r-value ссылке)
        if (length == capacity) { 
            resize(capacity + capacity / 2); 
        }
        data[length++] = std::move(value); // Перемещаем элемент и увеличиваем длину массива на 1
    }

    void insert(size_t index, const T& value) { // Метод для вставки элемента по указанному индексу 
        if (index > length) throw std::out_of_range("Index out of range"); // Проверка на выход за пределы длины массива 
        if (length == capacity) { 
            resize(capacity + capacity / 2); 
        }
        for (size_t i = length; i > index; --i) { 
            data[i] = std::move(data[i - 1]); // Сдвигаем элементы вправо для освобождения места под новый элемент 
        }
        data[index] = value; 
        ++length; 
    }

    void erase(size_t index) { // Метод для удаления элемента по указанному индексу 
        if (index >= length) throw std::out_of_range("Index out of range"); 
        for (size_t i = index; i < length - 1; ++i) { 
            data[i] = std::move(data[i + 1]); // Сдвигаем элементы влево после удаления элемента 
        }
        --length; 
    }

    void insert_middle(const T& value) { // Метод для вставки элемента в середину массива 
        size_t middle_index = length / 2; 
        insert(middle_index, value); 
    }

    void shrink_to_fit() { // Метод для уменьшения размера выделенной памяти до текущей длины массива 
       if (length < capacity) { 
           T* new_data = new T[length]; // Создаем новый массив с размером, равным текущей длине 
           for (size_t i = 0; i < length; ++i) { 
               new_data[i] = std::move(data[i]); 
           } 
           delete[] data; 
           data = new_data;
           capacity = length;
       } 
   }

    T get(size_t index) const { // Метод для получения элемента по индексу с проверкой границ 
        if (index >= length) throw std::out_of_range("Index out of range"); 
        return data[index]; 
    }

    size_t size() const { return length; } // Метод для получения текущего количества элементов в массиве 

    T& operator[](size_t index) { // Оператор доступа к элементам по индексу с проверкой границ 
        if (index >= length) throw std::out_of_range("Index out of range"); 
        return data[index]; 
    }

    void print() const { // Метод для вывода элементов массива в консоль 
        for (size_t i = 0; i < length; ++i) {
            std::cout << data[i] << (i < length - 1 ? ", " : ""); // Выводим элементы через запятую 
        }
        std::cout << std::endl;
    }
};
/////////////////////////////////////////////////////////////////////////////////////

// двусвязный список
/////////////////////////////////////////////////////////////////////////////////////
template <typename T> // Определяем шаблонный класс DoublyNode, который может хранить данные любого типа T
class DoublyNode {
public:
    T data; // Данные узла типа T
    DoublyNode* next; // Указатель на следующий узел в списке
    DoublyNode* prev; // Указатель на предыдущий узел в списке

    DoublyNode(const T& value) : data(value), next(nullptr), prev(nullptr) {} // Конструктор, инициализирующий данные и указатели
};

template <typename T> // Определяем шаблонный класс для итератора двусвязного списка
class DoublyLinkedListIterator {
private:
    DoublyNode<T>* current; // Указатель на текущий узел, на который указывает итератор

public:
    DoublyLinkedListIterator(DoublyNode<T>* node) : current(node) {} // Конструктор, инициализирующий итератор текущим узлом

    T& operator*() { // Оператор разыменования для доступа к данным текущего узла
        return current->data; // Возвращаем ссылку на данные текущего узла
    }

    DoublyLinkedListIterator& operator++() { // Префиксный инкремент для перехода к следующему узлу
        if (current) current = current->next; // Если текущий узел существует, перемещаемся к следующему узлу
        return *this; // Возвращаем текущий итератор для поддержки цепочки операций
    }

    DoublyLinkedListIterator& operator--() { // Префиксный декремент для перехода к предыдущему узлу
        if (current) current = current->prev; // Если текущий узел существует, перемещаемся к предыдущему узлу
        return *this; // Возвращаем текущий итератор для поддержки цепочки операций
    }

    bool operator!=(const DoublyLinkedListIterator& other) const { // Оператор неравенства для сравнения двух итераторов
        return current != other.current; // Возвращаем true, если текущие узлы не равны
    }
};

template <typename T> // Определяем шаблонный класс для двусвязного списка, который может хранить элементы любого типа T
class DoublyLinkedList {
private:
    DoublyNode<T>* head; // Указатель на первый элемент списка (голову)
    DoublyNode<T>* tail; // Указатель на последний элемент списка (хвост)
    size_t length; // Размер списка (количество элементов)

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), length(0) {} // Конструктор, инициализирующий пустой список

    ~DoublyLinkedList() { // Деструктор для освобождения памяти
        while (head) { // Пока есть элементы в списке
            DoublyNode<T>* temp = head; // Сохраняем указатель на текущую голову
            head = head->next; // Перемещаем голову на следующий элемент
            delete temp; // Освобождаем память текущего узла
        }
    }

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), length(0) { // Конструктор копирования
       for (DoublyNode<T>* current = other.head; current != nullptr; current = current->next) { // Проходим по всем узлам другого списка
           push_back(current->data); // Используем push_back для копирования данных в новый список
       }
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept : head(other.head), tail(other.tail), length(other.length) { // Конструктор перемещения
       other.head = nullptr; // Обнуляем указатели у перемещаемого объекта, чтобы избежать двойного освобождения памяти
       other.tail = nullptr;
       other.length = 0;
    }

   void push_back(const T& value) { // Метод для добавления элемента в конец списка (по ссылке)
       DoublyNode<T>* newNode = new DoublyNode<T>(value); // Создаем новый узел с переданным значением
       if (!head) { // Если список пустой
           head = tail = newNode; // Новый узел становится и головой, и хвостом списка
       } else { 
           tail->next = newNode; // Устанавливаем указатель next у текущего хвоста на новый узел
           newNode->prev = tail;  // Устанавливаем указатель prev у нового узла на текущий хвост
           tail = newNode; // Обновляем хвост на новый узел
       } 
       ++length; // Увеличиваем размер списка на 1
   }

   DoublyLinkedList& operator=(const DoublyLinkedList& other) { // Оператор присваивания копирования
       if (this == &other) return *this; // Проверка на самоприсваивание

       // Освобождаем текущие ресурсы
       while (head) { // Пока есть элементы в списке
           DoublyNode<T>* temp = head; // Сохраняем указатель на текущую голову
           head = head->next; // Перемещаем голову на следующий элемент
           delete temp; // Освобождаем память текущего узла
       }

       head = tail = nullptr; // Обнуляем указатели на голову и хвост 
       length = 0; // Сбрасываем длину списка

       for (DoublyNode<T>* current = other.head; current != nullptr; current = current->next) { // Проходим по всем узлам другого списка
           push_back(current->data); // Используем push_back для копирования данных в новый список
       }
       
       return *this; // Возвращаем текущий объект для поддержки цепочки присваиваний
   }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept { // Оператор присваивания перемещения
       if (this == &other) return *this; // Проверка на самоприсваивание

       // Освобождаем текущие ресурсы
       while (head) { 
           DoublyNode<T>* temp = head;
           head = head->next;
           delete temp;
       }

       head = other.head;  // Перемещаем указатели из другого объекта
       tail = other.tail;
       length = other.length;

       // Обнуляем перемещаемый объект, чтобы избежать двойного освобождения памяти
       other.head = nullptr; 
       other.tail = nullptr; 
       other.length = 0;

       return *this; 
   }

   void push_back(T&& value) { // Метод для добавления элемента в конец списка (по r-value ссылке)
       DoublyNode<T>* newNode = new DoublyNode<T>(std::move(value));  // Создаем новый узел с перемещением значения 
       if (!head) { 
           head = tail = newNode; 
       } else { 
           tail->next = newNode; 
           newNode->prev = tail;  
           tail = newNode; 
       } 
       ++length; 
   }

   void push_front(const T& value) { // Метод для добавления элемента в начало списка (по ссылке)
       DoublyNode<T>* newNode = new DoublyNode<T>(value);  // Создаем новый узел с переданным значением 
       if (!head) { 
           head = tail = newNode;  // Если список пустой, новый узел становится и головой, и хвостом 
       } else { 
           newNode->next = head;  // Устанавливаем указатель next нового узла на текущую голову 
           head->prev = newNode;   // Устанавливаем указатель prev у текущей головы на новый узел  
           head = newNode;         // Обновляем голову на новый узел 
       } 
       ++length;  // Увеличиваем размер списка на 1 
   }

   void insert(size_t index, const T& value) {  // Метод для вставки элемента по указанному индексу 
       if (index > length) throw std::out_of_range("Index out of range");  // Проверка на выход за пределы длины списка 

       if (index == 0) {  // Если индекс равен нулю, вставляем элемент в начало списка 
           push_front(value);  
           return;  
       } 

       if (index == length) {  // Если индекс равен длине, вставляем элемент в конец списка 
           push_back(value);  
           return;  
       } 

       DoublyNode<T>* newNode = new DoublyNode<T>(value);  // Создание нового узла с переданным значением 
       DoublyNode<T>* current = head;

       for (size_t i = 0; i < index; ++i) {  // Перемещаемся к нужному элементу по индексу 
           current = current->next;
       } 

       newNode->next = current;      // Устанавливаем указатель next у нового узла на текущий узел 
       newNode->prev = current->prev;  // Устанавливаем указатель prev у нового узла на предыдущий узел 

       if (current->prev)  // Если у текущего узла есть предыдущий узел, обновляем его указатель next 
           current->prev->next = newNode;

       current->prev = newNode;  // Устанавливаем указатель prev у текущего узла на новый узел 

       if (index == 0)  // Если мы вставили элемент в начало, обновляем голову списка 
           head = newNode;

       ++length;  // Увеличиваем размер списка на 1 
   }

    void insert_middle(const T& value) {  // Метод для вставки элемента в середину списка 
        size_t middle_index = length / 2;  // Вычисляем индекс середины списка 
        insert(middle_index, value);  // Вставляем значение в середину с помощью метода insert 
    }

   void erase(size_t index) {  // Метод для удаления элемента по указанному индексу 
        if (index >= length) throw std::out_of_range("Index out of range");  // Проверка на выход за пределы длины списка 

        DoublyNode<T>* current = head;  // Инициализируем указатель на голову списка 

        for (size_t i = 0; i < index; ++i)  
            current = current->next;  // Перемещаем указатель к узлу, который находится по указанному индексу 

        if (current->prev)  // Если у текущего узла есть предыдущий узел, обновляем его указатель next 
            current->prev->next = current->next;

        if (current->next)  // Если у текущего узла есть следующий узел, обновляем его указатель prev 
            current->next->prev = current->prev;

        if (current == head)  // Если удаляемый элемент - голова списка, обновляем голову 
            head = current->next;

        if (current == tail)  // Если удаляемый элемент - хвост списка, обновляем хвост 
            tail = current->prev;

        delete current;  // Освобождаем память удаляемого узла 

        --length;  // Уменьшаем размер списка на 1 
   }

   void print() const {  // Метод для вывода элементов списка в консоль 
      DoublyNode<T>* current= head;  
      while(current != nullptr){  
          std::cout << current -> data << " ";  // Выводим данные текущего узла  
          current=current -> next;  // Переходим к следующему узлу  
      }  
      std::cout << std::endl;  
   } 

   T get(size_t index) const {  // Метод для получения элемента по индексу с проверкой границ 
       if (index >= length) throw std::out_of_range("Index out of range");  

       DoublyNode<T>* current = head;  
       for (size_t i = 0; i < index && current != nullptr; ++i) {  
           current = current->next;  
       }  

       return current->data;  // Возвращаем данные найденного узла  
   }

   size_t getSize() const { return length; }  // Метод для получения размера списка 

   DoublyLinkedListIterator<T> begin() {  
      return DoublyLinkedListIterator<T>(head);  // Возвращаем итератор на голову списка  
   } 

   DoublyLinkedListIterator<T> end() {  
      return DoublyLinkedListIterator<T>(nullptr);  // Возвращаем итератор на nullptr, обозначающий конец списка  
   } 

};
/////////////////////////////////////////////////////////////////////////////////////



template <typename T> // Определяем шаблонный класс для узла односвязного списка, который может хранить данные любого типа T
class Node {
public:
    T data; // Данные узла типа T
    Node* next; // Указатель на следующий узел в списке

    Node(const T& value) : data(value), next(nullptr) {} // Конструктор, инициализирующий данные и указатель на следующий узел
};



// односвязный список
/////////////////////////////////////////////////////////////////////////////////////
template <typename T> // Определяем шаблонный класс для итератора односвязного списка
class SinglyLinkedListIterator {
private:
    Node<T>* current; // Указатель на текущий узел, на который указывает итератор

public:
    SinglyLinkedListIterator(Node<T>* node) : current(node) {} // Конструктор, инициализирующий итератор текущим узлом

    T& operator*() { // Оператор разыменования для доступа к данным текущего узла
        return current->data; // Возвращаем ссылку на данные текущего узла
    }

    SinglyLinkedListIterator& operator++() { // Префиксный инкремент для перехода к следующему узлу
        if (current) current = current->next; // Если текущий узел существует, перемещаемся к следующему узлу
        return *this; // Возвращаем текущий итератор для поддержки цепочки операций
    }

    bool operator!=(const SinglyLinkedListIterator& other) const { // Оператор неравенства для сравнения двух итераторов
        return current != other.current; // Возвращаем true, если текущие узлы не равны
    }
};

template <typename T> // Определяем шаблонный класс для односвязного списка
class SinglyLinkedList {
private:
    Node<T>* head; // Указатель на первый элемент списка (голова)
    size_t length; // Размер списка (количество элементов)

public:
    SinglyLinkedList() : head(nullptr), length(0) {} // Конструктор, инициализирующий пустой список

    ~SinglyLinkedList() { // Деструктор для освобождения памяти
        while (head) { // Пока есть элементы в списке
            Node<T>* temp = head; // Сохраняем указатель на текущую голову
            head = head->next; // Перемещаем голову на следующий элемент
            delete temp; // Освобождаем память текущего узла
        }
    }

    void push_back(const T& value) { // Метод для добавления элемента в конец списка (по ссылке)
        Node<T>* newNode = new Node<T>(value); // Создаем новый узел с переданным значением
        if (!head) { // Если список пустой
            head = newNode; // Новый узел становится головой списка
        } else { 
            Node<T>* temp = head; // Инициализируем временный указатель на голову списка
            while (temp->next) { // Проходим по списку до последнего узла
                temp = temp->next; 
            }
            temp->next = newNode; // Устанавливаем указатель next последнего узла на новый узел
        }
        ++length; // Увеличиваем размер списка на 1
    }

    void insert(size_t index, const T& value) { // Метод для вставки элемента по указанному индексу 
        if (index > length) throw std::out_of_range("Index out of range"); // Проверка на выход за пределы длины списка
        Node<T>* newNode = new Node<T>(value); // Создаем новый узел с переданным значением
        if (index == 0) {  // Если индекс равен нулю, вставляем элемент в начало списка 
            newNode->next = head;  // Устанавливаем указатель next нового узла на текущую голову 
            head = newNode;  // Обновляем голову на новый узел 
        } else {
            Node<T>* temp = head;  // Инициализируем временный указатель на голову списка 
            for (size_t i = 0; i < index - 1; ++i) {  // Перемещаемся к нужному элементу по индексу 
                temp = temp->next;
            }
            newNode->next = temp->next;  // Устанавливаем указатель next нового узла на следующий элемент после текущего 
            temp->next = newNode;  // Устанавливаем указатель next у текущего элемента на новый узел 
        }
        ++length;  // Увеличиваем размер списка на 1 
    }

    void insert_middle(const T& value) {  // Метод для вставки элемента в середину списка 
        size_t middle_index = length / 2;  // Вычисляем индекс середины списка 
        insert(middle_index, value);  // Вставляем значение в середину с помощью метода insert 
    }

    void erase(size_t index) {  // Метод для удаления элемента по указанному индексу 
        if (index >= length) throw std::out_of_range("Index out of range");  // Проверка на выход за пределы длины списка 
        Node<T>* temp = head;  // Инициализируем временный указатель на голову списка 
        if (index == 0) {  // Если удаляемый элемент - голова списка 
            head = head->next;  // Обновляем голову на следующий элемент 
            delete temp;  // Освобождаем память удаляемого узла 
        } else {
            for (size_t i = 0; i < index - 1; ++i) {  // Перемещаемся к элементу перед удаляемым 
                temp = temp->next;
            }
            Node<T>* toDelete = temp->next;  // Сохраняем указатель на удаляемый элемент 
            temp->next = toDelete->next;  // Обновляем указатель next у предыдущего элемента 
            delete toDelete;  // Освобождаем память удаляемого узла 
        }
        --length;  // Уменьшаем размер списка на 1 
    }

    size_t size() const { return length; }  // Метод для получения размера списка 

    SinglyLinkedListIterator<T> begin() {  
        return SinglyLinkedListIterator<T>(head);  // Возвращаем итератор на голову списка  
    } 

    SinglyLinkedListIterator<T> end() {  
        return SinglyLinkedListIterator<T>(nullptr);  // Возвращаем итератор на nullptr, обозначающий конец списка  
    } 

    void print() const {  // Метод для вывода элементов списка в консоль 
        Node<T>* temp = head;  
        while (temp) {  
            std::cout << temp->data << (temp->next ? ", " : "");  // Выводим данные текущего узла с запятой, если есть следующий элемент  
            temp = temp->next;  // Переходим к следующему узлу  
        }  
        std::cout << std::endl;  
    }
};
/////////////////////////////////////////////////////////////////////////////////////



int main() { // Начало функции main
    DynamicArray<int> arr; // Создаем экземпляр динамического массива для хранения целых чисел

    // Заполняем массив числами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i); // Добавляем элемент i в конец массива
    }

    std::cout << "Вывод ожидаемого контейнера: " << '\n'; // Выводим сообщение о выводе содержимого массива
    arr.print(); // Печатаем содержимое динамического массива

    // Вывод размера массива
    std::cout << "Size: " << arr.size() << std::endl; // Выводим размер массива (ожидается 10)

    // Удаление элементов (удаляется на элемент меньше, т.к. нумеруются с нуля)
    arr.erase(2);  // Удаляем элемент по индексу 2
    arr.erase(4);  // Удаляем элемент по индексу 4 (после предыдущего удаления это будет 5-й элемент)
    arr.erase(5);  // Удаляем элемент по индексу 5 (после предыдущих удалений это будет 6-й элемент)

    std::cout << "Вывод содержимого на экран " << '\n'; // Выводим сообщение о выводе оставшихся элементов
    arr.print(); // Печатаем оставшееся содержимое динамического массива

    arr.insert(0, 10); // Вставляем число 10 в начало массива
    std::cout << "Вывод содержимого с десяткой в начале " << '\n'; // Сообщение о выводе с добавленным элементом
    arr.print(); // Печатаем содержимое динамического массива

    // Вставка элемента в середину массива
    arr.insert_middle(20); // Вставляем число 20 в середину массива
    std::cout << "Вывод содержимого с 20 в середине " << '\n'; // Сообщение о выводе с добавленным элементом
    arr.print(); // Печатаем содержимое динамического массива

    arr.push_back(30); // Добавляем число 30 в конец массива

    std::cout << "Вывод содержимого с 30 в конце " << '\n'; // Сообщение о выводе с добавленным элементом
    arr.print(); // Печатаем содержимое динамического массива

    SinglyLinkedList<int> list; // Создаем экземпляр односвязного списка для хранения целых чисел

    // Добавление 10 элементов в односвязный список
    for (int i = 0; i < 10; ++i) {
        list.push_back(i); // Добавляем элемент i в конец списка
    }
    
    std::cout << "Вывод однонаправленного списка: " << '\n'; // Сообщение о выводе содержимого списка
    list.print(); // Печатаем содержимое односвязного списка

    std::cout << "Вывод однонаправленного списка с удаленными элементами: " << '\n'; // Сообщение о выводе после удаления элементов

    list.erase(2); // Удаляем элемент по индексу 2 из списка
    list.erase(4); // Удаляем элемент по индексу 4 из списка (после предыдущего удаления это будет 5-й элемент)
    list.erase(5); // Удаляем элемент по индексу 5 из списка (после предыдущих удалений это будет 6-й элемент)

    list.print(); // Печатаем оставшееся содержимое односвязного списка

    std::cout << "Вывод однонаправленного списка с 10 в начале: " << '\n';   // Сообщение о вставке элемента в начало списка 
    list.insert(0, 10); // Вставляем число 10 в начало односвязного списка
    list.print(); // Печатаем содержимое односвязного списка

    std::cout << "Вывод однонаправленного списка с 4 в середине: " << '\n'; // Сообщение о вставке элемента в середину списка 
    list.insert_middle(4); // Вставляем число 4 в середину односвязного списка
    list.print(); // Печатаем содержимое односвязного списка

    std::cout << "Вывод однонаправленного списка с 30 в конце: " << '\n'; // Сообщение о добавлении элемента в конец списка 
    list.push_back(30); // Добавляем число 30 в конец односвязного списка
    list.print(); // Печатаем содержимое односвязного списка
    
    DoublyLinkedList<int> doubleList; // Создаем экземпляр двусвязного списка для хранения целых чисел

    for (int i = 0; i < 10; ++i) { 
        doubleList.push_back(i); // Добавляем элементы от 0 до 9 в двусвязный список 
    }

    std::cout << "Двунаправленный список:\n"; 
    doubleList.print(); // Печатаем содержимое двусвязного списка

    std::cout << "Двунаправленный список с 4 в середине и удаленным третьим элементом " << '\n'; 
    doubleList.erase(2); // Удаляем третий элемент (индекс 2) из двусвязного списка 
    doubleList.insert_middle(4); // Вставляем число 4 в середину двусвязного списка 
    doubleList.print(); // Печатаем обновленное содержимое двусвязного списка 

    
    return 0; // Завершение функции main и возврат к операционной системе 
}
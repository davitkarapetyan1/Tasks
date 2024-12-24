#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdexcept>
#include <iostream>
#include <cstring>

// Класс для работы с разделяемым массивом
class shared_array {
public:
    shared_array(const std::string& name, size_t size);
    ~shared_array();

    // Операторы для работы с массивом
    int& operator[](size_t index);
    const int& operator[](size_t index) const;

    // Метод для получения размера массива
    size_t size() const;

private:
    std::string name_;         // Имя массива
    size_t size_;              // Размер массива
    int* array_;               // Указатель на данные массива
    sem_t* sem_;               // Семофор для синхронизации
};

#endif // SHARED_ARRAY_H


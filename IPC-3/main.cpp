#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main() {
    const std::string array_name = "shared_array_name"; // Имя массива
    const size_t array_size = 10; // Размер массива (пример)

    // Создаем разделяемый массив
    try {
        shared_array arr(array_name, array_size);
        
        while (true) {
            // Работа с массивом
            std::cout << "Enter index (0-" << array_size - 1 << ") to modify or -1 to exit: ";
            int index;
            std::cin >> index;
            
            if (index == -1) {
                break;
            }

            if (index >= 0 && index < array_size) {
                std::cout << "Enter value for index " << index << ": ";
                int value;
                std::cin >> value;
                
                // Модификация массива
                arr[index] = value;
                std::cout << "Updated value at index " << index << " to " << arr[index] << std::endl;
            } else {
                std::cout << "Invalid index!" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


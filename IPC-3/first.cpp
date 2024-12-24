// first.cpp
#include "shared_array.h"

int main() {
    shared_array array("shared-array", 1000); // Создаем общий массив с размером 1000 элементов

    while (true) {
        int index, value;
        std::cout << "[First] Enter index to modify: ";
        std::cin >> index;

        std::cout << "[First] Enter value to set: ";
        std::cin >> value;

        if (index < 0 || index >= array.size()) {
            std::cerr << "[First] Invalid index." << std::endl;
            continue;
        }

        // Изменяем элемент массива
        array[index] = value;

        std::cout << "[First] Array[" << index << "] set to " << value << std::endl;
    }

    return 0;
}


// second.cpp
#include "shared_array.h"

int main() {
    shared_array array("shared-array", 1000); // Открываем тот же массив

    while (true) {
        int index;
        std::cout << "[Second] Enter index to read: ";
        std::cin >> index;

        if (index < 0 || index >= array.size()) {
            std::cerr << "[Second] Invalid index." << std::endl;
            continue;
        }

        // Читаем элемент массива
        int value = array[index];
        std::cout << "[Second] Array[" << index << "] = " << value << std::endl;
    }

    return 0;
}


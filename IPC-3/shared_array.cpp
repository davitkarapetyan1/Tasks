#include "shared_array.h"

// Конструктор
shared_array::shared_array(const std::string& name, size_t size)
    : name_(name), size_(size), array_(nullptr), sem_(nullptr) {

    // Создаем именованный семафор для синхронизации
    sem_ = sem_open(name.c_str(), O_CREAT, 0644, 1);
    if (sem_ == SEM_FAILED) {
        throw std::runtime_error("Unable to create semaphore");
    }

    // Создаем именованную разделяемую память
    int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        sem_close(sem_);
        sem_unlink(name.c_str());
        throw std::runtime_error("Unable to create shared memory");
    }

    // Устанавливаем размер разделяемой памяти
    if (ftruncate(fd, size_ * sizeof(int)) == -1) {
        sem_close(sem_);
        sem_unlink(name.c_str());
        close(fd);
        throw std::runtime_error("Unable to set shared memory size");
    }

    // Отображаем память в адресное пространство
    array_ = (int*)mmap(nullptr, size_ * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (array_ == MAP_FAILED) {
        sem_close(sem_);
        sem_unlink(name.c_str());
        close(fd);
        throw std::runtime_error("Unable to map shared memory");
    }

    close(fd);  // Закрываем дескриптор файла после отображения памяти
}

// Деструктор
shared_array::~shared_array() {
    // Освобождаем ресурсы
    if (array_ != nullptr) {
        munmap(array_, size_ * sizeof(int));
    }
    if (sem_ != nullptr) {
        sem_close(sem_);
        sem_unlink(name_.c_str());
    }
}

// Оператор индексации для доступа к элементам
int& shared_array::operator[](size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    sem_wait(sem_);
    int& value = array_[index];
    sem_post(sem_);
    return value;
}

// Константный оператор индексации
const int& shared_array::operator[](size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    sem_wait(sem_);
    const int& value = array_[index];
    sem_post(sem_);
    return value;
}

// Получить размер массива
size_t shared_array::size() const {
    return size_;
}


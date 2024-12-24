#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <ucontext.h>
#include <cstdint>  // Для uintptr_t

void signalHandler(int signum, siginfo_t *info, void *context) {
    // Информация о процессе-отправителе сигнала
    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;

    struct passwd *pwd = getpwuid(sender_uid);
    const char *sender_username = pwd ? pwd->pw_name : "Unknown";

    std::cout << "Получен сигнал SIGUSR1 от процесса " << sender_pid
              << ", выполняемого пользователем с UID " << sender_uid << " (" << sender_username << ")." << std::endl;

    // Извлечение информации о регистрах из контекста сигнала
    ucontext_t *uc = (ucontext_t*)context;

    uintptr_t rip = 0, rax = 0, rbx = 0;

    // Используем регистры в зависимости от архитектуры (32-бит или 64-бит)
    #ifdef __x86_64__
        // Для 64-битной архитектуры (x86_64)
        rip = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP]; // RIP для 64-битных систем
        rax = (uintptr_t)uc->uc_mcontext.gregs[REG_RAX]; // RAX для 64-битных систем
        rbx = (uintptr_t)uc->uc_mcontext.gregs[REG_RBX]; // RBX для 64-битных систем
    #else
        // Для 32-битной архитектуры (x86)
        rip = (uintptr_t)uc->uc_mcontext.gregs[REG_EIP]; // EIP для 32-битных систем
        rax = (uintptr_t)uc->uc_mcontext.gregs[REG_EAX]; // EAX для 32-битных систем
        rbx = (uintptr_t)uc->uc_mcontext.gregs[REG_EBX]; // EBX для 32-битных систем
    #endif

    // Выводим состояние контекста: значения регистров
    std::cout << "Состояние контекста: RIP = " << std::hex << rip
              << ", RAX = " << rax << ", RBX = " << rbx << std::dec << std::endl;
}

int main() {
    // Выводим PID текущего процесса
    pid_t pid = getpid();
    std::cout << "PID этого процесса: " << pid << std::endl;

    // Устанавливаем обработчик сигнала
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;  // Мы хотим получить подробную информацию о сигнале
    sa.sa_sigaction = signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // Бесконечный цикл, чтобы процесс продолжал работать и принимать сигналы
    while (true) {
        sleep(10);  // Засыпаем на 10 секунд
    }

    return 0;
}


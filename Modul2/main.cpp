#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <cstring>

const size_t BUFFER_SIZE = 256;
int pipe_fd[2];

void sigusr1_handler(int) {
    const char* message = "Сигнал SIGUSR1 получен\n";
    write(1, message, strlen(message));
}

void child_process() {
    signal(SIGUSR1, sigusr1_handler);
    close(pipe_fd[1]);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        std::cout << buffer;
    }

    close(pipe_fd[0]);
    exit(0);
}

void parent_process() {
    close(pipe_fd[0]);

    std::string input;
    while (true) {
        std::cout << ">";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        input += '\n';
        write(pipe_fd[1], input.c_str(), input.size());
    }

    close(pipe_fd[1]);
    wait(nullptr);
}

int main() {
    if (pipe(pipe_fd) == -1) {
        perror("pipe error");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error");
        return 1;
    } else if (pid == 0) {
        child_process();
    } else {
        parent_process();
    }

    return 0;
}


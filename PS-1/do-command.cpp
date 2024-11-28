#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>
#include <cstring>

void do_command(char** argv) {
    std::clock_t start = std::clock();
    
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    }

    if (pid == 0) {
        
        execvp(argv[0], argv);
        
 
        std::cerr << "Error executing command: " << strerror(errno) << std::endl;
        exit(1);
    } else {
        
        int status;
        waitpid(pid, &status, 0);  
        
        std::clock_t end = std::clock();
        double duration = (end - start) / (double) CLOCKS_PER_SEC;
        
        if (WIFEXITED(status)) {
            std::cout << "Command completed with exit code " << WEXITSTATUS(status) << " and took " 
                      << duration << " seconds." << std::endl;
        } else {
            std::cout << "Command failed to complete properly." << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [arguments...]" << std::endl;
        return 1;
    }

    do_command(argv + 1);

    return 0;
}


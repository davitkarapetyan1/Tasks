#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <fstream>

std::vector<std::string> parseCommand(const std::string& command) {
    std::vector<std::string> args;
    size_t pos = 0;
    std::string token;
    std::string cmd = command;
    while ((pos = cmd.find(' ')) != std::string::npos) {
        token = cmd.substr(0, pos);
        args.push_back(token);
        cmd.erase(0, pos + 1);
    }
    if (!cmd.empty()) {
        args.push_back(cmd); 
    }
    return args;
}

void executeCommand(const std::string& command) {
    std::vector<std::string> args = parseCommand(command);
    if (args.empty()) return;

    bool isSilent = (args[0] == "silent");

    if (isSilent) {
        args.erase(args.begin());
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) { 
        const char* currentDir = getenv("PWD");
        std::string path = std::string(currentDir) + ":" + std::string(getenv("PATH"));
        setenv("PATH", path.c_str(), 1);

        if (isSilent) {
            std::string logFile = std::to_string(getpid()) + ".log";
            freopen(logFile.c_str(), "w", stdout);
            freopen(logFile.c_str(), "w", stderr);
        }

        std::vector<const char*> execArgs;
        for (const auto& arg : args) {
            execArgs.push_back(arg.c_str());
        }
        execArgs.push_back(nullptr);  

        
        execvp(execArgs[0], const_cast<char**>(execArgs.data()));
        perror("Exec failed"); 
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0); 
    }
}

int main() {
    std::string command;
    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, command);
        if (command == "exit") {
            std::cout << "Exiting shell...\n";
            break;
        }
        executeCommand(command);
    }
    return 0;
}


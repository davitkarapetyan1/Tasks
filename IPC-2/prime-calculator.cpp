#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

int nthPrime(int n) {
    int count = 0;
    int num = 2;
    while (true) {
        if (isPrime(num)) {
            count++;
            if (count == n) return num;
        }
        num++;
    }
}

int main() {
    int pipe1[2], pipe2[2]; 
                             
    pid_t pid;

    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Pipe creation failed!" << std::endl;
        return 1;
    }

    while (true) {
        int m;
        std::string input;
        
       
        std::cout << "[Parent] Please enter the number (or type 'exit' to quit): ";
        std::cin >> input;
        
       
        if (input == "exit") {
            std::cout << "[Parent] Exiting..." << std::endl;
            break;
        }

        try {
            m = std::stoi(input);  
        } catch (...) {
            std::cout << "[Parent] Invalid input!" << std::endl;
            continue;
        }

        std::cout << "[Parent] Sending " << m << " to the child process..." << std::endl;

        pid = fork();
        
        if (pid == -1) {
            std::cerr << "Fork failed!" << std::endl;
            return 1;
        }

        if (pid == 0) { 
            close(pipe1[1]);  
            close(pipe2[0]);  
            
            int received;
            read(pipe1[0], &received, sizeof(received)); 
            std::cout << "[Child] Calculating " << received << "-th prime number..." << std::endl;
            int result = nthPrime(received);
            
            write(pipe2[1], &result, sizeof(result));  

            close(pipe1[0]);
            close(pipe2[1]);

            exit(0);
        } else { 
            close(pipe1[0]); 
            close(pipe2[1]);  
            
            write(pipe1[1], &m, sizeof(m));  
            std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;

            int prime;
            read(pipe2[0], &prime, sizeof(prime)); 

            std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << prime << "..." << std::endl;

            close(pipe1[1]);
            close(pipe2[0]);

            
            wait(NULL);
        }
    }

    return 0;
}


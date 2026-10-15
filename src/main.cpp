#include <iostream>
#include <deque>
#include <random>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <csignal>

class TerminalGuard {
private:
    struct termios oldt;
public:
    TerminalGuard() {
        tcgetattr(STDIN_FILENO, &oldt);
        
        struct termios newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Turn off character text show on terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    ~TerminalGuard() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #ifdef __linux__
            std::system("stty echo icanon");
        #elif __APPLE__
            std::system("stty echo icanon");
        #endif
        std::cout << "\n[Hethong] Da khoi phuc terminal va hien lai chu an toan.\n";
    }
};

bool kbhit() {
    struct timeval tv = {0,0};
    fd_set fds; // Files descriptor set
    FD_ZERO(&fds); // set Files descriptor set into 0 (a clear set)
    FD_SET(0, &fds); // put stadard input descriptor to fds (stdin)
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

TerminalGuard terminalGuard;
void signalHandler(int sigNum) {
    exit(sigNum);
}

int main() {
    int fieldWidth  = 40;
    int fieldHeight = 20;

    int current_x = 1;
    int current_y = 0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_y(1, fieldWidth - 2);
    std::uniform_int_distribution<> dis_x(1, fieldHeight - 2);
    int food_x = dis_x(gen);
    int food_y = dis_y(gen);
    bool game_over = false;
    std::deque<std::pair<int, int>> snake = {{10,5}, {9,5}, {8,5}};
    signal(SIGINT, signalHandler);
    std::cout << "Hit arrow keys to move (q to quit)\n"; 
    int score = 0;
    while(!game_over) {

        if(kbhit()) {
            char c;
            read(0, &c, 1); // read the first character
            
            if(c == 'q') {
                break;
            }

            // Arrow keys have is a full ANSI command with \033 block, [ and a Letter to indicate which arrow
            if(c == '\033') {
                char seq[2];
                if(kbhit()) {
                    read(0, &seq[0], 1);
                    if(kbhit()) {
                        read(0, &seq[1], 1);
                        if(seq[0] == '[') {
                            switch (seq[1]) {
                                case 'A': if(current_x !=  1) {current_x = -1, current_y =  0;} break;
                                case 'B': if(current_x != -1) {current_x =  1, current_y =  0;} break;
                                case 'C': if(current_y != -1) {current_x =  0, current_y =  1;} break;
                                case 'D': if(current_y !=  1) {current_x =  0, current_y = -1;} break;
                            }
                        }
                    }
                }
            }
        }
        
        // update snake position
        std::pair<int, int> head = snake.front();
        std::pair<int, int> newHead = {head.first + current_y, head.second + current_x};

        if(newHead.first <= 0 || newHead.first >= fieldWidth - 1 || newHead.second <= 0 || newHead.second >= fieldHeight - 1) {
            game_over = true;
            break;
        }

        for(const auto& body : snake) {
            if(newHead == body) {
                game_over = true;
                break;
            }
        }
        if(newHead.first == food_y && newHead.second == food_x) {
            bool colision = false;
            score += 10;
            snake.push_front(newHead);
            do {
                food_x = dis_x(gen);
                food_y = dis_y(gen);
                colision = false;

                for(const auto& body: snake) {
                    if(body.first == food_y && body.second == food_x) {
                        colision = true; // The new food is on the snake body
                    }
                }
            } while (colision);
        }
        snake.push_front(newHead);
        snake.pop_back();

        std::string frame_buffer = "";
        for(int x = 0; x < fieldHeight; x++) {
            for(int y = 0; y < fieldWidth; y++) {
                if (y == 0 || y == fieldWidth - 1 || x == 0 || x == fieldHeight - 1) {
                    frame_buffer += "#";
                } else if (y == food_y && x == food_x) {
                    frame_buffer += "*";
                } else {
                    bool is_body = false;
                    for(const auto& body : snake) {
                        if(body.first == y && body.second == x) {
                            is_body = true;
                            break;
                        }
                    }
                    frame_buffer += (is_body ? "0" : " ");
                }
            }
            frame_buffer += "\n";
        }
        std::cout << "\033[2J\033[H" << frame_buffer << "\nCurrent Score = " << score << std::flush;
        if(current_x != 0) {
            usleep(120000);
        } else {
            usleep(100000);
        }
    }
    
    return 0;
}
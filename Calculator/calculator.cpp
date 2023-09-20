#include <iostream>
#include <queue>
#include <limits>

std::queue<float> Numbers = {};
std::queue<int> Type = {};

void inputNumbersAndOperations() {
    int interactionType = -1;
    int userCounter = 1;
    float inputNum;
    while (interactionType != 0) {
        while (true) {
            std::cout << "Please enter number #" << userCounter << ": ";
            std::cin >> inputNum;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
            } else {
                break;
            }
        }
        Numbers.push(inputNum);


        while (true) {
            std::cout << "Please enter interaction type: 0Finish, 1Add, 2Subt, 3Multi, 4Divide, 5Exponential, 6Floor: ";
            std::cin >> interactionType;
            if (std::cin.fail() || (interactionType < 0 || interactionType > 6)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 0 and 6." << std::endl;
            } else {
                break;
            }
        }
        Type.push(interactionType);
        userCounter++;
    }
}

template <typename T>
void print_queue(std::queue<T> q)
{
    while (!q.empty())
    {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
}

float calculate(std::queue<float> Nums, std::queue<int> Operations) {
    float currentRunning;
    int currentCommand;
    float currNum;
    currentRunning = Nums.front();
    Nums.pop();
    while (!Nums.empty()) {
        currNum = Nums.front();
        Nums.pop();
        currentCommand = Operations.front();
        Operations.pop();
        if (currentCommand == 1) {
            currentRunning += currNum;
        }
        if (currentCommand == 2) {
            currentRunning -= currNum;
        }
        if (currentCommand == 3) {
            currentRunning *= currNum;
        }
        if (currentCommand == 4) {
            currentRunning /= currNum;
        }
        if (currentCommand == 5) {
            float oldCurrentRunning = currentRunning;
            float holder = oldCurrentRunning;
            for (int i = 1; i < currNum; i++) {
                holder *= oldCurrentRunning;
            }
            if (currNum >= 1) {
                currentRunning = holder;
            } else {
                currentRunning = 1;
            }

        }
    }
    return currentRunning;
}

void dazzle() {
    std::cout << std::string(160, '=') << "\n";
}

int main() {
    dazzle();
    std::cout << "Welcome to the M-Type Calculator!" << std::endl;
    int decision = 1;
    // here's the twist: instead of being limited to two inputs per calculation, this calculator will allow many inputs, and even allow changes in interaction type.
    // new calculation begin:
    // 1) Enter number -> 2) select interaction type -> 3) enter number -> revert back to step 2 -> continue as usual
    // this will require
    std::cout << "The format of the calculations will be as follows: 1) Enter number A, 2) Select interaction type, 3) Enter number B, 4) Select interaction type, and so on. \n";
    dazzle();
    while (true) {
        inputNumbersAndOperations();
        //print_queue(Numbers);
        //print_queue(Type);
        std::cout << "The result was " << calculate(Numbers, Type) << "!\n";
        std::cout << "Would you like to continue? 0No 1Yes: ";
        std::cin >> decision;
        if (decision == 0) {
            std::cout << "\nThank you for choosing to use the M-Type Calculator!\n";
            return 0;
        }
        dazzle();
    }
}



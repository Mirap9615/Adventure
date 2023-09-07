//
// Created by William on 9/6/23.
//

#include "universal.h"

static std::map<int, std::string> items_all;
std::map<int, std::string>& ItemsAllHook() {
    return items_all;
}


void printAllItems() {
    std::map<int, std::string> itemList = ItemsAllHook();
    std::cout << "There are currently " << itemList.size() << " registered items." << std::endl;
    for (const auto& pair : itemList) {
        std::cout << "id: " << pair.first << " is " << pair.second << std::endl;
    }
}

void printSlowly(const std::string& text, int delay_ms) {
    for (char c : text) {
        std::cout << c;
        std::cout.flush(); // allows immediate printing
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}
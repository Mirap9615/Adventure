//
// Created by William on 9/6/23.
//

#include "universal.h"

std::map<int, std::shared_ptr<Object>> all_items;
std::map<int, std::shared_ptr<Object>>& ItemsAllHook() {
    return all_items;
}



void printSlowly(const std::string& text, int delay_ms) {
    for (char c : text) {
        std::cout << c;
        std::cout.flush(); // allows immediate printing
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}

Settings::Settings() : detailed_combat_text(false), text_crawl_speed_letter_ms(15) {
    //std::cout << "created once and only." << std::endl;
}
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "nlohmann/json.hpp"
#include <chrono>
#include <algorithm>
#include "beings.h"
#include "qol.h"
#include "objects.h"


// Global Variables
bool dev_mode = false;

// Pre declaration
class Object;
class Item;
class Weapon;
class Inventory;
class Party;
class Events;

void chapter_one(std::shared_ptr<Organism>& player);
std::map<int, std::unique_ptr<Monster>> monsters_all;



class Magical : public Organism {
public:
    explicit Magical(const std::string& given_name) : Organism(given_name) {
        magic_able = true;
        carcerisStrength = 150;
    }
    Magical(std::string inputName, const std::vector<float>& in_stats) : Organism(std::move(inputName)) {
        stats.board_change(in_stats);
    }

    void behavior() override {
        std::cout << "As a magic-capable, " << name << " is capable of performing mindless magic, while running and rolling." << std::endl;
    }

    void cast() override {
        std::cout << "If " << name << " was to cast magic, they would average " << stats.mag_atk.effectiveValue() << " magical damage per spell." << std::endl;
    }

    void increaseMagicalAtk(float inputValue) override {
        stats.mag_atk += inputValue;
    }

    // simple assignment operator overload (cause there is nothing)
    Magical& operator=(const Magical& other) {
        // 1. Check self assignment
        if (this == &other) {
            return *this;
        }

        // 2. Copy the base class members
        Organism::operator=(other);

        // 3. Copy the derived class members
        return *this;
    }

private:
};

class Monster : public Magical {
public:
    // name, stats, type, habitat
    Monster(const std::string& given_name, const std::vector<float>& in_stats, std::string given_type, std::string  given_habitat)
            : Magical(given_name, in_stats), type(std::move(given_type)), habitat(std::move(given_habitat)) {
    }

    // Getters for the new attributes
    std::string getType() const {
        return type;
    }

    std::string getHabitat() const {
        return habitat;
    }

    void behavior() override {
        Magical::behavior();  // Call base class behavior
        std::cout << "As a monster of type " << type << ", " << getName() << " resides in " << habitat << "." << std::endl;
    }

    // assignment operator overload
    Monster& operator=(const Monster& other) {
        if (this == &other) {
            return *this; // return *this to deal with self-assignment
        }

        // Copy the base class members
        Magical::operator=(other);

        // Copy the derived class members
        type = other.type;
        habitat = other.habitat;

        return *this;
    }

protected:
    std::string type;     // Monster type (e.g., "slime", "humanoid", etc.)
    std::string habitat;  // Habitat where the monster is found (e.g., "forest", "ocean", etc.)
};


class Sicut : public Magical {
public:
    explicit Sicut(const std::string& given_name) : Magical(given_name), charm(100, 100, 0,0) {
        balance = 0;
        reputation = 0;
    }
    Sicut(const std::string& inputName, const std::vector<float>& in_stats) : Magical(inputName), charm(100, 100, 0, 0) {
        stats.board_change(in_stats);
    }

    void behavior() override {
        std::cout << "As a sicut, " << name << " could potentially be performing mindful magic, while running. Probably would not roll, at least not in public." << std::endl;
    }

    // copy constructor (new thing)
    Sicut(const Sicut& other) : Magical(other), charm(other.charm), reputation(other.reputation) {
        // Assuming that the copy constructor for the Magical class and Attribute class handles everything appropriately
    }

    // copy assignment constructor (existing thing)
    Sicut& operator=(const Sicut& other) {
        // 1. Check self assignment
        if (this == &other) {
            return *this;  // Handle self-assignment
        }

        // 2. Call the base class copy assignment constructor overload
        Magical::operator=(other);

        // 3. Copy non-inherited, non-pointer members
        this->charm = other.charm;
        this->reputation = other.reputation;

        return *this;
    }

protected:
    double reputation;
    Attribute charm;
};

class Protagonist : public Sicut {
public:
    explicit Protagonist(const std::string& given_name) : Sicut(given_name) {}
    Protagonist(const std::string& inputName, const std::vector<float>& in_stats) : Sicut(inputName) {
        stats.board_change(in_stats);
    }

    void behavior() override {
        std::cout << "As the protagonist, " << name << " is capable of performing world-changing feats, perhaps of magic, and potentially while running. Probably would not roll, maybe not even in private." << std::endl;
    }
private:
    std::vector<std::string> current_quests;

};

class Events {
public:

private:
};

void printAllItems(const std::map<int, std::string>& itemList) {
    std::cout << "There are currently " << itemList.size() << " registered items." << std::endl;

    for (const auto& pair : itemList) {
        std::cout << "id: " << pair.first << " is " << pair.second << std::endl;
    }
}

void loadItems() {
    std::ifstream file("items.json");
    nlohmann::json json;
    file >> json;

    for (const auto& item : json) {
        int id = item["id"];
        std::string name = item["name"];
        //items_all[id] = name;
    }
}

std::unique_ptr<Monster> createBasicMonster(const std::string& name, const std::vector<float>& stats, const std::string& type, const std::string& habitat, const std::string& details = "A nameless monster.") {
    std::unique_ptr<Monster> monster = std::make_unique<Monster>(name, stats, type, habitat);
    monster->setDetails(details);
    return monster;
}

void loadMonsters() {
    std::ifstream file("monsters.json");
    nlohmann::json json_data;
    file >> json_data;

    for (const auto& entry : json_data) {
        int id = entry["id"];
        std::string name = entry["Name"];
        std::vector<float> stats = {entry["Hp"], entry["Mp"], entry["Stamina"], entry["Defense"],
                                    entry["Phys_Atk"], entry["Mag_Atk"], entry["Intelligence"]};
        // vector < Health	Mana	Stamina	Defense	Phys Atk	Mag Atk	Speed	Int >
        std::string type = entry["type"];
        std::string habitat = entry["habitat"];
        monsters_all[id] = createBasicMonster(name, stats, type, habitat);
    }
}

static std::unique_ptr<Organism> createOrganism(const int type, const std::string& name, const std::vector<float>& stats = {}) {
    if (type == 0) { // Protagonist
        return std::make_unique<Protagonist>(name, stats);
    } else if (type == 1) { // Sicut
        return std::make_unique<Magical>(name, stats);
    } else if (type == 2) { // Magical
        return std::make_unique<Magical>(name, stats);
    } else { // Organism
        return std::make_unique<Organism>(name, stats);
    }
}

static bool compareByRank(const std::pair<Organism*, PartyRole>& a, const std::pair<Organism*, PartyRole>& b) {
    return a.second > b.second;
}

std::string partyRoleToString(PartyRole role) {
    switch(role) {
        case INITIATE:
            return "Initiate";
        case MEMBER:
            return "Member";
        case OFFICER:
            return "Officer";
        case LEADER:
            return "Leader";
        case RULER:
            return "Ruler";
        default:
            return "Unknown";
    }
}

class Party {
private:
    std::vector<std::pair<Organism*, PartyRole>> members;
    std::string name;
    std::vector<std::string> history;

public:
    // Constructor
    Party(std::string input_name ) :name(std::move(input_name)) {}

    // Add a member to the party with a specific role

    void addMember(Organism* organism, PartyRole role = INITIATE) {
        members.emplace_back(organism, role);
        std::stringstream ss;
        ss << "Member added: " << organism << " as " << role;
        history.push_back(ss.str());
    }

    // Remove a member from the party
    void removeMember(Organism* organism) {
        members.erase(std::remove_if(members.begin(), members.end(),
                                     [&](const std::pair<Organism*, PartyRole>& member) {
                                         return member.first == organism;
                                     }),
                      members.end());
    }

    // Check if an organism is in the same party
    bool isInSameParty(Organism* organism) {
        for (const auto& member : members) {
            if (member.first == organism) {
                return true;
            }
        }
        return false;
    }

    // Method for promoting a member
    void promoteMember(Organism* issuer, Organism* target, PartyRole newRole) {
        for (auto & it : members) {
            Organism* curr_name = it.first;
            PartyRole& role = it.second;

            // Issuer must have a higher rank and cannot promote beyond their own rank
            if (curr_name == issuer && role > MEMBER && role >= newRole) {
                for (auto & member : members) {
                    Organism* curr_target = member.first;
                    PartyRole& curr_role = member.second;

                    if (curr_target == target && curr_role < role) {
                        curr_role = newRole;
                        std::cout << issuer->getName() << " promoted " << target->getName() << " to " << newRole << std::endl;
                        std::stringstream ss;
                        ss << issuer->getName() << " promoted " << target->getName() << " to " << newRole;
                        history.push_back(ss.str());
                        return;
                    }
                }
            }
        }
        std::cout << "Cannot promote: Either issuer or target not found, or issuer doesn't have the authority." << std::endl;
    }

    void removeMember(Organism* issuer, Organism* target) {
        for (auto it = members.begin(); it != members.end(); ++it) {
            Organism* member = it->first;
            PartyRole& role = it->second;

            if (member == issuer && role > MEMBER) {
                for (auto it2 = members.begin(); it2 != members.end(); ++it2) {
                    Organism* mem = it2->first;
                    PartyRole& rol = it2->second;

                    if (mem == target && rol < role) {
                        members.erase(it2);
                        std::cout << "Member " << target << " was removed by " << issuer << std::endl;
                        std::stringstream ss;
                        ss << issuer << " removed " << target;
                        history.push_back(ss.str());
                        return;
                    }
                }
            }
        }
        std::cout << "Cannot remove: Either issuer or target not found, or issuer doesn't have a high enough rank." << std::endl;
    }

    // Party merger
    Party& operator+=(Party& rhs) {
        for (auto& it : rhs.members) {
            Organism* member = it.first;
            PartyRole role = it.second;
            this->members.emplace_back(std::make_pair(member, role));
        }
        std::stringstream ss;
        ss << "Merged party " << rhs.name << " into " << this->name;
        history.push_back(ss.str());
        std::cout << "Merged party " << rhs.name << " into " << this->name << std::endl;
        rhs.members.clear(); // Clear the rhs party's member list
        return *this;
    }


    // Get the role of a party member
    PartyRole getRole(Organism* organism) {
        for (const auto& member : members) {
            if (member.first == organism) {
                return member.second;
            }
        }
        return INITIATE;  // Default if not found
    }

    void listMembers() {
        // Sort members by rank
        std::sort(members.begin(), members.end(), compareByRank);

        std::cout << "Members of Party " << name << ":" << std::endl;
        for (const auto& member : members) {
            std::cout << member.first->getName() << " (" << partyRoleToString(member.second) << ")" << std::endl;
        }
    }

    // Deep breaths everyone.
    /*
    bool party_duel(Party& rhs, int mode = 0) {
        std::vector<Organism*> partyA, partyB;

        // Populate partyA and partyB vectors with organisms from both parties (up to 4 each)
        int count = 0;
        for (auto& member : members) {
            if (count >= 4) break;
            partyA.push_back(member.first);
            count++;
        }
        count = 0;
        for (auto& member : rhs.members) {
            if (count >= 4) break;
            partyB.push_back(member.first);
            count++;
        }

        // Initialize a vector to store progress for each combatant
        std::vector<float> progressA(partyA.size(), 0.0f);
        std::vector<float> progressB(partyB.size(), 0.0f);

        // Main combat loop
        while (true) {
            // Update progress based on speed
            for (size_t i = 0; i < partyA.size(); ++i) {
                progressA[i] += partyA[i]->stats.spd.effectiveValue();
                if (progressA[i] >= 1000) {
                    // Find the weakest opponent in partyB
                    auto weakest_it = std::min_element(partyB.begin(), partyB.end(), [](Organism* a, Organism* b) {
                        return a->currentPower() < b->currentPower();
                    });

                    // Attack the weakest opponent (assuming attack logic in attackOpponent method)
                    partyA[i]->attackOpponent(**weakest_it, mode);

                    // Reset progress
                    progressA[i] = 0;
                }
            }

            // Similar logic for partyB
            for (size_t i = 0; i < partyB.size(); ++i) {
                progressB[i] += partyB[i]->stats.spd.effectiveValue();
                if (progressB[i] >= 1000) {
                    auto weakest_it = std::min_element(partyA.begin(), partyA.end(), [](Organism* a, Organism* b) {
                        return a->currentPower() < b->currentPower();
                    });
                    partyB[i]->attackOpponent(**weakest_it, mode);
                    progressB[i] = 0;
                }
            }

            // Remove dead combatants
            partyA.erase(std::remove_if(partyA.begin(), partyA.end(), [](Organism* org) { return org->stats.hp.effectiveValue() <= 0; }), partyA.end());
            partyB.erase(std::remove_if(partyB.begin(), partyB.end(), [](Organism* org) { return org->stats.hp.effectiveValue() <= 0; }), partyB.end());

            // Check win conditions
            if (partyA.empty()) {
                return false;
            } else if (partyB.empty()) {
                return true;
            }
        }
        */


};

std::unique_ptr<Organism> createProtagonist() {
    std::string name;
    std::cout << "Welcome to the Adventure Game (Sick title coming eventually)!\n";
    std::cout << "What would you like to be called?: ";
    std::getline(std::cin, name);

    auto protagonist = std::make_unique<Protagonist>(name);

    std::cout << "You are now " << name << ".\n";
    return protagonist;
}

void heckOff() {
    std::vector<std::unique_ptr<Organism>> organisms;
    std::cout << std::endl;
    std::unique_ptr<Organism> mae = std::make_unique<Protagonist>("Mae");
    std::unique_ptr<Organism> Pentalon = std::make_unique<Sicut>("Pentalon", std::vector<float>{325, 305, 700, 51, 16, 13, 138, 123});

    organisms.push_back(std::move(mae)); // 0
    organisms.push_back(createOrganism(4, "Zebra")); // 1
    organisms.push_back(createOrganism(1, "John")); // 2
    organisms.push_back(std::move(Pentalon)); // 3
    organisms.push_back(createOrganism(1,"Aruvious", {430, 560, 1150, 85, 28, 45, 150, 145})); // 4
    organisms.push_back(createOrganism(0, "Girl", {1400, 1000, 1600, 100, 55, 100, 300, 160})); // 5
    organisms.push_back(createOrganism(0, "Girlier", {1400, 1000, 1600, 100, 55, 100, 300, 160})); // 6
    organisms[5]->turn_combat(*organisms[6]);
    organisms[0]->turn_combat(*organisms[5]);
    organisms[5]->fullHeal();


    Organism* girlptr = organisms[5].get();
    Organism* maeptr = organisms[0].get();
    Organism* aruptr = organisms[4].get();

    // Add more organisms for testing party_duel
    organisms.push_back(createOrganism(1, "Vulcan", {1200, 900, 1300, 90, 50, 80, 250, 140})); // 7
    organisms.push_back(createOrganism(2, "Athena", {1100, 950, 1250, 88, 52, 85, 245, 145})); // 8
    organisms.push_back(createOrganism(3, "Thor", {1300, 850, 1350, 92, 48, 75, 255, 135}));  // 9
    organisms.push_back(createOrganism(4, "Apollo", {1150, 920, 1280, 89, 51, 82, 248, 142})); // 10

    Organism* vulcanptr = organisms[7].get();
    Organism* athenaptr = organisms[8].get();
    Organism* thorptr = organisms[9].get();
    Organism* apolloptr = organisms[10].get();

    Party Salam("Salam");
    Salam.addMember(girlptr, LEADER);
    Salam.addMember(maeptr, OFFICER);
    Salam.addMember(vulcanptr, MEMBER);
    Salam.addMember(athenaptr, INITIATE);

    Party Epoch("Epoch");
    Epoch.addMember(aruptr, LEADER);
    Epoch.addMember(thorptr, OFFICER);
    Epoch.addMember(apolloptr, MEMBER);
    Epoch.addMember(organisms[6].get(), INITIATE); // Girlier

    Salam.listMembers();
    Epoch.listMembers();
}

void preSetUp() {
    loadItems();
    loadMonsters();
}



void displayInitialLore() {
    printSlowly(
            "--- The Story Begins ---\n"
            "You find yourself in the glorious Empire of Conselle, one of the most powerful Empires Syurga has ever seen.\n"
            "Not long ago, you were expelled from the capital city of Mysynfo, after a misunderstanding with the ruling nobility.\n"
            "With limited options and the world seemingly against you, you head south to the city of Crende.\n"
            "Crende, the adventuring hub of the world, calls to you. Here, adventurers rise and fall like the tides, "
            "and stories of glory and despair turn into legends.\n"
            "In Crende, you can carve out a new life, reclaim your honor, and perhaps even change the course of all of Sicutity, if you become a Hero.\n"
            "But there is no way you will become a Hero. Not unless you carve out your own destiny."
    );

    printSlowly(
            "--- Chapter One: The New Beginning ---\n"
            "You make up your mind. You're going to become a great hero.\n"
            "After a long journey, you finally arrive in Crende — thirsty, starving, but full of hope.\n"
            "As you walk through the city, you marvel at its chaotic splendor. Yet, beneath it all, you feel an underlying sense of order, as if the city was controlled by invisible forces.\n"
            "You know you need to join the adventurers' guild. Finding the grandest building in downtown, you register as an adventurer.\n"
            "'You are an F10 rank adventurer, the lowest of the low,' says the clerk. You grit your teeth. 'I'll be A-rank soon enough, just you wait!'\n"
            "The hall erupts in laughter. Face burning, you run out of the hall."
            "You now stand in the middle of a busy city square, now a 'lowest of the low' tier adventurer. What do you do?\n"
    );

}

void choiceJob() {
    printSlowly(
            "You look around the city hoping to find a job, but people laugh in your face.\n"
            "'You're too weak,' they say to you, 'go get stronger. You can't even man a store.'\n"
            "You get mad and try to swing at someone. You instantly black out.\n"
            "You wake up a few hours later on the street, feeling saliva all over your body.\n"
            "You feel bloodied up. Did a random citizen really take you out with one punch?\n"
            "You realize nobody in this city is normal. You are in the land of wolves."
    );
}

void awardSlate(std::shared_ptr<Organism>& player, const Monster& defeated_monster) {
    float deservedSlate = calculateSlate(defeated_monster);
    player->earnSlate(deservedSlate);
    std::cout << "For their efforts, " << player->getName() << " has earned " << deservedSlate << " slate!" << std::endl;

}
int choiceSolo(std::shared_ptr<Organism>& player) {
    printSlowly(
            "You walk through the city and towards the southern gate — the boundary separating the known world from the unknown.\n"
            "As you pass through the gate, you feel energy flowing through you, making you tremble with excitement.\n"
            "You venture past the city's boundary into an endless expanse of lush forest. 'This is the land of monsters?' you ponder.\n"
            "You march forward into the unknown. Suddenly, something hits your leg hard. You recognize it as a slime and prepare for battle.\n"
            "Drawing your iron sword — a purchase that cost you the rest of your money - you prepare yourself for the fight."
    );
    // Prompt the player for a choice
    std::string choice;
    do {
        std::cout << "Do you wish to fight? (y/n): ";
        std::cin >> choice;
    } while (choice != "y" && choice != "n");

    if (choice == "n") {
        return 1;
    }

    // Create a slime from the predefined monsters
    Monster first_slime = *monsters_all[1];

    // Perform the combat
    player->turn_combat(first_slime, 0, true);

    // Congratulate the player
    awardSlate(player, *monsters_all[1]);
    printSlowly("Congratulations! You have the talents of a true adventurer.");
    printSlowly("\nReveling in the joy of killing monsters, you ventured deeper into the forest.");

    // Lore crawl for additional slimes defeated

    for (int i = 0; i < 3; ++i) {
        Monster another_slime = *monsters_all[1];
        player->turn_combat(another_slime, 0, true);
        awardSlate(player, *monsters_all[1]);
    }

    printSlowly("\nBefore you knew it, you had killed another three slimes.");



    printSlowly(
            "The sun was setting. You quickly gather your things and return to the safety of the city."
            );

    return 0;
}

void choiceParty() {
    printSlowly(
            "You return to the adventurers' guild, still smarting from the earlier humiliation.\n"
            "As you step in, the laughter resumes — apparently, adventurers have long memories.\n"
            "'Oh, its the big shot! You B rank yet?' A roguish adventurer smirked. \n"
            "Your cheeks burn with embarrassment, and you run out of the hall once more."
    );
}

void chapter_one(std::shared_ptr<Organism>& player) {
    std::vector<char> availableChoices {'a', 'b', 'c'};
    char choice;

    while (true) {
        if (std::find(availableChoices.begin(), availableChoices.end(), 'a') != availableChoices.end()) {
            std::cout << "a) Find a job in the city\n";
        }
        if (std::find(availableChoices.begin(), availableChoices.end(), 'b') != availableChoices.end()) {
            std::cout << "b) Immediately go adventuring solo\n";
        }
        if (std::find(availableChoices.begin(), availableChoices.end(), 'c') != availableChoices.end()) {
            std::cout << "c) Try to find an adventuring party to join\n";
        }
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 'a') {
            choiceJob();
            availableChoices.erase(std::remove(availableChoices.begin(), availableChoices.end(), 'a'), availableChoices.end());
        } else if (choice == 'b') {
            int result = choiceSolo(player);
            if (result == 0) {
                return;
            } else if (result == 1){
                printSlowly("You stand at the city square once again, confidence vastly lowered. "
                            "You know, deep down, that you were supposed to take that fight.");
            }
        } else if (choice == 'c') {
            choiceParty();
            availableChoices.erase(std::remove(availableChoices.begin(), availableChoices.end(), 'c'), availableChoices.end());
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void chapter_two(std::shared_ptr<Organism>& player) {

}

int main() {
    preSetUp();
    std::shared_ptr<Organism> player = createProtagonist();
    //displayInitialLore();
    chapter_one(player);
    chapter_two(player);

};
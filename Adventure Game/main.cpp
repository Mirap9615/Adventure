#include "universal.h"
#include "beings.h"
#include "qol.h"
#include "objects.h"
#include "inventory.h"
#include "monster.h"
#include "lore.h"


// Global Variables
bool dev_mode = false;

// Pre declaration
class Object;
class Item;
class Weapon;
class Party;
class Events;

std::map<int, std::unique_ptr<Monster>> monsters_all;

class Events {
public:

private:
};

void chapter_one(std::shared_ptr<Organism>& player);


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
    std::map<int, std::string> items_all = ItemsAllHook();

    for (const auto& item : json) {
        int id = item["id"];
        std::string name = item["name"];
        items_all[id] = name;
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

void menuChoice() {

}

void chapter_two(std::shared_ptr<Organism>& player) {
    returnToCity();
    menuChoice();
}

int main() {
    preSetUp();
    std::shared_ptr<Organism> player = createProtagonist();
    //displayInitialLore();
    chapter_one(player);
    chapter_two(player);

};
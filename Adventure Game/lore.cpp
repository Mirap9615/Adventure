//
// Created by William on 9/6/23.
//

#include "lore.h"

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

void choiceParty() {
    printSlowly(
            "You return to the adventurers' guild, still smarting from the earlier humiliation.\n"
            "As you step in, the laughter resumes — apparently, adventurers have long memories.\n"
            "'Oh, its the big shot! You B rank yet?' A roguish adventurer smirked. \n"
            "Your cheeks burn with embarrassment, and you run out of the hall once more."
    );
}

void returnToCity() {
    printSlowly(
            "As you walk back through the southern gate, you notice the sky has turned a deep shade of purple.\n"
            "'Damn. There'll be nothing open at this hour' you curse. But as you step through the gate back to the city, your mouth drops in shock.\n"
            "The street is full - full with street performers showcasing their talents, merchants passionately haggling with customers, \n"
            "and adventurers with their incessant chatter and boasting.\n"
            "You reach into your pocket and feel the weight of the Slates you earned from killing the slimes from earlier. A small reward, but a reward nonetheless.\n"
            "Your stomach growls. You catch a sniff of the wafting aroma of freshly cooked food and soon find yourself standing in front of a rustic establishment: The Fish's Tail.\n\n"
            "You push open the door, its creak barely audible over the clamor of conversation and laughter that fills the air. It is a lively place, to be sure.\n"
            "You take a seat at the bar, your eyes taking in the details: the worn wood of the stools, the intricate carvings on the walls, and the vast array of bottles that line the shelves.\n"
            "The bartender, a burly man with a thick beard, makes his way over to you. You can't help but think that this man could single-handedly wipe out a horde of slimes without breaking a sweat.\n"
            "He smirks, as if reading your thoughts. 'Your first day, eh?'\n"
            "You nod.\n'Be careful out there. The most dangerous time in an adventure is its beginning. Many talk big but few return.'\n"
            "'So, what would you like to order?'"
    );
}

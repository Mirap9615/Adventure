# Adventure

Multi file compile hierarchy:  

universal.h -> qol.h -> objects.h -> weapons.h -> inventory.h -> beings.h -> monsters.h   

Stats is a collection of Attributes  

Object -> Item -> Weapon  

Organism -> Magical -> Sicut -> Protagonist  

Sicut -> Vendor

Inventory: inventoryFull(), addItems(int id, int amount, std::map<int, std::string>& items_all)

Attribute types:
Organisms (1 to 9): 1 Health	2 Mana	3 Stamina 4	Defense	5 Phys Atk	6 Mag Atk	7 Speed	8 Int 9 Power  

Weapons (10 to 12): 10 Damage 11 Durability 12 Range  

Item maintypes:  
0 material  1 weapon 2 tool 3 consumable 4 armor

Item subtypes (for main type 0):
0 material

Item subtypes (for main type 3):
0 food
1 potion 

Item subtypes (for main type 4): 
0 hand 1 helmet 2 scarf 3 chestplate 4 rerebrace 5 leggings 6 shoes 7 ring 

OLD: 5 hand 6 helmet 7 Scarf 8 Chestplate 9 Rerebrace 10 Leggings 11 Shoes 12 Rings  

id occupied until 62
id types:
1 to 16: material  
17 to 19: tools
20 to 24: food
25 to 27: tools


Magic types:   
0 None 1 All 2 Fire, 3 Air, 4 Water, 5 Earth, 6 Light, 7 Dark, 8 Illusionary, 9 E&M    

Slot Numbers:
0 left hand 1 right hand 2 helmet 3 scarf 4 chestplate 5 rerebrace 6 leggings 7 left shoe 8 right shoe 9 left ring 10 right ring  


Slots rules:  

1. You can only equip a weapon on your hand slots
2. You can only equip a ring on your ring slot(s)
3.  You can only equip a material on your hand slots
4. You cannot equip a consumable or food on any slot that is not your hands 
5. You can only equip a helmet on your helmet slot, a chestplate on your chestplate slot, a legging on your leggings slot, a rerebrace on your rerebrace slot, a shoe on a shoe slot, and a scarf on a scarf slot, BUT you can also equip all armor types to your hands. 
6. You can only equip tools on your hands
7. You can only equip foods on your hands 
8. If you do not specify which slot you want to equip something, it will automatically equip it to your right hand. If that is full, your left hand. If both is full, it will reject you. 

the method getType will return that item's type:
type number 0 is material, type number 1 is weapon, type number 2 is tools, type number 3 is consumables, type number 4 is food, type number 5 is right hand, 6 is left hand, 7 is helmet, 8 is scarf, 9 is chestplate, 10 is rerebrace, 11 is leggings, 12 is right shoe, 13 is left shoe, 14 is right hand ring, 15 is left hand ring 




All Items list:
First, there are individual items: dirt, stone, sand, gravel, glass, wood, copper, tin, silver, lead, iron, cobalt, gold, platinum, natural diamond, titanium, bucket of water, bucket of lava, bucket of honey, apple, orange, grape, berry, stone fruit, empty bucket. 

Then there are different tiers of gear: wood, iron, platinum, titanium, orichalcum, mythium, odel, accursed, mystium, great, divine; Each gear tier should have a sword, axe, pickaxe, shovel, spear, bow, helmet, chestplate, leggings, boots, shield, block, and arrow. For example, wood_pickaxe should be an item, and so should odel_chestplate. 

 fish, cow, chicken, sheep, goat, pig, ant, cockroach, shark, bird, fox, lion, tiger, cat, rabbit, serpent, worm, skeleton, zombie, ghost, sandman, 

 Pending:  
 a right hand slot, a left hand slot, a helmet slot, a scarf slot, a chestplate slot, a rerebrace slot, a leggings slot, a right shoe slot, a left shoe slot, a left ring finger slot, and a right ring finger slot. They would be able to hold items that would stay in that slot.


 right_hand(nullptr), left_hand(nullptr), helmet(nullptr), scarf(nullptr),
              chestplate(nullptr), rerebrace(nullptr), leggings(nullptr),
              right_shoe(nullptr), left_shoe(nullptr), left_ring_finger(nullptr), right_ring_finger(nullptr)

Next thing to work on: selection-based (non-automated) combat system

# Adventure

Multi file compile hierarchy
universal.h -> qol.h -> objects.h -> weapons.h -> inventory.h -> beings.h -> monsters.h   

Stats is a collection of Attributes  

Object -> Item -> Weapon  

Organism -> Magical -> Sicut -> Protagonist  

Inventory: inventoryFull(), addItems(int id, int amount, std::map<int, std::string>& items_all)

Attribute types:
Organisms (1 to 9): 1 Health	2 Mana	3 Stamina 4	Defense	5 Phys Atk	6 Mag Atk	7 Speed	8 Int 9 Power  

Weapons (10 to 12): 10 Damage 11 Durability 12 Range  


Magic types:   
0 None 1 All 2 Fire, 3 Air, 4 Water, 5 Earth, 6 Light, 7 Dark, 8 Illusionary, 9 E&M   


All Items list:
First, there are individual items: dirt, stone, sand, gravel, glass, wood, copper, tin, silver, lead, iron, cobalt, gold, platinum, natural diamond, titanium, bucket of water, bucket of lava, bucket of honey, apple, orange, grape, berry, stone fruit, empty bucket. 

Then there are different tiers of gear: wood, iron, platinum, titanium, orichalcum, mythium, odel, accursed, mystium, great, divine; Each gear tier should have a sword, axe, pickaxe, shovel, spear, bow, helmet, chestplate, leggings, boots, shield, block, and arrow. For example, wood_pickaxe should be an item, and so should odel_chestplate. 

 fish, cow, chicken, sheep, goat, pig, ant, cockroach, shark, bird, fox, lion, tiger, cat, rabbit, serpent, worm, skeleton, zombie, ghost, sandman, 

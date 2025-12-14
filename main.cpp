#include <iostream>
#include <memory>
#include <limits>

#include "dungeon.h"
#include "observer.h"

void displayMenu() {
    std::cout << "\n=== Dungeon Editor for Balagur Fate 3 ===\n";
    std::cout << "1. Add new NPC\n";
    std::cout << "2. Print all NPCs\n";
    std::cout << "3. Save to file\n";
    std::cout << "4. Load from file\n";
    std::cout << "5. Start battle\n";
    std::cout << "6. Clear dungeon\n";
    std::cout << "0. Exit\n";
    std::cout << "=========================================\n";
    std::cout << "Choice: ";
}

int main() {
    Dungeon dungeon;
    
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    auto fileObserver = std::make_shared<FileObserver>("log.txt");
    
    dungeon.addBattleObserver(consoleObserver);
    dungeon.addBattleObserver(fileObserver);
    
    int choice;
    
    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        try {
            switch (choice) {
                case 1: {
                    std::string type, name;
                    int x, y;
                    
                    std::cout << "NPC type (Knight/Squirrel/Pegasus): ";
                    std::getline(std::cin, type);
                    
                    std::cout << "Name: ";
                    std::getline(std::cin, name);
                    
                    std::cout << "X coordinate (0-500): ";
                    std::cin >> x;
                    
                    std::cout << "Y coordinate (0-500): ";
                    std::cin >> y;
                    
                    dungeon.addNPC(type, name, x, y);
                    std::cout << "NPC added successfully!\n";
                    break;
                }
                    
                case 2:
                    dungeon.printAllNPCs();
                    break;
                    
                case 3: {
                    std::string filename;
                    std::cout << "Enter filename to save: ";
                    std::getline(std::cin, filename);
                    
                    dungeon.saveToFile(filename);
                    std::cout << "Dungeon saved to " << filename << std::endl;
                    break;
                }
                    
                case 4: {
                    std::string filename;
                    std::cout << "Enter filename to load: ";
                    std::getline(std::cin, filename);
                    
                    dungeon.loadFromFile(filename);
                    std::cout << "Dungeon loaded from " << filename << std::endl;
                    break;
                }
                    
                case 5: {
                    int range;
                    std::cout << "Enter battle range (meters): ";
                    std::cin >> range;
                    
                    std::cout << "\n=== Starting Battle ===\n";
                    dungeon.startBattle(range);
                    std::cout << "=== Battle Complete ===\n";
                    break;
                }
                    
                case 6: {
                    // Создаем новый dungeon для очистки
                    dungeon = Dungeon();
                    dungeon.addBattleObserver(consoleObserver);
                    dungeon.addBattleObserver(fileObserver);
                    std::cout << "Dungeon cleared!\n";
                    break;
                }
                    
                case 0:
                    std::cout << "Goodbye!\n";
                    break;
                    
                default:
                    std::cout << "Invalid choice. Try again.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
    } while (choice != 0);
    
    return 0;
}
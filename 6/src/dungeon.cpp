#include <fstream>
#include <iostream>
#include <memory>
#include <algorithm>

#include "dungeon.h"
#include "npc_factory.h"
#include "visitor.h"

Dungeon::Dungeon() : battleSubject(std::make_shared<BattleSubject>()) {}

bool Dungeon::isValidCoordinates(int x, int y) const {
    return x >= 0 && x <= 500 && y >= 0 && y <= 500;
}

void Dungeon::addNPC(const std::shared_ptr<NPC>& npc) {
    if (isValidCoordinates(npc->getX(), npc->getY())) {
        npcs.push_back(npc);
    } else {
        throw std::runtime_error("Invalid coordinates");
    }
}

void Dungeon::addNPC(const std::string& type, const std::string& name, int x, int y) {
    auto npc = NPCFactory::createNPC(type, name, x, y);
    addNPC(npc);
}

void Dungeon::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            file << NPCFactory::saveNPCToString(*npc) << std::endl;
        }
    }
    file.close();
}

void Dungeon::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading");
    }
    
    npcs.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            try {
                auto npc = NPCFactory::loadNPCFromString(line);
                addNPC(npc);
            } catch (const std::exception& e) {
                std::cerr << "Error loading NPC: " << e.what() << std::endl;
            }
        }
    }
    file.close();
}

void Dungeon::printAllNPCs() const {
    std::cout << "\n=== NPC List ===" << std::endl;
    std::cout << "Total: " << npcs.size() << " NPCs" << std::endl;
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        const auto& npc = npcs[i];
        if (npc->isAlive()) {
            std::cout << i+1 << ". " << npc->getType() 
                      << " '" << npc->getName() << "'"
                      << " at (" << npc->getX() << ", " << npc->getY() << ")"
                      << std::endl;
        } else {
            std::cout << i+1 << ". [DEAD] " << npc->getType() 
                      << " '" << npc->getName() << "'"
                      << std::endl;
        }
    }
    std::cout << "================\n" << std::endl;
}

void Dungeon::startBattle(int range) {
    BattleVisitor visitor(range);
    
    battleSubject->notifyObservers("Battle started with range " + std::to_string(range) + "m");
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            auto& npc1 = npcs[i];
            auto& npc2 = npcs[j];
            
            if (visitor.shouldFight(*npc1, *npc2)) {
                bool npc1WasAlive = npc1->isAlive();
                bool npc2WasAlive = npc2->isAlive();
                
                npc1->accept(visitor);
                npc2->accept(visitor); // посещаем 
                if (!npc1->isAlive() && npc1WasAlive) {
                    battleSubject->notifyObservers(
                        npc1->getType() + " '" + npc1->getName() + "' was killed by " +
                        npc2->getType() + " '" + npc2->getName() + "'"
                    );
                }
                if (!npc2->isAlive() && npc2WasAlive) {
                    battleSubject->notifyObservers(
                        npc2->getType() + " '" + npc2->getName() + "' was killed by " +
                        npc1->getType() + " '" + npc1->getName() + "'"
                    );
                }
            }
        }
    }
    
    removeDeadNPCs();
    battleSubject->notifyObservers("Battle ended. Remaining NPCs: " + std::to_string(npcs.size()));
}

void Dungeon::removeDeadNPCs() {
    npcs.erase(
        std::remove_if(npcs.begin(), npcs.end(),
            [](const std::shared_ptr<NPC>& npc) { return !npc->isAlive(); }),
        npcs.end()
    );
}

size_t Dungeon::getNPCCount() const {
    return npcs.size();
}

void Dungeon::addBattleObserver(std::shared_ptr<BattleObserver> observer) {
    battleSubject->addObserver(observer);
}
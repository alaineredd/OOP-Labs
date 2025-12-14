#include <iostream>
#include <random>
#include <iomanip>

#include "game.h"
#include "npc_factory.h"

Game::Game() : running(false), stopRequested(false) {
    dungeonMutex = os::createMutex();
    printMutex = os::createMutex();
    npcRWLock = os::createRWLock();
    
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    auto fileObserver = std::make_shared<FileObserver>("game_log.txt");
    dungeon.addBattleObserver(consoleObserver);
    dungeon.addBattleObserver(fileObserver);
    
    gameStartTime = 0;
    gameEndTime = 0;
}

Game::~Game() {
    stop();
    
    os::destroyRWLock(npcRWLock);
    os::destroyMutex(printMutex);
    os::destroyMutex(dungeonMutex);
}

void Game::initializeNPCs() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 2);
    std::uniform_int_distribution<> coordDist(0, MAP_WIDTH);
    
    const std::vector<std::string> npcTypes = {"Knight", "Squirrel", "Pegasus"};
    
    for (int i = 0; i < TOTAL_NPCS; ++i) {
        std::string type = npcTypes[typeDist(gen)];
        std::string name = type + "_" + std::to_string(i+1);
        int x = coordDist(gen);
        int y = coordDist(gen);
        
        os::writeLock(npcRWLock);
        dungeon.addNPC(type, name, x, y);
        os::unlockRWLock(npcRWLock);
    }
    
    std::cout << "Initialized " << TOTAL_NPCS << " NPCs" << std::endl;
}

void* Game::movementThreadFunc(void* arg) {
    Game* game = static_cast<Game*>(arg);
    game->processMovements();
    return nullptr;
}

void* Game::battleThreadFunc(void* arg) {
    Game* game = static_cast<Game*>(arg);
    game->processBattles();
    return nullptr;
}

void* Game::printThreadFunc(void* arg) {
    Game* game = static_cast<Game*>(arg);
    game->printMap();
    return nullptr;
}

void Game::processMovements() {
    while (running && !stopRequested) {
        os::sleep(100);
        
        os::writeLock(npcRWLock);
        
        auto npcs = dungeon.getNPCs();
        for (auto& npc : npcs) {
            if (npc->isAlive()) {
                npc->moveRandom(MAP_WIDTH, MAP_HEIGHT);
            }
        }
        
        for (size_t i = 0; i < npcs.size(); ++i) {
            if (!npcs[i]->isAlive()) continue;
            for (size_t j = i + 1; j < npcs.size(); ++j) {
                if (!npcs[j]->isAlive()) continue;
                if (battleVisitor.shouldFight(*npcs[i], *npcs[j])) {
                    npcs[i]->accept(battleVisitor, *npcs[j]); 
                }
            }
        }
        
        os::unlockRWLock(npcRWLock);
        dungeon.removeDeadNPCs();
    }
}


void Game::processBattles() {
    while (running && !stopRequested) {
        BattleTask task(nullptr, nullptr);
        
        if (battleVisitor.getBattleTask(task)) {
            os::writeLock(npcRWLock);
            
            if (task.attacker && task.defender && 
                task.attacker->isAlive() && task.defender->isAlive()) {
                battleVisitor.processBattle(*task.attacker, *task.defender);
            }
            
            os::unlockRWLock(npcRWLock);
            
            dungeon.removeDeadNPCs();
        }
        
        os::sleep(50);
    }
}

void Game::printMap() {
    while (running && !stopRequested) {
        os::sleep(1000);
        
        os::lockMutex(printMutex);
        std::cout << "\n=== Game Map ===" << std::endl;
        
        unsigned long long currentTime = os::getCurrentTimeMillis();
        if (currentTime >= gameEndTime) {
            std::cout << "Time remaining: 0s" << std::endl;
        } else {
            unsigned long long remaining = (gameEndTime - currentTime) / 1000;
            std::cout << "Time remaining: " << remaining << "s" << std::endl;
        }
        
        os::readLock(npcRWLock);
        dungeon.printAllNPCs();
        os::unlockRWLock(npcRWLock);
        
        os::unlockMutex(printMutex);
    }
}

void Game::run() {
    std::cout << "Starting game..." << std::endl;
    
    gameStartTime = os::getCurrentTimeMillis();
    gameEndTime = gameStartTime + (GAME_DURATION_SEC * 1000);
    
    initializeNPCs();
    
    running = true;
    stopRequested = false;
    
    movementThread = os::createThread(movementThreadFunc, this);
    battleThread = os::createThread(battleThreadFunc, this);
    printThread = os::createThread(printThreadFunc, this);
    
    std::cout << "Game started! Running for " << GAME_DURATION_SEC << " seconds..." << std::endl;
    
    unsigned long long currentTime = os::getCurrentTimeMillis();
    while (currentTime < gameEndTime && !stopRequested) {
        os::sleep(100);
        currentTime = os::getCurrentTimeMillis();
    }
    
    stop();
    
    std::cout << "\n=== Game Over ===" << std::endl;
    std::cout << "Survivors:" << std::endl;
    
    os::readLock(npcRWLock);
    dungeon.printAllNPCs();
    os::unlockRWLock(npcRWLock);
}

void Game::stop() {
    stopRequested = true;
    running = false;
    
    os::sleep(100);
    
    if (movementThread) {
        os::joinThread(movementThread);
        movementThread = 0;
    }
    if (battleThread) {
        os::joinThread(battleThread);
        battleThread = 0;
    }
    if (printThread) {
        os::joinThread(printThread);
        printThread = 0;
    }
    
    std::cout << "Game stopped." << std::endl;
}
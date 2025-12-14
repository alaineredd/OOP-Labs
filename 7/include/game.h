// game.h
#pragma once

#include <vector>
#include <memory>
#include <atomic>

#include "dungeon.h"
#include "visitor.h"
#include "os.h"

class Game {
private:
    static const int TOTAL_NPCS = 50;
    static const int MAP_WIDTH = 100;
    static const int MAP_HEIGHT = 100;
    static const int GAME_DURATION_SEC = 30;
    
    Dungeon dungeon;
    AsyncBattleVisitor battleVisitor;
    
    std::atomic<bool> running;
    std::atomic<bool> stopRequested;
    
    os::ThreadHandle movementThread;
    os::ThreadHandle battleThread;
    os::ThreadHandle printThread;
    
    os::MutexHandle dungeonMutex;
    os::MutexHandle printMutex;
    os::RWLockHandle npcRWLock;
    
    unsigned long long gameStartTime;
    unsigned long long gameEndTime;
    
    void initializeNPCs();
    
    void processMovements();
    void processBattles();
    void printMap();
    
    static void* movementThreadFunc(void* arg);
    static void* battleThreadFunc(void* arg);
    static void* printThreadFunc(void* arg);
    
public:
    Game();
    ~Game();
    
    void run();
    void stop();
};
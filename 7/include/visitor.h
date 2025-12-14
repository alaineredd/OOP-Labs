#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "os.h"

class NPC;
class Knight;
class Squirrel;
class Pegasus;

class BattleTask {
public:
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    
    BattleTask(std::shared_ptr<NPC> a, std::shared_ptr<NPC> d) 
        : attacker(a), defender(d) {}
};

class NPCVisitor {
public:
    virtual ~NPCVisitor() = default;
    
    virtual void visit(Knight& knight, NPC& other) = 0;
    virtual void visit(Squirrel& squirrel, NPC& other) = 0;
    virtual void visit(Pegasus& pegasus, NPC& other) = 0;
    
    virtual bool shouldFight(NPC& npc1, NPC& npc2) = 0;
    virtual void processBattle(NPC& attacker, NPC& defender) = 0;
};

class AsyncBattleVisitor : public NPCVisitor {
private:
    std::vector<BattleTask> battleQueue;
    os::MutexHandle queueMutex;
    os::CondHandle queueCond;
    
public:
    AsyncBattleVisitor();
    ~AsyncBattleVisitor();
    
    void visit(Knight& knight, NPC& other) override;
    void visit(Squirrel& squirrel, NPC& other) override;
    void visit(Pegasus& pegasus, NPC& other) override;
    
    bool shouldFight(NPC& npc1, NPC& npc2) override;
    void processBattle(NPC& attacker, NPC& defender) override;

    void addBattleTask(std::shared_ptr<NPC> attacker, std::shared_ptr<NPC> defender);
    bool getBattleTask(BattleTask& task);
    bool isEmpty() const;
};
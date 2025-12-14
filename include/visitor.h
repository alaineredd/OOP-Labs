#pragma once

#include "npc.h"

class Knight;
class Squirrel;
class Pegasus;

class NPCVisitor {
public:
    virtual ~NPCVisitor() = default;
    
    virtual void visit(Knight& knight, NPC& other) = 0;
    virtual void visit(Squirrel& squirrel, NPC& other) = 0;
    virtual void visit(Pegasus& pegasus, NPC& other) = 0;
};

class BattleVisitor : public NPCVisitor {
private:
    int battleRange;
    
public:
    BattleVisitor(int range);
    
    void visit(Knight& knight, NPC& other) override;
    void visit(Squirrel& squirrel, NPC& other) override;
    void visit(Pegasus& pegasus, NPC& other) override;
    
    bool shouldFight(NPC& npc1, NPC& npc2);
};

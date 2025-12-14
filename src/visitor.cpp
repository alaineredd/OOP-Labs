#include <iostream>

#include "visitor.h"
#include "npc.h"

BattleVisitor::BattleVisitor(int range) : battleRange(range) {}

void BattleVisitor::visit(Knight& knight, NPC& other) {
    if (other.getType() == "Squirrel" && knight.isInRange(other, battleRange)) {
        if (knight.isAlive() && other.isAlive()) {
            other.die();
        }
    }
}

void BattleVisitor::visit(Squirrel& squirrel, NPC& other) {
    if (other.getType() == "Pegasus" && squirrel.isInRange(other, battleRange)) {
        if (squirrel.isAlive() && other.isAlive()) {
            other.die();
        }
    }
}

void BattleVisitor::visit(Pegasus& pegasus, NPC& other) {}

bool BattleVisitor::shouldFight(NPC& npc1, NPC& npc2) {
    return npc1.isAlive() && npc2.isAlive() && 
           npc1.isInRange(npc2, battleRange) &&
           npc1.getType() != npc2.getType();
}
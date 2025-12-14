#include <iostream>

#include "visitor.h"
#include "npc.h"

AsyncBattleVisitor::AsyncBattleVisitor() {
    queueMutex = os::createMutex();
    queueCond = os::createCondition();
}

AsyncBattleVisitor::~AsyncBattleVisitor() {
    os::destroyCondition(queueCond);
    os::destroyMutex(queueMutex);
}

void AsyncBattleVisitor::visit(Knight& knight, NPC& other) {
    if (other.getType() == "Squirrel") {
        addBattleTask(knight.shared_from_this(), 
                      std::static_pointer_cast<NPC>(other.shared_from_this()));
    }
}

void AsyncBattleVisitor::visit(Squirrel& squirrel, NPC& other) {
    if (other.getType() == "Pegasus") {
        addBattleTask(squirrel.shared_from_this(), 
                      std::static_pointer_cast<NPC>(other.shared_from_this()));
    }
}

void AsyncBattleVisitor::visit(Pegasus& pegasus, NPC& other) {
}

bool AsyncBattleVisitor::shouldFight(NPC& npc1, NPC& npc2) {
    if (!npc1.isAlive() || !npc2.isAlive()) return false;
    
    if (!npc1.isInRange(npc2, std::min(npc1.getAttackDistance(), npc2.getAttackDistance()))) {
        return false;
    }
    
    std::string type1 = npc1.getType();
    std::string type2 = npc2.getType();
    
    if (type1 == "Knight" && type2 == "Squirrel") return true;
    if (type1 == "Squirrel" && type2 == "Pegasus") return true;
    
    return false;
}

void AsyncBattleVisitor::processBattle(NPC& attacker, NPC& defender) {
    int attackPower = attacker.rollDice();
    int defensePower = defender.rollDice();
    
    if (attackPower > defensePower) {
        defender.die();
        std::cout << attacker.getType() << " " << attacker.getName() 
                  << " killed " << defender.getType() << " " << defender.getName()
                  << " (" << attackPower << " vs " << defensePower << ")" << std::endl;
    } else {
        std::cout << attacker.getType() << " " << attacker.getName() 
                  << " failed to kill " << defender.getType() << " " << defender.getName()
                  << " (" << attackPower << " vs " << defensePower << ")" << std::endl;
    }
}

void AsyncBattleVisitor::addBattleTask(std::shared_ptr<NPC> attacker, std::shared_ptr<NPC> defender) {
    os::lockMutex(queueMutex);
    battleQueue.emplace_back(attacker, defender);
    os::signalCondition(queueCond);
    os::unlockMutex(queueMutex);
}

bool AsyncBattleVisitor::getBattleTask(BattleTask& task) {
    os::lockMutex(queueMutex);
    
    if (battleQueue.empty()) {
        os::unlockMutex(queueMutex);
        return false;
    }
    
    task = battleQueue.front();
    battleQueue.erase(battleQueue.begin());
    os::unlockMutex(queueMutex);
    return true;
}

bool AsyncBattleVisitor::isEmpty() const {
    os::lockMutex(queueMutex);
    bool empty = battleQueue.empty();
    os::unlockMutex(queueMutex);
    return empty;
}
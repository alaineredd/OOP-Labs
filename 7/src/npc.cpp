#include <cmath>
#include <iostream>
#include <random>

#include "npc.h"
#include "visitor.h"

std::mt19937 NPC::rng(std::random_device{}());
std::uniform_int_distribution<int> NPC::diceDist(1, 6);

NPC::NPC(const std::string& name, int x, int y, int moveDist, int attackDist) 
    : name(name), x(x), y(y), alive(true), 
      moveDistance(moveDist), attackDistance(attackDist) {}

std::string NPC::getName() const {
    return name;
}

int NPC::getX() const {
    return x;
}

int NPC::getY() const {
    return y;
}

bool NPC::isAlive() const {
    return alive;
}

void NPC::die() {
    alive = false;
}

int NPC::getMoveDistance() const {
    return moveDistance;
}

int NPC::getAttackDistance() const {
    return attackDistance;
}

void NPC::moveRandom(int maxX, int maxY) {
    if (!alive) return;
    
    std::uniform_int_distribution<int> dist(-moveDistance, moveDistance);
    int dx = dist(rng);
    int dy = dist(rng);
    
    int newX = x + dx;
    int newY = y + dy;

    if (newX < 0) newX = 0;
    if (newX > maxX) newX = maxX;
    if (newY < 0) newY = 0;
    if (newY > maxY) newY = maxY;
    
    x = newX;
    y = newY;
}

int NPC::rollDice() const {
    return diceDist(rng);
}

double NPC::distanceTo(const NPC& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx*dx + dy*dy);
}

bool NPC::isInRange(const NPC& other, int range) const {
    return distanceTo(other) <= range;
}

void NPC::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

Knight::Knight(const std::string& name, int x, int y) 
    : NPC(name, x, y, 30, 10) {}

std::string Knight::getType() const {
    return "Knight";
}

void Knight::accept(NPCVisitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

std::shared_ptr<NPC> Knight::clone() const {
    return std::make_shared<Knight>(*this);
}

Squirrel::Squirrel(const std::string& name, int x, int y) 
    : NPC(name, x, y, 5, 5) {}

std::string Squirrel::getType() const {
    return "Squirrel";
}

void Squirrel::accept(NPCVisitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

std::shared_ptr<NPC> Squirrel::clone() const {
    return std::make_shared<Squirrel>(*this);
}

Pegasus::Pegasus(const std::string& name, int x, int y) 
    : NPC(name, x, y, 30, 10) {}

std::string Pegasus::getType() const {
    return "Pegasus";
}

void Pegasus::accept(NPCVisitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

std::shared_ptr<NPC> Pegasus::clone() const {
    return std::make_shared<Pegasus>(*this);
}
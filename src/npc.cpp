#include <cmath>
#include <iostream>

#include "npc.h"
#include "visitor.h"

NPC::NPC(const std::string& name, int x, int y) 
    : name(name), x(x), y(y), alive(true) {}

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

double NPC::distanceTo(const NPC& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx*dx + dy*dy);
}

bool NPC::isInRange(const NPC& other, int range) const {
    return distanceTo(other) <= range;
}

Knight::Knight(const std::string& name, int x, int y) 
    : NPC(name, x, y) {}

std::string Knight::getType() const {
    return "Knight";
}

void Knight::accept(NPCVisitor& visitor) {
    visitor.visit(*this, *this);
}

Squirrel::Squirrel(const std::string& name, int x, int y) 
    : NPC(name, x, y) {}

std::string Squirrel::getType() const {
    return "Squirrel";
}

void Squirrel::accept(NPCVisitor& visitor) {
    visitor.visit(*this, *this);
}

Pegasus::Pegasus(const std::string& name, int x, int y) 
    : NPC(name, x, y) {}

std::string Pegasus::getType() const {
    return "Pegasus";
}

void Pegasus::accept(NPCVisitor& visitor) {
    visitor.visit(*this, *this);
}
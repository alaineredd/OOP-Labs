#pragma once

#include <string>
#include <memory>
#include <random>

class NPCVisitor;

class NPC : public std::enable_shared_from_this<NPC> {
protected:
    std::string name;
    int x;
    int y;
    bool alive;
    int moveDistance;
    int attackDistance;
    
    static std::mt19937 rng;
    static std::uniform_int_distribution<int> diceDist;
    
public:
    NPC(const std::string& name, int x, int y, int moveDist, int attackDist);
    virtual ~NPC() = default;
    
    std::string getName() const;
    int getX() const;
    int getY() const;
    bool isAlive() const;
    void die();
    int getMoveDistance() const;
    int getAttackDistance() const;
    
    virtual std::string getType() const = 0;
    virtual void accept(NPCVisitor& visitor, NPC& other) = 0;
    virtual std::shared_ptr<NPC> clone() const = 0;
    
    void moveRandom(int maxX, int maxY);
    int rollDice() const;
    double distanceTo(const NPC& other) const;
    bool isInRange(const NPC& other, int range) const;
    void setPosition(int newX, int newY);
};

class Knight : public NPC {
public:
    Knight(const std::string& name, int x, int y);
    std::string getType() const override;
    void accept(NPCVisitor& visitor, NPC& other) override;
    std::shared_ptr<NPC> clone() const override;
};

class Squirrel : public NPC {
public:
    Squirrel(const std::string& name, int x, int y);
    std::string getType() const override;
    void accept(NPCVisitor& visitor, NPC& other) override;
    std::shared_ptr<NPC> clone() const override;
};

class Pegasus : public NPC {
public:
    Pegasus(const std::string& name, int x, int y);
    std::string getType() const override;
    void accept(NPCVisitor& visitor, NPC& other) override;
    std::shared_ptr<NPC> clone() const override;
};
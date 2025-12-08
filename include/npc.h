#pragma once

#include <string>
#include <memory>

// Базовый класс для всех NPC
class NPC {
protected:
    std::string name;
    int x;
    int y;
    bool alive;

public:
    NPC(const std::string& name, int x, int y);
    virtual ~NPC() = default;
    
    virtual std::string getType() const = 0;
    virtual void accept(class NPCVisitor& visitor) = 0;
    
    std::string getName() const;
    int getX() const;
    int getY() const;
    bool isAlive() const;
    void die();
    
    double distanceTo(const NPC& other) const;
    bool isInRange(const NPC& other, int range) const;
};

class Knight : public NPC {
public:
    Knight(const std::string& name, int x, int y);
    std::string getType() const override;
    void accept(NPCVisitor& visitor) override;
};

class Squirrel : public NPC {
public:
    Squirrel(const std::string& name, int x, int y);
    std::string getType() const override;
    void accept(NPCVisitor& visitor) override;
};

class Pegasus : public NPC {
public:
    Pegasus(const std::string& name, int x, int y);
    std::string getType() const override;
    void accept(NPCVisitor& visitor) override;
};
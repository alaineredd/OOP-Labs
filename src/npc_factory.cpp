#include "npc_factory.hpp"
#include <sstream>
#include <iostream>

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, 
                                           const std::string& name, 
                                           int x, int y) {
    if (type == "Knight") {
        return std::make_shared<Knight>(name, x, y);
    } else if (type == "Squirrel") {
        return std::make_shared<Squirrel>(name, x, y);
    } else if (type == "Pegasus") {
        return std::make_shared<Pegasus>(name, x, y);
    }
    throw std::runtime_error("Unknown NPC type: " + type);
}

std::shared_ptr<NPC> NPCFactory::loadNPCFromString(const std::string& data) {
    std::istringstream iss(data);
    std::string type, name;
    int x, y;
    
    if (!(iss >> type >> name >> x >> y)) {
        throw std::runtime_error("Invalid NPC data format");
    }
    
    return createNPC(type, name, x, y);
}

std::string NPCFactory::saveNPCToString(const NPC& npc) {
    std::ostringstream oss;
    oss << npc.getType() << " " << npc.getName() << " " 
        << npc.getX() << " " << npc.getY();
    return oss.str();
}
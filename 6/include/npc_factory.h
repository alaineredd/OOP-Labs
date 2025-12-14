#pragma once

#include <memory>
#include <string>

#include "npc.h"

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(const std::string& type, 
                                          const std::string& name, 
                                          int x, int y);
    
    static std::shared_ptr<NPC> loadNPCFromString(const std::string& data);
    static std::string saveNPCToString(const NPC& npc);
};
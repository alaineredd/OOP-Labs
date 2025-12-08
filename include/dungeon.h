#include <memory>
#include <vector>
#include <string>

#include "npc.h"
#include "observer.h"

class Dungeon {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::shared_ptr<BattleSubject> battleSubject;
    
    bool isValidCoordinates(int x, int y) const;
    
public:
    Dungeon();
    
    void addNPC(const std::shared_ptr<NPC>& npc);
    void addNPC(const std::string& type, const std::string& name, int x, int y);
    
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    void printAllNPCs() const;
    
    void startBattle(int range);
    
    void removeDeadNPCs();
    size_t getNPCCount() const;
    
    void addBattleObserver(std::shared_ptr<BattleObserver> observer);
};
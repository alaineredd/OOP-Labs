#include <memory>
#include <string>
#include <vector>

#include "npc.h"

class BattleObserver {
public:
    virtual ~BattleObserver() = default;
    virtual void onBattleResult(const std::string& result) = 0;
};

class ConsoleObserver : public BattleObserver {
public:
    void onBattleResult(const std::string& result) override;
};

class FileObserver : public BattleObserver {
private:
    std::string filename;
    
public:
    FileObserver(const std::string& filename);
    void onBattleResult(const std::string& result) override;
};

class BattleSubject {
private:
    std::vector<std::shared_ptr<BattleObserver>> observers;
    
public:
    void addObserver(std::shared_ptr<BattleObserver> observer);
    void removeObserver(std::shared_ptr<BattleObserver> observer);
    void notifyObservers(const std::string& result);
};
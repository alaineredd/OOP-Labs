#include <iostream>
#include <fstream>
#include <ctime>
#include "observer.h"

void ConsoleObserver::onBattleResult(const std::string& result) {
    std::cout << "[BATTLE] " << result << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename(filename) {}

void FileObserver::onBattleResult(const std::string& result) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        std::time_t now = std::time(nullptr);
        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        file << "[" << timeStr << "] [BATTLE] " << result << std::endl;
        file.close();
    }
}

void BattleSubject::addObserver(std::shared_ptr<BattleObserver> observer) {
    observers.push_back(observer);
}

void BattleSubject::removeObserver(std::shared_ptr<BattleObserver> observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void BattleSubject::notifyObservers(const std::string& result) {
    for (const auto& observer : observers) {
        observer->onBattleResult(result);
    }
}
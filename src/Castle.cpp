#include <sstream>
#include <cstdlib>
#include <ctime>
#include <utility>
#include "../include/Castle.h"
#include "../include/Player.h"
#include "../include/DMCExceptions.h"

Castle::Castle() : roomName("Unknown Room"), artifactHidden(true), artifactName("Unknown Artifact") {}

Castle::Castle(const std::string& roomName, const std::string& artifactName)
    : roomName(roomName), artifactHidden(true), artifactName(artifactName) {}

// ===== Rule of Three Implementation =====

Castle::Castle(const Castle& other)
    : roomName(other.roomName), artifactHidden(other.artifactHidden), artifactName(other.artifactName) {
    std::cout << "[Castle Copy Constructor] Deep copying room: " << roomName << std::endl;
    for (const auto& demon : other.demons) {
        if (demon) {
            demons.push_back(demon->clone());
        }
    }
}

void swap(Castle& first, Castle& second) noexcept {
    using std::swap;
    swap(first.roomName, second.roomName);
    swap(first.artifactHidden, second.artifactHidden);
    swap(first.artifactName, second.artifactName);
    swap(first.demons, second.demons);
}

Castle& Castle::operator=(Castle other) {
    std::cout << "[Castle Copy Assignment] Deep copying via Copy-and-Swap for: " << other.roomName << std::endl;
    swap(*this, other);
    return *this;
}

Castle::~Castle() {
    std::cout << "[Castle Destructor] Cleared all demons from room: " << roomName << std::endl;
}

const std::string& Castle::getRoomName() const {
    return roomName;
}

const std::string& Castle::getArtifactName() const {
    return artifactName;
}

bool Castle::isArtifactHidden() const {
    return artifactHidden;
}

int Castle::getDemonCount() const {
    return static_cast<int>(demons.size());
}

int Castle::getAliveDemonCount() const {
    int count = 0;
    for (const auto& demon : demons) {
        if (demon && demon->isAlive()) {
            ++count;
        }
    }
    return count;
}

void Castle::addDemon(const Demon& demon) {
    demons.push_back(demon.clone());
}


std::string Castle::getRoomDescription() const {
    std::ostringstream oss;
    oss << "=== Room: " << roomName << " ===\n";
    oss << "Artifact: " << artifactName;
    if (artifactHidden) {
        oss << " (hidden)";
    } else {
        oss << " (FOUND!)";
    }
    oss << "\n";
    oss << "Demons (" << getAliveDemonCount() << "/" << demons.size() << " alive):\n";
    for (const auto& demon : demons) {
        if (demon) {
            oss << "  - " << *demon << "\n"; // Calling NVI print polimorfic
        }
    }
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Castle& castle) {
    os << "Castle[" << castle.roomName
       << " | Artifact: " << castle.artifactName
       << (castle.artifactHidden ? " (hidden)" : " (FOUND)")
       << " | Demons: [";
    for (int i = 0; i < static_cast<int>(castle.demons.size()); ++i) {
        if (i > 0) os << ", ";
        if (castle.demons[i]) {
            os << *(castle.demons[i]); // calling polymorphic display via NVI print
        } else {
            os << "nullptr";
        }
    }
    os << "]]";
    return os;
}



const std::vector<std::unique_ptr<Demon>>& Castle::getDemons() const {
    return demons;
}

void Castle::applyRoomEntryEffects(Player& player, std::vector<std::string>& log) {
    for (auto& demon : demons) {
        if (demon && demon->isAlive()) {
            if (auto* frost = dynamic_cast<FrostDemon*>(demon.get())) {
                log.push_back(">>> WARNING: " + frost->getName() + "'s freezing presence drops the room temperature! <<<");
                
                std::streambuf* oldCout = std::cout.rdbuf();
                std::ostringstream ss;
                std::cout.rdbuf(ss.rdbuf());
                
                frost->applyFrostDebuff(player);
                
                std::cout.rdbuf(oldCout);
                
                std::string captured = ss.str();
                std::istringstream iss(captured);
                std::string line;
                while (std::getline(iss, line)) {
                    if (!line.empty()) {
                        log.push_back(line);
                    }
                }
            }
        }
    }
}

void Castle::executeCombatRound(Player& player, Demon& target, std::vector<std::string>& log) {
    if (!player.isAlive() || !target.isAlive()) return;

    std::streambuf* oldCout = std::cout.rdbuf();
    std::ostringstream ss;
    std::cout.rdbuf(ss.rdbuf());

    // 1. Dante attacks
    player.attack(target);

    // 2. Demon retaliates if still alive
    if (target.isAlive() && player.isAlive()) {
        if (std::rand() % 100 < 30) {
            target.triggerSpecialAbility(player);
        } else {
            int demonDmg = target.attack();
            const Weapon& activeWeapon = player.getActiveWeapon();

            if (activeWeapon.getType() == WeaponType::Ranged && std::rand() % 100 < 35) {
                std::cout << "  >>> " << player.getName() << " keeps their distance! "
                          << target.getName() << " misses! <<<" << std::endl;
                demonDmg = 0;
            } else if (activeWeapon.getType() == WeaponType::Melee && std::rand() % 100 < 25) {
                std::cout << "  >>> " << player.getName() << " parries "
                          << target.getName() << "'s strike with " << activeWeapon.getName() << "! <<<" << std::endl;
                demonDmg = 0;
            }

            if (demonDmg > 0) {
                int actual = player.takeDamage(demonDmg);
                std::cout << "  [DEMON] " << target.getName()
                          << " strikes for " << actual << " damage!" << std::endl;
            }
        }
    }

    if (!target.isAlive()) {
        std::cout << "  *** " << target.getName() << " has been defeated! ***" << std::endl;
    }

    std::cout.rdbuf(oldCout);

    std::string captured = ss.str();
    std::istringstream iss(captured);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            log.push_back(line);
        }
    }

    // Reveal artifact if all demons are dead
    if (getAliveDemonCount() == 0 && artifactHidden) {
        artifactHidden = false;
        log.push_back("  *** All demons defeated! ***");
        log.push_back("  *** You discovered the artifact: " + artifactName + "! ***");
    }
}


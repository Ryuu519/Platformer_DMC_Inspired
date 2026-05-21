#include <sstream>
#include <cstdlib>
#include <ctime>
#include "../include/Castle.h"
#include "../include/Player.h"

Castle::Castle() : roomName("Unknown Room"), artifactHidden(true), artifactName("Unknown Artifact") {}

Castle::Castle(const std::string& roomName, const std::string& artifactName)
    : roomName(roomName), artifactHidden(true), artifactName(artifactName) {}

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
        if (demon.isAlive()) {
            ++count;
        }
    }
    return count;
}

void Castle::addDemon(const Demon& demon) {
    demons.push_back(demon);
}

bool Castle::exploreRoom(Player& player) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Entering: " << roomName << std::endl;
    std::cout << "========================================" << std::endl;

    if (demons.empty()) {
        std::cout << "The room is empty..." << std::endl;
    } else {
        std::cout << "Demons detected: " << getAliveDemonCount() << std::endl;
        std::cout << std::endl;

        // Fight each alive demon
        for (auto& demon : demons) {
            if (!demon.isAlive()) {
                continue;
            }
            if (!player.isAlive()) {
                std::cout << player.getName() << " has fallen in battle!" << std::endl;
                return false;
            }

            std::cout << "--- Battle: " << player.getName() << " vs " << demon.getName()
                      << " ---" << std::endl;
            std::cout << "  " << demon.getThreatLevel() << std::endl;

            // Combat loop: player attacks based on weapon speed, then demon retaliates
            while (demon.isAlive() && player.isAlive()) {
                const Weapon& activeWeapon = player.getActiveWeapon();
                // Number of attacks per turn is based on weapon speed (at least 1)
                int numAttacks = static_cast<int>(activeWeapon.getAttackSpeed() + 0.5);
                if (numAttacks < 1) numAttacks = 1;

                std::cout << "  [Turn Start] " << player.getName() << " attacks " << numAttacks 
                          << " time(s) with " << activeWeapon.getName() << "!" << std::endl;

                for (int a = 0; a < numAttacks && demon.isAlive(); ++a) {
                    player.attack(demon);
                }

                // Demon retaliates if still alive
                if (demon.isAlive() && player.isAlive()) {
                    int demonDmg = demon.attack();

                    // Distinct weapon gameplay effects:
                    if (activeWeapon.getType() == WeaponType::Ranged) {
                        // 40% chance the demon misses because player keeps distance with ranged weapon
                        if (std::rand() % 100 < 40) {
                            std::cout << "  >>> " << demon.getName() << " misses because " 
                                      << player.getName() << " is keeping distance with Ranged weapons! <<<" << std::endl;
                            demonDmg = 0;
                        }
                    } else { // Melee
                        // 25% chance to deflect/parry demon attacks with melee weapons
                        if (std::rand() % 100 < 25) {
                            std::cout << "  >>> " << player.getName() << " perfectly parries " 
                                      << demon.getName() << "'s strike with " << activeWeapon.getName() << "! <<<" << std::endl;
                            demonDmg = 0;
                        }
                    }

                    if (demonDmg > 0) {
                        int actualDmg = player.takeDamage(demonDmg);
                        std::cout << "  " << demon.getName() << " strikes back for "
                                  << actualDmg << " damage!" << std::endl;
                    }
                    std::cout << "  " << player.getName() << " HP: "
                              << player.getCurrentHealth() << "/" << player.getMaxHealth()
                              << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }

    // Check if all demons are dead -> reveal artifact
    if (getAliveDemonCount() == 0 && artifactHidden) {
        artifactHidden = false;
        std::cout << "*** All demons defeated! ***" << std::endl;
        std::cout << "*** You discovered the artifact: " << artifactName << "! ***" << std::endl;
        return true;
    }

    return !artifactHidden;
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
        oss << "  - " << demon << "\n";
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
        os << castle.demons[i]; // TRUE COMPOSITION calling Demon's operator<< !
    }
    os << "]]";
    return os;
}

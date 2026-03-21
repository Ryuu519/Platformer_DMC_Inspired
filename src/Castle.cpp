#include <sstream>
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

            // Combat loop: player attacks, then demon retaliates
            while (demon.isAlive() && player.isAlive()) {
                // Player attacks
                player.attack(demon);

                // Demon retaliates if still alive
                if (demon.isAlive()) {
                    int demonDmg = demon.attack();
                    int actualDmg = player.takeDamage(demonDmg);
                    std::cout << "  " << demon.getName() << " strikes back for "
                              << actualDmg << " damage!" << std::endl;
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
       << " | Demons: " << castle.getAliveDemonCount() << "/" << castle.demons.size()
       << " | Artifact: " << castle.artifactName
       << (castle.artifactHidden ? " (hidden)" : " (FOUND)")
       << "]";
    return os;
}

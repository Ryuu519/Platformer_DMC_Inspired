#include "../include/Demon.h"

static std::string demonTypeToString(DemonType type) {
    switch (type) {
        case DemonType::Scarecrow:   return "Scarecrow";
        case DemonType::HellPride:   return "Hell Pride";
        case DemonType::Frost:       return "Frost";
        case DemonType::ShadowDemon: return "Shadow Demon";
        case DemonType::Boss:        return "Boss";
    }
    return "Unknown";
}

Demon::Demon() : name("Unknown Demon"), type(DemonType::Scarecrow),
                 maxHealth(50), currentHealth(50), attackDamage(5), alive(true) {}

Demon::Demon(const std::string& name, DemonType type, int maxHealth, int attackDamage)
    : name(name), type(type), maxHealth(maxHealth), currentHealth(maxHealth),
      attackDamage(attackDamage), alive(true) {}

const std::string& Demon::getName() const {
    return name;
}

DemonType Demon::getType() const {
    return type;
}

int Demon::getCurrentHealth() const {
    return currentHealth;
}

int Demon::getMaxHealth() const {
    return maxHealth;
}

int Demon::getAttackDamage() const {
    return attackDamage;
}

bool Demon::isAlive() const {
    return alive;
}

int Demon::takeDamage(int amount) {
    if (!alive) {
        return 0;
    }
    int actualDamage = amount;
    if (actualDamage > currentHealth) {
        actualDamage = currentHealth;
    }
    currentHealth -= actualDamage;
    if (currentHealth <= 0) {
        currentHealth = 0;
        alive = false;
    }
    return actualDamage;
}

int Demon::attack() const {
    if (!alive) {
        return 0;
    }
    return attackDamage;
}

std::string Demon::getThreatLevel() const {
    if (!alive) {
        return name + " has been slain.";
    }
    double hpPercent = static_cast<double>(currentHealth) / maxHealth * 100.0;
    std::string threat;
    if (type == DemonType::Boss) {
        threat = "EXTREME";
    } else if (hpPercent > 75) {
        threat = "High";
    } else if (hpPercent > 40) {
        threat = "Medium";
    } else {
        threat = "Low";
    }
    return name + " (" + demonTypeToString(type) + ") - Threat: " + threat
           + " [HP: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth) + "]";
}

std::ostream& operator<<(std::ostream& os, const Demon& demon) {
    os << "Demon[" << demon.name << " | " << demonTypeToString(demon.type)
       << " | HP: " << demon.currentHealth << "/" << demon.maxHealth
       << " | ATK: " << demon.attackDamage
       << " | " << (demon.alive ? "Alive" : "Dead") << "]";
    return os;
}

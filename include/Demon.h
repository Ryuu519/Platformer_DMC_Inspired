#ifndef DEMON_H
#define DEMON_H

#include <iostream>
#include <string>

enum class DemonType {
    Scarecrow,
    HellPride,
    Frost,
    ShadowDemon,
    Boss
};

class Demon {
private:
    std::string name;
    DemonType type;
    int maxHealth;
    int currentHealth;
    int attackDamage;
    bool alive;

public:
    Demon();
    Demon(const std::string& name, DemonType type, int maxHealth, int attackDamage);

    const std::string& getName() const;
    DemonType getType() const;
    int getCurrentHealth() const;
    int getMaxHealth() const;
    int getAttackDamage() const;
    bool isAlive() const;

    /// Reduces health, updates alive status, returns actual damage taken
    int takeDamage(int amount);

    /// Performs an attack and returns the damage dealt
    int attack() const;

    /// Returns a threat description based on type and remaining HP percentage
    std::string getThreatLevel() const;

    friend std::ostream& operator<<(std::ostream& os, const Demon& demon);
};

#endif // DEMON_H

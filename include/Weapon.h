#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <string>

enum class WeaponType {
    Melee,
    Ranged
};

class Weapon {
private:
    std::string name;
    int damage;
    WeaponType type;
    double attackSpeed;

public:
    Weapon();
    Weapon(const std::string& name, int damage, WeaponType type, double attackSpeed);

    /// Converts WeaponType enum to its human-readable string name (static utility)
    static std::string typeToString(WeaponType type);

    const std::string& getName() const;
    int getDamage() const;
    WeaponType getType() const;
    double getAttackSpeed() const;

    /// Calculates effective DPS based on damage and attack speed
    double calculateDPS() const;

    /// Returns a stylized combat description for this weapon
    std::string getCombatDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);
};

#endif // WEAPON_H

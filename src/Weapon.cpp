#include "../include/Weapon.h"

Weapon::Weapon() : name("Unknown"), damage(0), type(WeaponType::Melee), attackSpeed(1.0) {}

Weapon::Weapon(const std::string& name, int damage, WeaponType type, double attackSpeed)
    : name(name), damage(damage), type(type), attackSpeed(attackSpeed) {}

const std::string& Weapon::getName() const {
    return name;
}

int Weapon::getDamage() const {
    return damage;
}

WeaponType Weapon::getType() const {
    return type;
}

double Weapon::getAttackSpeed() const {
    return attackSpeed;
}

double Weapon::calculateDPS() const {
    return damage * attackSpeed;
}

std::string Weapon::getCombatDescription() const {
    std::string desc = name;
    if (type == WeaponType::Melee) {
        desc += " slashes through the air";
    } else {
        desc += " fires a volley of bullets";
    }
    desc += " dealing " + std::to_string(damage) + " damage";
    return desc;
}

std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
    os << "Weapon[" << weapon.name << " | "
       << (weapon.type == WeaponType::Melee ? "Melee" : "Ranged")
       << " | DMG: " << weapon.damage
       << " | SPD: " << weapon.attackSpeed
       << " | DPS: " << weapon.calculateDPS() << "]";
    return os;
}

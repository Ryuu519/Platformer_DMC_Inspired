#include "../include/Weapon.h"
#include "../include/DMCExceptions.h"

Weapon::Weapon() : name("Unknown"), damage(1), type(WeaponType::Melee), attackSpeed(1.0) {}

Weapon::Weapon(const std::string& name, int damage, WeaponType type, double attackSpeed)
    : name(name), damage(damage), type(type), attackSpeed(attackSpeed) {
    if (name.empty()) {
        throw InvalidWeaponException("Weapon name cannot be empty!");
    }
    if (damage <= 0) {
        throw InvalidWeaponException("Weapon damage must be greater than 0! (Received: " + std::to_string(damage) + " for " + name + ")");
    }
    if (attackSpeed <= 0.0) {
        throw InvalidWeaponException("Weapon attack speed must be greater than 0! (Received: " + std::to_string(attackSpeed) + " for " + name + ")");
    }
}

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

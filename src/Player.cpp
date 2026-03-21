#include <cstring>
#include <sstream>
#include "../include/Player.h"
#include "../include/Demon.h"

// ===== Private helper =====

void Player::copyName(const char* src) {
    if (src != nullptr) {
        name = new char[std::strlen(src) + 1];
        std::strcpy(name, src);
    } else {
        name = new char[8];
        std::strcpy(name, "Unknown");
    }
}

// ===== Constructors =====

Player::Player()
    : name(nullptr), maxHealth(100), currentHealth(100),
      devilTriggerBar(0.0), devilTriggerMax(100.0),
      devilTriggerActive(false), activeWeaponIndex(-1) {
    copyName("Dante");
}

Player::Player(const char* name, int maxHealth, double dtMax)
    : name(nullptr), maxHealth(maxHealth), currentHealth(maxHealth),
      devilTriggerBar(0.0), devilTriggerMax(dtMax),
      devilTriggerActive(false), activeWeaponIndex(-1) {
    copyName(name);
}

// ===== Rule of Three =====

Player::Player(const Player& other)
    : name(nullptr), maxHealth(other.maxHealth), currentHealth(other.currentHealth),
      devilTriggerBar(other.devilTriggerBar), devilTriggerMax(other.devilTriggerMax),
      devilTriggerActive(other.devilTriggerActive), weapons(other.weapons),
      activeWeaponIndex(other.activeWeaponIndex) {
    copyName(other.name);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        delete[] name;
        copyName(other.name);
        maxHealth = other.maxHealth;
        currentHealth = other.currentHealth;
        devilTriggerBar = other.devilTriggerBar;
        devilTriggerMax = other.devilTriggerMax;
        devilTriggerActive = other.devilTriggerActive;
        weapons = other.weapons;
        activeWeaponIndex = other.activeWeaponIndex;
    }
    return *this;
}

Player::~Player() {
    delete[] name;
}

// ===== Getters =====

const char* Player::getName() const {
    return name;
}

int Player::getCurrentHealth() const {
    return currentHealth;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

double Player::getDevilTriggerBar() const {
    return devilTriggerBar;
}

bool Player::isDevilTriggerActive() const {
    return devilTriggerActive;
}

bool Player::isAlive() const {
    return currentHealth > 0;
}

int Player::getWeaponCount() const {
    return static_cast<int>(weapons.size());
}

const Weapon& Player::getActiveWeapon() const {
    return weapons[activeWeaponIndex];
}

// ===== Public member functions =====

void Player::equipWeapon(const Weapon& weapon) {
    weapons.push_back(weapon);
    if (activeWeaponIndex < 0) {
        activeWeaponIndex = 0;
    }
}

bool Player::switchWeapon(int index) {
    if (index >= 0 && index < static_cast<int>(weapons.size())) {
        activeWeaponIndex = index;
        return true;
    }
    return false;
}

void Player::activateDevilTrigger() {
    if (devilTriggerActive) {
        // Deactivate DT
        devilTriggerActive = false;
        std::cout << name << " deactivates Devil Trigger." << std::endl;
        return;
    }

    if (devilTriggerBar < 30.0) {
        std::cout << name << " does not have enough Devil Trigger energy! (need 30, have "
                  << devilTriggerBar << ")" << std::endl;
        return;
    }

    devilTriggerActive = true;

    // Heal 20% of max health on activation
    int healAmount = maxHealth / 5;
    currentHealth += healAmount;
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }

    std::cout << ">>> " << name << " activates DEVIL TRIGGER! <<<" << std::endl;
    std::cout << "    Healed for " << healAmount << " HP. Current HP: "
              << currentHealth << "/" << maxHealth << std::endl;
    std::cout << "    All attacks deal 2x damage!" << std::endl;
}

int Player::attack(Demon& target) {
    if (!isAlive() || activeWeaponIndex < 0) {
        return 0;
    }

    const Weapon& weapon = weapons[activeWeaponIndex];
    int baseDamage = weapon.getDamage();

    // Devil Trigger doubles damage
    int finalDamage = devilTriggerActive ? baseDamage * 2 : baseDamage;

    int actualDamage = target.takeDamage(finalDamage);

    std::cout << name << " attacks " << target.getName() << " with " << weapon.getName()
              << " for " << actualDamage << " damage";
    if (devilTriggerActive) {
        std::cout << " (DT BOOST!)";
    }
    std::cout << std::endl;

    // Build DT bar on hit (melee builds more)
    double dtGain = (weapon.getType() == WeaponType::Melee) ? 8.0 : 4.0;
    if (!devilTriggerActive) {
        devilTriggerBar += dtGain;
        if (devilTriggerBar > devilTriggerMax) {
            devilTriggerBar = devilTriggerMax;
        }
    } else {
        // DT drains while active
        devilTriggerBar -= 10.0;
        if (devilTriggerBar <= 0) {
            devilTriggerBar = 0;
            devilTriggerActive = false;
            std::cout << "    Devil Trigger has run out of energy!" << std::endl;
        }
    }

    if (!target.isAlive()) {
        std::cout << "    " << target.getName() << " has been slain!" << std::endl;
    }

    return actualDamage;
}

int Player::takeDamage(int amount) {
    if (!isAlive()) {
        return 0;
    }
    int actualDamage = amount;
    // DT reduces incoming damage by 50%
    if (devilTriggerActive) {
        actualDamage /= 2;
    }
    if (actualDamage > currentHealth) {
        actualDamage = currentHealth;
    }
    currentHealth -= actualDamage;
    return actualDamage;
}

std::string Player::getStatusReport() const {
    std::ostringstream oss;
    oss << "====== STATUS REPORT: " << name << " ======\n";
    oss << "  HP: " << currentHealth << "/" << maxHealth << "\n";
    oss << "  DT Bar: " << devilTriggerBar << "/" << devilTriggerMax;
    if (devilTriggerActive) {
        oss << " [ACTIVE]";
    }
    oss << "\n";
    oss << "  Weapons (" << weapons.size() << "):\n";
    for (int i = 0; i < static_cast<int>(weapons.size()); ++i) {
        oss << "    " << (i == activeWeaponIndex ? "-> " : "   ") << weapons[i] << "\n";
    }
    oss << "========================================";
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player[" << player.name
       << " | HP: " << player.currentHealth << "/" << player.maxHealth
       << " | DT: " << player.devilTriggerBar << "/" << player.devilTriggerMax;
    if (player.devilTriggerActive) {
        os << " ACTIVE";
    }
    os << " | Weapons: " << player.weapons.size() << "(";
    for (int i = 0; i < static_cast<int>(player.weapons.size()); ++i) {
        if (i > 0) os << ", ";
        os << player.weapons[i].getName();
        if (i == player.activeWeaponIndex) os << "*";
    }
    os << ")]";
    return os;
}

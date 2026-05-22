#include <sstream>
#include <algorithm>
#include "../include/Player.h"
#include "../include/Demon.h"
#include "../include/DMCExceptions.h"

// ===== Static Member Definition =====
int Player::s_totalCreated = 0;

// ===== Constructors =====

Player::Player()
    : name("Dante"), maxHealth(100), currentHealth(100),
      devilTriggerBar(0.0), devilTriggerMax(100.0),
      devilTriggerActive(false), activeWeaponIndex(-1) {
    ++s_totalCreated;
}

Player::Player(const std::string& name, int maxHealth, double dtMax)
    : name(name), maxHealth(maxHealth), currentHealth(maxHealth),
      devilTriggerBar(0.0), devilTriggerMax(dtMax),
      devilTriggerActive(false), activeWeaponIndex(-1) {
    if (name.empty()) {
        throw InvalidEntityException("Player name cannot be empty!");
    }
    if (maxHealth <= 0) {
        throw InvalidEntityException("Player max health must be greater than 0! (Received: " + std::to_string(maxHealth) + ")");
    }
    if (dtMax <= 0.0) {
        throw InvalidEntityException("Player max Devil Trigger energy must be greater than 0! (Received: " + std::to_string(dtMax) + ")");
    }
    ++s_totalCreated;
}

// ===== Rule of Three =====

Player::Player(const Player& other)
    : name(other.name), maxHealth(other.maxHealth), currentHealth(other.currentHealth),
      devilTriggerBar(other.devilTriggerBar), devilTriggerMax(other.devilTriggerMax),
      devilTriggerActive(other.devilTriggerActive), weapons(other.weapons),
      activeWeaponIndex(other.activeWeaponIndex) {
    ++s_totalCreated;
    std::cout << "[Player Copy Constructor] Copied: " << name << std::endl;
}

Player& Player::operator=(const Player& other) {
    std::cout << "[Player Copy Assignment] Assigning " << other.name << " to " << this->name << std::endl;
    if (this != &other) {
        name = other.name;
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
    std::cout << "[Player Destructor] Destroying player: " << name << std::endl;
}

// ===== Static Method =====

int Player::getPlayerCount() {
    return s_totalCreated;
}

// ===== Getters =====

const std::string& Player::getName() const {
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

const Weapon& Player::getActiveWeapon() const {
    if (activeWeaponIndex < 0 || activeWeaponIndex >= static_cast<int>(weapons.size())) {
        throw CombatStateException("No active weapon equipped!");
    }
    return weapons[activeWeaponIndex];
}

// ===== High-Level Aggregate Methods =====

double Player::getHealthRatio() const {
    return static_cast<double>(currentHealth) / maxHealth;
}

double Player::getDTRatio() const {
    return devilTriggerBar / devilTriggerMax;
}

std::string Player::getHealthString() const {
    return "HP: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
}

std::string Player::getDTString() const {
    std::string result = "DT: " + std::to_string(static_cast<int>(devilTriggerBar))
                       + "/" + std::to_string(static_cast<int>(devilTriggerMax));
    if (devilTriggerActive) {
        result += " [ACTIVE]";
    }
    return result;
}

std::string Player::getWeaponSummary() const {
    if (activeWeaponIndex < 0 || activeWeaponIndex >= static_cast<int>(weapons.size())) {
        return "No weapon equipped";
    }
    const Weapon& w = weapons[activeWeaponIndex];
    return w.getName() + " [" + Weapon::typeToString(w.getType()) + "] | DMG: "
         + std::to_string(w.getDamage()) + " | DPS: "
         + std::to_string(static_cast<int>(w.calculateDPS()));
}

// ===== Public Member Functions =====

void Player::equipWeapon(const Weapon& weapon) {
    weapons.push_back(weapon);
    if (activeWeaponIndex < 0) {
        activeWeaponIndex = 0;
    }
}

bool Player::switchWeapon(int index) {
    if (index < 0 || index >= static_cast<int>(weapons.size())) {
        throw CombatStateException("Cannot switch to weapon index " + std::to_string(index)
            + ". Dante only has " + std::to_string(weapons.size()) + " weapon(s) equipped!");
    }
    activeWeaponIndex = index;
    return true;
}

void Player::sortWeaponsByDPS() {
    std::sort(weapons.begin(), weapons.end(),
        [](const Weapon& a, const Weapon& b) {
            return a.calculateDPS() > b.calculateDPS(); // descending
        });
    // After sort, reset to best weapon (index 0)
    if (!weapons.empty()) {
        activeWeaponIndex = 0;
    }
}

void Player::activateDevilTrigger() {
    if (devilTriggerActive) {
        // Deactivate DT
        devilTriggerActive = false;
        std::cout << name << " deactivates Devil Trigger." << std::endl;
        return;
    }

    if (devilTriggerBar < 30.0) {
        throw CombatStateException("Not enough Devil Trigger energy! Dante has " + std::to_string(devilTriggerBar) + " energy, but activation requires 30.0!");
    }

    devilTriggerActive = true;

    // Heal 20% of max health on activation
    const int healAmount = maxHealth / 5;
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
    const int baseDamage = weapon.getDamage();

    // Devil Trigger doubles damage
    const int finalDamage = devilTriggerActive ? baseDamage * 2 : baseDamage;

    const int actualDamage = target.takeDamage(finalDamage);

    std::cout << name << " attacks " << target.getName() << " with " << weapon.getName()
              << " for " << actualDamage << " damage";
    if (devilTriggerActive) {
        std::cout << " (DT BOOST!)";
    }
    std::cout << std::endl;

    // Build DT bar on hit (melee builds more)
    const double dtGain = (weapon.getType() == WeaponType::Melee) ? 8.0 : 4.0;
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
    oss << "  " << getHealthString() << "\n";
    oss << "  " << getDTString() << "\n";
    oss << "  Weapons (" << weapons.size() << "):\n";

    // Use std::max_element (STL) to find the weapon with highest DPS
    const auto bestIt = weapons.empty() ? weapons.cend()
        : std::max_element(weapons.cbegin(), weapons.cend(),
            [](const Weapon& a, const Weapon& b) {
                return a.calculateDPS() < b.calculateDPS();
            });

    for (int i = 0; i < static_cast<int>(weapons.size()); ++i) {
        const bool isBest = !weapons.empty() && (weapons.cbegin() + i == bestIt);
        oss << "    " << (i == activeWeaponIndex ? "-> " : "   ") << weapons[i];
        if (isBest) { oss << " [BEST DPS]"; }
        oss << "\n";
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
    os << " | Weapons: [";
    for (int i = 0; i < static_cast<int>(player.weapons.size()); ++i) {
        if (i > 0) os << ", ";
        os << player.weapons[i]; // TRUE COMPOSITION calling Weapon's operator<< !
        if (i == player.activeWeaponIndex) os << "*";
    }
    os << "]]";
    return os;
}

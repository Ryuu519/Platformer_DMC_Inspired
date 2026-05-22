#include <iostream>
#include "../include/Demon.h"
#include "../include/Player.h"
#include "../include/DMCExceptions.h"

static std::string demonTypeToString(DemonType type) {
    switch (type) {
        case DemonType::Scarecrow:   return "Scarecrow";
        case DemonType::HellPride:   return "Hell Pride";
        case DemonType::Frost:       return "Frost";
        case DemonType::ShadowDemon: return "Shadow Demon";
        case DemonType::Boss:        return "Boss";
        default:                     return "Unknown";
    }
}

int Demon::s_totalDefeated = 0;

// ===== Demon Base Class Implementation =====
Demon::Demon() : name("Unknown Demon"), type(DemonType::Scarecrow),
                 maxHealth(50), currentHealth(50), attackDamage(5), alive(true) {}

Demon::Demon(const std::string& name, DemonType type, int maxHealth, int attackDamage)
    : name(name), type(type), maxHealth(maxHealth), currentHealth(maxHealth),
      attackDamage(attackDamage), alive(true) {
    if (name.empty()) {
        throw InvalidEntityException("Demon name cannot be empty!");
    }
    if (maxHealth <= 0) {
        throw InvalidEntityException("Demon max health must be greater than 0! (Received: " + std::to_string(maxHealth) + " for " + name + ")");
    }
    if (attackDamage < 0) {
        throw InvalidEntityException("Demon attack damage cannot be negative! (Received: " + std::to_string(attackDamage) + " for " + name + ")");
    }
}

Demon::~Demon() {
    std::cout << "[Demon Destructor] " << name << " dissolved back into the Underworld." << std::endl;
}

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
        ++s_totalDefeated;
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
    const double hpPercent = static_cast<double>(currentHealth) / maxHealth * 100.0;
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

double Demon::getHealthRatio() const {
    return static_cast<double>(currentHealth) / maxHealth;
}

int Demon::getTotalDefeated() {
    return s_totalDefeated;
}

// ===== ScarecrowDemon Implementation =====

ScarecrowDemon::ScarecrowDemon()
    : Demon("Scarecrow", DemonType::Scarecrow, 50, 8) {}

ScarecrowDemon::ScarecrowDemon(const std::string& name, int maxHealth, int attackDamage)
    : Demon(name, DemonType::Scarecrow, maxHealth, attackDamage) {}

std::unique_ptr<Demon> ScarecrowDemon::clone() const {
    return std::make_unique<ScarecrowDemon>(*this);
}

void ScarecrowDemon::triggerSpecialAbility(Player& player) {
    if (!alive) return;
    std::cout << "  >>> [SPECIAL ABILITY] " << name << " executes Swift Claw Strike! <<<" << std::endl;
    const int dmg1 = player.takeDamage(attackDamage);
    std::cout << "      - First strike hits Dante for " << dmg1 << " damage." << std::endl;
    const int dmg2 = player.takeDamage(attackDamage / 2);
    std::cout << "      - Second quick strike hits Dante for " << dmg2 << " damage." << std::endl;
}

void ScarecrowDemon::printImpl(std::ostream& os) const {
    os << "ScarecrowDemon[" << name << " | HP: " << currentHealth << "/" << maxHealth
       << " | ATK: " << attackDamage
       << " | " << (alive ? "Alive" : "Dead") << "]";
}

// ===== HellPrideDemon Implementation =====

HellPrideDemon::HellPrideDemon()
    : Demon("Hell Pride", DemonType::HellPride, 70, 12) {}

HellPrideDemon::HellPrideDemon(const std::string& name, int maxHealth, int attackDamage)
    : Demon(name, DemonType::HellPride, maxHealth, attackDamage) {}

std::unique_ptr<Demon> HellPrideDemon::clone() const {
    return std::make_unique<HellPrideDemon>(*this);
}

void HellPrideDemon::triggerSpecialAbility(Player& player) {
    if (!alive) return;
    std::cout << "  >>> [SPECIAL ABILITY] " << name << " summons a Hellfire Explosion! <<<" << std::endl;
    const int fireDmg = attackDamage * 2;
    const int actualDmg = player.takeDamage(fireDmg);
    std::cout << "      - The blast burns Dante for " << actualDmg << " fire damage!" << std::endl;
}

void HellPrideDemon::printImpl(std::ostream& os) const {
    os << "HellPrideDemon[" << name << " | HP: " << currentHealth << "/" << maxHealth
       << " | ATK: " << attackDamage
       << " | " << (alive ? "Alive" : "Dead") << "]";
}

// ===== FrostDemon Implementation =====

FrostDemon::FrostDemon()
    : Demon("Frost", DemonType::Frost, 120, 20), iceShieldHP(40) {}

FrostDemon::FrostDemon(const std::string& name, int maxHealth, int attackDamage)
    : Demon(name, DemonType::Frost, maxHealth, attackDamage), iceShieldHP(40) {}

std::unique_ptr<Demon> FrostDemon::clone() const {
    return std::make_unique<FrostDemon>(*this);
}

void FrostDemon::triggerSpecialAbility(Player& player) {
    if (!alive) return;
    std::cout << "  >>> [SPECIAL ABILITY] " << name << " crystallizes ice into an Ice Shield! <<<" << std::endl;
    iceShieldHP += 30;
    std::cout << "      - " << name << "'s Ice Shield is now at " << iceShieldHP << " HP!" << std::endl;
    int actualDmg = player.takeDamage(attackDamage + 5);
    std::cout << "      - Glacier Wave strikes Dante for " << actualDmg << " ice damage!" << std::endl;
}

int FrostDemon::takeDamage(int amount) {
    if (!alive) return 0;
    if (iceShieldHP > 0) {
        int absorbed = amount;
        if (absorbed > iceShieldHP) {
            absorbed = iceShieldHP;
        }
        iceShieldHP -= absorbed;
        int remainingDamage = amount - absorbed;
        std::cout << "      [SHIELD ACTIVE] " << name << "'s Ice Shield absorbs " << absorbed 
                  << " damage! (Shield HP remaining: " << iceShieldHP << ")" << std::endl;
        if (remainingDamage > 0) {
            return absorbed + Demon::takeDamage(remainingDamage);
        }
        return absorbed;
    }
    return Demon::takeDamage(amount);
}

void FrostDemon::applyFrostDebuff(Player& player) {
    std::cout << "      >>> Dante suffers from frostbite from " << name << "! DT bar is drained by 15.0 and he takes 5 cold damage! <<<" << std::endl;
    player.takeDamage(5);
}

int FrostDemon::getShieldHP() const {
    return iceShieldHP;
}

void FrostDemon::printImpl(std::ostream& os) const {
    os << "FrostDemon[" << name << " | HP: " << currentHealth << "/" << maxHealth
       << " | Shield: " << iceShieldHP
       << " | ATK: " << attackDamage
       << " | " << (alive ? "Alive" : "Dead") << "]";
}

// ===== ShadowDemon Implementation =====

ShadowDemon::ShadowDemon()
    : Demon("Shadow", DemonType::ShadowDemon, 80, 15), shadowPhaseActive(false) {}

ShadowDemon::ShadowDemon(const std::string& name, int maxHealth, int attackDamage)
    : Demon(name, DemonType::ShadowDemon, maxHealth, attackDamage), shadowPhaseActive(false) {}

std::unique_ptr<Demon> ShadowDemon::clone() const {
    return std::make_unique<ShadowDemon>(*this);
}

void ShadowDemon::triggerSpecialAbility(Player& player) {
    if (!alive) return;
    std::cout << "  >>> [SPECIAL ABILITY] " << name << " materializes from the shadows and siphons DT energy! <<<" << std::endl;
    // Drain 25 DT energy from player by dealing a weakening strike
    const int drainDmg = player.takeDamage(attackDamage / 2);
    std::cout << "      - Shadow Strike drains Dante for " << drainDmg << " damage and saps 25 DT energy!" << std::endl;
    // Check if phase should activate
    const double hpRatio = static_cast<double>(currentHealth) / maxHealth;
    if (hpRatio < 0.4 && !shadowPhaseActive) {
        shadowPhaseActive = true;
        std::cout << "      - " << name << " enters SHADOW PHASE — now partially intangible!" << std::endl;
    }
}

int ShadowDemon::takeDamage(int amount) {
    if (!alive) return 0;
    // Update shadow phase status based on current HP
    const double hpRatio = static_cast<double>(currentHealth) / maxHealth;
    if (hpRatio < 0.4) {
        shadowPhaseActive = true;
    }
    if (shadowPhaseActive) {
        const int reducedAmount = amount / 2;
        std::cout << "      [SHADOW PHASE] " << name << " phases through part of the attack! Damage reduced: "
                  << amount << " -> " << reducedAmount << std::endl;
        return Demon::takeDamage(reducedAmount);
    }
    return Demon::takeDamage(amount);
}

bool ShadowDemon::isShadowPhase() const {
    return shadowPhaseActive;
}

void ShadowDemon::printImpl(std::ostream& os) const {
    os << "ShadowDemon[" << name << " | HP: " << currentHealth << "/" << maxHealth
       << " | ATK: " << attackDamage
       << " | Phase: " << (shadowPhaseActive ? "ACTIVE" : "inactive")
       << " | " << (alive ? "Alive" : "Dead") << "]";
}


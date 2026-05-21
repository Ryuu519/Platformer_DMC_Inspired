#ifndef DEMON_H
#define DEMON_H

#include <iostream>
#include <string>
#include <memory>

enum class DemonType {
    Scarecrow,
    HellPride,
    Frost,
    ShadowDemon,
    Boss
};

class Player; // forward declaration to avoid circular dependency

class Demon {
protected:
    std::string name;
    DemonType type;
    int maxHealth;
    int currentHealth;
    int attackDamage;
    bool alive;

public:
    Demon();
    Demon(const std::string& name, DemonType type, int maxHealth, int attackDamage);
    virtual ~Demon();

    // Virtual constructor (clone method)
    virtual std::unique_ptr<Demon> clone() const = 0;

    const std::string& getName() const;
    DemonType getType() const;
    int getCurrentHealth() const;
    int getMaxHealth() const;
    int getAttackDamage() const;
    bool isAlive() const;

    /// Reduces health, updates alive status, returns actual damage taken
    virtual int takeDamage(int amount);

    /// Performs an attack and returns the damage dealt
    int attack() const;

    /// Returns a threat description based on type and remaining HP percentage
    std::string getThreatLevel() const;

    /// Specific virtual function representing a special gameplay mechanic
    virtual void triggerSpecialAbility(Player& player) = 0;

    // NVI (Non-Virtual Interface) pattern for display
    void print(std::ostream& os) const {
        printImpl(os);
    }

protected:
    // Pure virtual helper function for printing, implemented by derived classes
    virtual void printImpl(std::ostream& os) const = 0;
};

// Global operator<< using the NVI print function
inline std::ostream& operator<<(std::ostream& os, const Demon& demon) {
    demon.print(os);
    return os;
}

// ============================================================================
// Derived Class 1: ScarecrowDemon (Swift and tricky, can double attack when low)
// ============================================================================
class ScarecrowDemon : public Demon {
public:
    ScarecrowDemon();
    ScarecrowDemon(const std::string& name, int maxHealth, int attackDamage);

    std::unique_ptr<Demon> clone() const override;
    void triggerSpecialAbility(Player& player) override;

protected:
    void printImpl(std::ostream& os) const override;
};

// ============================================================================
// Derived Class 2: HellPrideDemon (Aggressive, unleashes exploding hellfire)
// ============================================================================
class HellPrideDemon : public Demon {
public:
    HellPrideDemon();
    HellPrideDemon(const std::string& name, int maxHealth, int attackDamage);

    std::unique_ptr<Demon> clone() const override;
    void triggerSpecialAbility(Player& player) override;

protected:
    void printImpl(std::ostream& os) const override;
};

// ============================================================================
// Derived Class 3: FrostDemon (Ice-based, freezing presence and armor)
// ============================================================================
class FrostDemon : public Demon {
private:
    int iceShieldHP;

public:
    FrostDemon();
    FrostDemon(const std::string& name, int maxHealth, int attackDamage);

    std::unique_ptr<Demon> clone() const override;
    void triggerSpecialAbility(Player& player) override;
    int takeDamage(int amount) override;

    // Specific meaningful helper methods
    void applyFrostDebuff(Player& player);
    int getShieldHP() const;

protected:
    void printImpl(std::ostream& os) const override;
};

#endif // DEMON_H

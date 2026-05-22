#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include "Weapon.h"

class Demon; // forward declaration

class Player {
private:
    std::string name;
    int maxHealth;
    int currentHealth;
    double devilTriggerBar;  // 0.0 to 100.0
    double devilTriggerMax;
    bool devilTriggerActive;
    std::vector<Weapon> weapons;
    int activeWeaponIndex;

public:
    Player();
    Player(const std::string& name, int maxHealth, double dtMax);

    // Rule of Five
    Player(const Player& other);
    Player(Player&& other) noexcept = default;
    Player& operator=(const Player& other);
    Player& operator=(Player&& other) noexcept = default;
    ~Player();

    const std::string& getName() const;
    int getCurrentHealth() const;
    int getMaxHealth() const;
    double getDevilTriggerBar() const;
    bool isDevilTriggerActive() const;
    bool isAlive() const;
    int getWeaponCount() const;
    const Weapon& getActiveWeapon() const;

    /// Adds a weapon to the player's arsenal
    void equipWeapon(const Weapon& weapon);

    /// Switches to a weapon by index (0-based), returns true if valid
    bool switchWeapon(int index);

    /// Activates/deactivates Devil Trigger - heals and boosts damage when activated
    void activateDevilTrigger();

    /// Attacks a demon with the active weapon, applying DT multiplier if active.
    /// Builds Devil Trigger bar on hit. Returns damage dealt.
    int attack(Demon& target);

    /// Takes damage from an enemy, reduces DT bar. Returns actual damage taken.
    int takeDamage(int amount);

    /// Displays a full status report of the player
    std::string getStatusReport() const;

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif // PLAYER_H

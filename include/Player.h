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

    static int s_totalCreated; ///< Total Player instances ever created across the session

public:
    Player();
    Player(const std::string& name, int maxHealth, double dtMax);

    // Rule of Five
    Player(const Player& other);
    Player(Player&& other) noexcept = default;
    Player& operator=(const Player& other);
    Player& operator=(Player&& other) noexcept = default;
    ~Player();

    /// Returns total Player objects ever constructed (static session counter)
    static int getPlayerCount();

    const std::string& getName() const;
    int getCurrentHealth() const;
    int getMaxHealth() const;
    double getDevilTriggerBar() const;
    bool isDevilTriggerActive() const;
    bool isAlive() const;
    const Weapon& getActiveWeapon() const;

    /// Returns currentHealth / maxHealth ratio (0.0–1.0) — used for bar rendering
    double getHealthRatio() const;

    /// Returns devilTriggerBar / devilTriggerMax ratio (0.0–1.0) — used for bar rendering
    double getDTRatio() const;

    /// Returns formatted "HP: X/Y" string for display
    std::string getHealthString() const;

    /// Returns formatted "DT: X/Y [ACTIVE]" string for display
    std::string getDTString() const;

    /// Returns a one-line summary of the active weapon (name, type, damage, DPS)
    std::string getWeaponSummary() const;

    /// Adds a weapon to the player's arsenal
    void equipWeapon(const Weapon& weapon);

    /// Switches to a weapon by index (0-based), returns true if valid
    bool switchWeapon(int index);

    /// Sorts all equipped weapons in descending DPS order using std::sort
    void sortWeaponsByDPS();

    /// Activates/deactivates Devil Trigger - heals and boosts damage when activated
    void activateDevilTrigger();

    /// Attacks a demon with the active weapon, applying DT multiplier if active.
    /// Builds Devil Trigger bar on hit. Returns damage dealt.
    int attack(Demon& target);

    /// Takes damage from an enemy, reduces DT bar. Returns actual damage taken.
    int takeDamage(int amount);

    /// Displays a full status report of the player (uses std::max_element to mark best weapon)
    std::string getStatusReport() const;

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif // PLAYER_H

#ifndef CASTLE_H
#define CASTLE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Demon.h"

class Player; // forward declaration

class Castle {
private:
    std::string roomName;
    std::vector<std::unique_ptr<Demon>> demons;
    bool artifactHidden;
    std::string artifactName;

    static int s_roomsCleared; ///< Total rooms fully cleared across the session

public:
    Castle();
    Castle(const std::string& roomName, const std::string& artifactName);

    // Rule of Five for deep copying the pointers
    Castle(const Castle& other);
    Castle(Castle&& other) noexcept;
    Castle& operator=(Castle other); // using copy-and-swap (takes by value!)
    ~Castle();

    // Friend swap function for the copy-and-swap idiom
    friend void swap(Castle& first, Castle& second) noexcept;

    /// Returns total rooms fully cleared across the session (static counter)
    static int getRoomsCleared();

    const std::string& getRoomName() const;
    const std::string& getArtifactName() const;
    bool isArtifactHidden() const;
    int getDemonCount() const;
    int getAliveDemonCount() const;

    /// Returns true if all demons in the room have been defeated (high-level semantic check)
    bool isCleared() const;

    /// Returns a pointer to the first alive demon (uses std::find_if), or nullptr if none
    Demon* findFirstAliveDemon();
    const Demon* findFirstAliveDemon() const;

    /// Adds a demon to this room (polymorphic add/clone)
    void addDemon(const Demon& demon);

    const std::vector<std::unique_ptr<Demon>>& getDemons() const;

    /// Applies special entry effects like FrostDemon cold aura
    void applyRoomEntryEffects(Player& player, std::vector<std::string>& log);

    /// Executes a single attack turn (Dante attacks, then demon retaliates)
    /// and logs all printed output into log.
    void executeCombatRound(Player& player, Demon& target, std::vector<std::string>& log);

    /// Returns a detailed map/description of the room and its inhabitants
    std::string getRoomDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Castle& castle);
};

#endif // CASTLE_H

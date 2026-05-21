#ifndef CASTLE_H
#define CASTLE_H

#include <iostream>
#include <string>
#include <vector>
<<<<<<< HEAD
#include <memory>
=======
>>>>>>> 1a3ae9cfb1c91f476224ec9ac56d2acd5f9d950f
#include "Demon.h"

class Player; // forward declaration

class Castle {
private:
    std::string roomName;
<<<<<<< HEAD
    std::vector<std::unique_ptr<Demon>> demons;
=======
    std::vector<Demon> demons;
>>>>>>> 1a3ae9cfb1c91f476224ec9ac56d2acd5f9d950f
    bool artifactHidden;
    std::string artifactName;

public:
    Castle();
    Castle(const std::string& roomName, const std::string& artifactName);

<<<<<<< HEAD
    // Rule of Three for deep copying the pointers
    Castle(const Castle& other);
    Castle& operator=(Castle other); // using copy-and-swap (takes by value!)
    ~Castle();

    // Friend swap function for the copy-and-swap idiom
    friend void swap(Castle& first, Castle& second) noexcept;

=======
>>>>>>> 1a3ae9cfb1c91f476224ec9ac56d2acd5f9d950f
    const std::string& getRoomName() const;
    const std::string& getArtifactName() const;
    bool isArtifactHidden() const;
    int getDemonCount() const;
    int getAliveDemonCount() const;

<<<<<<< HEAD
    /// Adds a demon to this room (polymorphic add/clone)
    void addDemon(const Demon& demon);

    const std::vector<std::unique_ptr<Demon>>& getDemons() const;

    /// Applies special entry effects like FrostDemon cold aura
    void applyRoomEntryEffects(Player& player, std::vector<std::string>& log);

    /// Executes a single attack turn (Dante attacks, then demon retaliates)
    /// and logs all printed output into log.
    void executeCombatRound(Player& player, Demon& target, std::vector<std::string>& log);
=======
    /// Adds a demon to this room
    void addDemon(const Demon& demon);

    /// Explores the room: player fights each alive demon, artifact revealed when all slain.
    /// Returns true if the artifact is found.
    bool exploreRoom(Player& player);
>>>>>>> 1a3ae9cfb1c91f476224ec9ac56d2acd5f9d950f

    /// Returns a detailed map/description of the room and its inhabitants
    std::string getRoomDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Castle& castle);
};

#endif // CASTLE_H

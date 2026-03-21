#ifndef CASTLE_H
#define CASTLE_H

#include <iostream>
#include <string>
#include <vector>
#include "Demon.h"

class Player; // forward declaration

class Castle {
private:
    std::string roomName;
    std::vector<Demon> demons;
    bool artifactHidden;
    std::string artifactName;

public:
    Castle();
    Castle(const std::string& roomName, const std::string& artifactName);

    const std::string& getRoomName() const;
    const std::string& getArtifactName() const;
    bool isArtifactHidden() const;
    int getDemonCount() const;
    int getAliveDemonCount() const;

    /// Adds a demon to this room
    void addDemon(const Demon& demon);

    /// Explores the room: player fights each alive demon, artifact revealed when all slain.
    /// Returns true if the artifact is found.
    bool exploreRoom(Player& player);

    /// Returns a detailed map/description of the room and its inhabitants
    std::string getRoomDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Castle& castle);
};

#endif // CASTLE_H

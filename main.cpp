#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "include/Weapon.h"
#include "include/Demon.h"
#include "include/Player.h"
#include "include/Castle.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::cout << "============================================" << std::endl;
    std::cout << "   DEVIL MAY CRY - Castle of the Damned" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;

    // ===== 1. Create Weapons =====
    std::cout << "--- Forging Devil Arms ---" << std::endl;
    Weapon rebellion("Rebellion", 35, WeaponType::Melee, 1.2);
    Weapon ebonyIvory("Ebony & Ivory", 15, WeaponType::Ranged, 3.0);
    Weapon cerberus("Cerberus", 40, WeaponType::Melee, 0.9);

    std::cout << rebellion << std::endl;
    std::cout << ebonyIvory << std::endl;
    std::cout << cerberus << std::endl;

    // Show combat descriptions
    std::cout << std::endl;
    std::cout << rebellion.getCombatDescription() << std::endl;
    std::cout << ebonyIvory.getCombatDescription() << std::endl;

    // Show DPS calculations
    std::cout << std::endl;
    std::cout << "DPS comparison:" << std::endl;
    std::cout << "  " << rebellion.getName() << ": " << rebellion.calculateDPS()
              << " (Speed: " << rebellion.getAttackSpeed() << ")" << std::endl;
    std::cout << "  " << ebonyIvory.getName() << ": " << ebonyIvory.calculateDPS()
              << " (Speed: " << ebonyIvory.getAttackSpeed() << ")" << std::endl;
    std::cout << "  " << cerberus.getName() << ": " << cerberus.calculateDPS()
              << " (Speed: " << cerberus.getAttackSpeed() << ")" << std::endl;

    // ===== 2. Create Player (Dante) =====
    std::cout << std::endl;
    std::cout << "--- Summoning the Hunter ---" << std::endl;
    Player dante("Dante", 200, 100.0);
    dante.equipWeapon(rebellion);
    dante.equipWeapon(ebonyIvory);
    dante.equipWeapon(cerberus);

    std::cout << dante << std::endl;
    std::cout << "  Weapons equipped: " << dante.getWeaponCount()
              << " | DT Active: " << (dante.isDevilTriggerActive() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    std::cout << dante.getStatusReport() << std::endl;

    // ===== 3. Demonstrate copy constructor and operator= =====
    std::cout << std::endl;
    std::cout << "--- Testing Rule of Three ---" << std::endl;

    // Copy constructor
    Player danteCopy(dante);
    std::cout << "Copy created: " << danteCopy << std::endl;

    // Modify the copy to prove independence
    danteCopy.takeDamage(50);
    std::cout << "After damaging copy:" << std::endl;
    std::cout << "  Original: HP = " << dante.getCurrentHealth() << std::endl;
    std::cout << "  Copy:     HP = " << danteCopy.getCurrentHealth() << std::endl;

    // operator=
    Player danteAssign;
    danteAssign = dante;
    std::cout << "Assigned: " << danteAssign << std::endl;

    // ===== 4. Create Demons =====
    std::cout << std::endl;
    std::cout << "--- Demons Emerge ---" << std::endl;
    Demon scarecrow("Scarecrow", DemonType::Scarecrow, 50, 8);
    Demon hellPride("Hell Pride", DemonType::HellPride, 70, 12);
    Demon frost("Frost", DemonType::Frost, 120, 20);

    std::cout << scarecrow << std::endl;
    std::cout << hellPride << std::endl;
    std::cout << frost << std::endl;

    // Show threat levels and attack power
    std::cout << std::endl;
    std::cout << "Threat analysis:" << std::endl;
    std::cout << "  " << scarecrow.getThreatLevel()
              << " (ATK: " << scarecrow.getAttackDamage() << ")" << std::endl;
    std::cout << "  " << hellPride.getThreatLevel()
              << " (ATK: " << hellPride.getAttackDamage() << ")" << std::endl;
    std::cout << "  " << frost.getThreatLevel()
              << " (ATK: " << frost.getAttackDamage() << ")" << std::endl;

    // ===== 5. Create Castle Rooms =====
    std::cout << std::endl;
    std::cout << "--- Building the Castle ---" << std::endl;
    Castle entrance("Grand Entrance Hall", "Sparda's Amulet");
    entrance.addDemon(scarecrow);
    entrance.addDemon(hellPride);

    Castle throneRoom("Throne of Mundus", "Force Edge Sword");
    throneRoom.addDemon(frost);

    std::cout << entrance << std::endl;
    std::cout << "  Room: " << entrance.getRoomName()
              << " | Artifact: " << entrance.getArtifactName()
              << " | Demons: " << entrance.getDemonCount() << std::endl;
    std::cout << throneRoom << std::endl;
    std::cout << "  Room: " << throneRoom.getRoomName()
              << " | Artifact: " << throneRoom.getArtifactName()
              << " | Demons: " << throneRoom.getDemonCount() << std::endl;

    // Room descriptions
    std::cout << std::endl;
    std::cout << entrance.getRoomDescription() << std::endl;
    std::cout << throneRoom.getRoomDescription() << std::endl;

    // ===== 6. Interactive weapon choice =====
    /////////////////////////////////////////////////////////////////////////
    /// Keyboard input: the user chooses which weapon to equip before exploring
    /////////////////////////////////////////////////////////////////////////
    std::cout << "========================================" << std::endl;
    std::cout << "Choose your weapon before entering the castle:" << std::endl;
    std::cout << "  0 - " << rebellion.getName() << std::endl;
    std::cout << "  1 - " << ebonyIvory.getName() << std::endl;
    std::cout << "  2 - " << cerberus.getName() << std::endl;
    std::cout << "Enter weapon index: ";

    int weaponChoice;
    std::cin >> weaponChoice;
    if (!dante.switchWeapon(weaponChoice)) {
        std::cout << "Invalid choice, keeping current weapon." << std::endl;
    } else {
        std::cout << "Equipped: " << dante.getActiveWeapon().getName() << std::endl;
    }

    // ===== 7. Explore the first room =====
    std::cout << std::endl;
    entrance.exploreRoom(dante);

    // ===== 8. Devil Trigger choice =====
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Devil Trigger bar: " << dante.getDevilTriggerBar() << "/100" << std::endl;
    std::cout << "Activate Devil Trigger before the next room? (1 = yes, 0 = no): ";

    int dtChoice;
    std::cin >> dtChoice;
    if (dtChoice == 1) {
        dante.activateDevilTrigger();
    } else {
        std::cout << "Saving Devil Trigger for later..." << std::endl;
    }

    // ===== 9. Switch weapon for second room =====
    std::cout << std::endl;
    std::cout << "Switch weapon for the next room?" << std::endl;
    std::cout << "  0 - " << rebellion.getName() << std::endl;
    std::cout << "  1 - " << ebonyIvory.getName() << std::endl;
    std::cout << "  2 - " << cerberus.getName() << std::endl;
    std::cout << "Enter weapon index (or -1 to keep current): ";

    int weaponChoice2;
    std::cin >> weaponChoice2;
    if (weaponChoice2 >= 0) {
        if (!dante.switchWeapon(weaponChoice2)) {
            std::cout << "Invalid choice, keeping current weapon." << std::endl;
        } else {
            std::cout << "Equipped: " << dante.getActiveWeapon().getName() << std::endl;
        }
    }

    // ===== 10. Explore the second room =====
    std::cout << std::endl;
    throneRoom.exploreRoom(dante);

    // ===== 11. Final status =====
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "          MISSION COMPLETE" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    std::cout << dante.getStatusReport() << std::endl;

    std::cout << std::endl;
    std::cout << "Room status:" << std::endl;
    std::cout << "  " << entrance << std::endl;
    std::cout << "  " << throneRoom << std::endl;

    if (!entrance.isArtifactHidden() && !throneRoom.isArtifactHidden()) {
        std::cout << std::endl;
        std::cout << "*** " << dante.getName() << " collected all artifacts! ***" << std::endl;
        std::cout << "*** The path to Mundus is open! ***" << std::endl;
    }

    std::cout << std::endl;
    return 0;
}

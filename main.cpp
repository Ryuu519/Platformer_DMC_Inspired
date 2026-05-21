#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>

#include "include/Weapon.h"
#include "include/Demon.h"
#include "include/Player.h"
#include "include/Castle.h"
#include "include/DMCExceptions.h"

// ============================================================
// Helper: print a separator line for console
// ============================================================
static void separator(char c = '=', int n = 48) {
    for (int i = 0; i < n; ++i) std::cout << c;
    std::cout << std::endl;
}

// ============================================================
// OOP DEMO: Homework 2 Console Outputs
// ============================================================
static void runHomeworkConsoleDemos() {
    separator('=', 48);
    std::cout << "   DEVIL MAY CRY - Castle of the Damned" << std::endl;
    std::cout << "        [ CONSOLE OOP DEMO RUN ]" << std::endl;
    separator('=', 48);
    std::cout << std::endl;

    std::cout << "[ OOP DEMO ] Custom Exception Handling" << std::endl;
    separator('-', 48);

    // 1. InvalidWeaponException
    try {
        Weapon broken("Broken Blade", -10, WeaponType::Melee, 1.0);
    } catch (const DMCGameException& e) {
        std::cout << "CAUGHT InvalidWeaponException: " << e.what() << std::endl;
    }

    // 2. InvalidEntityException - empty demon name
    try {
        ScarecrowDemon nobody("", 100, 10);
    } catch (const DMCGameException& e) {
        std::cout << "CAUGHT InvalidEntityException: " << e.what() << std::endl;
    }

    // 3. InvalidEntityException - empty player name
    try {
        Player ghost("", 100, 100.0);
    } catch (const DMCGameException& e) {
        std::cout << "CAUGHT InvalidEntityException: " << e.what() << std::endl;
    }

    // 4. CombatStateException - activate DT with 0 energy
    {
        Player tempHunter("Vergil", 150, 100.0);
        try {
            tempHunter.activateDevilTrigger();
        } catch (const DMCGameException& e) {
            std::cout << "CAUGHT CombatStateException: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "[ OOP DEMO ] Rule of Three (Player)" << std::endl;
    separator('-', 48);

    Player dante("Dante", 200, 100.0);
    {
        Player danteCopy(dante);
        danteCopy.takeDamage(50);
        std::cout << "  Original HP: " << dante.getCurrentHealth()
                  << " | Copy HP: " << danteCopy.getCurrentHealth()
                  << " (independent objects!)" << std::endl;
        Player danteAssign;
        danteAssign = dante;
        std::cout << "  Assigned:  " << danteAssign << std::endl;
    }
    std::cout << std::endl;

    std::cout << "[ OOP DEMO ] Castle Deep Copy (Rule of Three)" << std::endl;
    separator('-', 48);

    ScarecrowDemon scarecrow("Scarecrow", 50, 8);
    HellPrideDemon hellPride("Hell Pride", 70, 12);
    FrostDemon frost("Frost", 120, 20);

    Castle entrance("Grand Entrance Hall", "Sparda's Amulet");
    entrance.addDemon(scarecrow);
    entrance.addDemon(hellPride);

    Castle throneRoom("Throne of Mundus", "Force Edge Sword");
    throneRoom.addDemon(frost);

    {
        Castle entranceCopy(entrance);
        Castle assignedRoom;
        assignedRoom = throneRoom;
        std::cout << "  Original: " << entrance.getRoomName()
                  << " (" << entrance.getDemonCount() << " demons)" << std::endl;
        std::cout << "  Copy:     " << entranceCopy.getRoomName()
                  << " (" << entranceCopy.getDemonCount() << " demons)" << std::endl;
        std::cout << "  Assigned: " << assignedRoom.getRoomName()
                  << " (" << assignedRoom.getDemonCount() << " demons)" << std::endl;
    }
    std::cout << "  (Copies destroyed safely - no memory leaks)" << std::endl;
    std::cout << std::endl;

    separator('=', 48);
    std::cout << "  CONSOLE OOP DEMO COMPLETE. LAUNCHING SFML GUI..." << std::endl;
    separator('=', 48);
}

// ============================================================
// GAME STATE MACHINE
// ============================================================
enum class GameState {
    TitleScreen,
    WeaponSelection,
    Room1Intro,
    Room1Combat,
    Room1Complete,
    Room2Intro,
    Room2Combat,
    VictoryScreen,
    GameOverScreen
};

// ============================================================
// GUI BUTTON HELPER
// ============================================================
class GUIButton {
public:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered = false;

    GUIButton() = default;

    GUIButton(float x, float y, float w, float h, const std::string& label, const sf::Font& font, unsigned int fontSize = 20) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(w, h));
        shape.setFillColor(sf::Color(40, 40, 40));
        shape.setOutlineColor(sf::Color(139, 0, 0));
        shape.setOutlineThickness(2.f);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        text.setPosition(x + w/2.0f, y + h/2.0f);
    }

    void update(const sf::Vector2i& mousePos) {
        if (shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            isHovered = true;
            shape.setFillColor(sf::Color(80, 20, 20));
            shape.setOutlineColor(sf::Color(220, 20, 20));
        } else {
            isHovered = false;
            shape.setFillColor(sf::Color(40, 40, 40));
            shape.setOutlineColor(sf::Color(139, 0, 0));
        }
    }

    bool isClicked([[maybe_unused]] const sf::Vector2i& mousePos, bool mousePressed) const {
        return isHovered && mousePressed;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};

// ============================================================
// GAME DATA CONTAINER
// ============================================================
struct GameData {
    Player dante;
    Castle entrance;
    Castle throneRoom;
    std::vector<std::string> combatLog;
    GameState state;
    bool weaponMenuOpen = false;

    void reset() {
        dante = Player("Dante", 200, 100.0);
        Weapon rebellion("Rebellion", 35, WeaponType::Melee, 1.2);
        Weapon ebonyIvory("Ebony & Ivory", 15, WeaponType::Ranged, 3.0);
        Weapon cerberus("Cerberus", 40, WeaponType::Melee, 0.9);
        dante.equipWeapon(rebellion);
        dante.equipWeapon(ebonyIvory);
        dante.equipWeapon(cerberus);

        entrance = Castle("Grand Entrance Hall", "Sparda's Amulet");
        ScarecrowDemon scarecrow("Scarecrow", 50, 8);
        HellPrideDemon hellPride("Hell Pride", 70, 12);
        entrance.addDemon(scarecrow);
        entrance.addDemon(hellPride);

        throneRoom = Castle("Throne of Mundus", "Force Edge Sword");
        FrostDemon frost("Frost", 120, 20);
        throneRoom.addDemon(frost);

        combatLog.clear();
        combatLog.push_back("Dante stands before the gates of the Castle...");
        state = GameState::TitleScreen;
        weaponMenuOpen = false;
    }
};

// ============================================================
// MAIN RUN SFML GRAPHICS LOOP
// ============================================================
static void runSFMLGame() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Devil May Cry: Castle of the Damned", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "CRITICAL ERROR: Could not load Arial font!" << std::endl;
        return;
    }

    GameData data;
    data.reset();

    // Exceptions state variables
    std::string errMessage = "";
    sf::Clock errClock;

    // GUI Buttons
    GUIButton btnStart(387.f, 450.f, 250.f, 60.f, "START GAME", font, 24);
    
    // Weapon choice buttons
    GUIButton btnChoiceReb(100.f, 250.f, 240.f, 300.f, "Rebellion\n\n[Melee]\nDamage: 35\nSpeed: 1.2\n\n*Click to Equip*", font, 18);
    GUIButton btnChoiceEb(392.f, 250.f, 240.f, 300.f, "Ebony & Ivory\n\n[Ranged]\nDamage: 15\nSpeed: 3.0\n\n*Click to Equip*", font, 18);
    GUIButton btnChoiceCerb(684.f, 250.f, 240.f, 300.f, "Cerberus\n\n[Melee]\nDamage: 40\nSpeed: 0.9\n\n*Click to Equip*", font, 18);

    // Intro Buttons
    GUIButton btnEnterRoom(387.f, 500.f, 250.f, 50.f, "ENTER ROOM", font, 20);

    // Transition buttons
    GUIButton btnEnterRoom2(387.f, 580.f, 250.f, 50.f, "ENTER ROOM 2", font, 20);

    // Combat Action buttons
    GUIButton btnAttack(50.f, 630.f, 250.f, 50.f, "ATTACK", font, 20);
    GUIButton btnSwitchW(320.f, 630.f, 250.f, 50.f, "SWITCH WEAPON", font, 20);
    GUIButton btnDT(590.f, 630.f, 250.f, 50.f, "DEVIL TRIGGER", font, 20);

    // Weapon Switch popups
    GUIButton btnWReb(320.f, 570.f, 250.f, 40.f, "0 - Rebellion", font, 16);
    GUIButton btnWEb(320.f, 520.f, 250.f, 40.f, "1 - Ebony & Ivory", font, 16);
    GUIButton btnWCerb(320.f, 470.f, 250.f, 40.f, "2 - Cerberus", font, 16);

    // GameOver & Victory Buttons
    GUIButton btnRetry(250.f, 550.f, 200.f, 50.f, "PLAY AGAIN", font, 20);
    GUIButton btnExit(574.f, 550.f, 200.f, 50.f, "EXIT GAME", font, 20);

    while (window.isOpen()) {
        sf::Event event;
        bool mouseClicked = false;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mouseClicked = true;
                }
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Update button states (hovers)
        if (data.state == GameState::TitleScreen) {
            btnStart.update(mousePos);
        } else if (data.state == GameState::WeaponSelection) {
            btnChoiceReb.update(mousePos);
            btnChoiceEb.update(mousePos);
            btnChoiceCerb.update(mousePos);
        } else if (data.state == GameState::Room1Intro || data.state == GameState::Room2Intro) {
            btnEnterRoom.update(mousePos);
        } else if (data.state == GameState::Room1Complete) {
            btnEnterRoom2.update(mousePos);
            btnSwitchW.update(mousePos);
            btnDT.update(mousePos);
            if (data.weaponMenuOpen) {
                btnWReb.update(mousePos);
                btnWEb.update(mousePos);
                btnWCerb.update(mousePos);
            }
        } else if (data.state == GameState::Room1Combat || data.state == GameState::Room2Combat) {
            btnAttack.update(mousePos);
            btnSwitchW.update(mousePos);
            btnDT.update(mousePos);
            if (data.weaponMenuOpen) {
                btnWReb.update(mousePos);
                btnWEb.update(mousePos);
                btnWCerb.update(mousePos);
            }
        } else if (data.state == GameState::VictoryScreen || data.state == GameState::GameOverScreen) {
            btnRetry.update(mousePos);
            btnExit.update(mousePos);
        }

        // Process clicks and state logic
        if (mouseClicked) {
            try {
                if (data.state == GameState::TitleScreen) {
                    if (btnStart.isClicked(mousePos, mouseClicked)) {
                        data.state = GameState::WeaponSelection;
                    }
                } else if (data.state == GameState::WeaponSelection) {
                    if (btnChoiceReb.isClicked(mousePos, mouseClicked)) {
                        data.dante.switchWeapon(0);
                        data.combatLog.push_back("Equipped starting weapon: Rebellion.");
                        data.state = GameState::Room1Intro;
                    } else if (btnChoiceEb.isClicked(mousePos, mouseClicked)) {
                        data.dante.switchWeapon(1);
                        data.combatLog.push_back("Equipped starting weapon: Ebony & Ivory.");
                        data.state = GameState::Room1Intro;
                    } else if (btnChoiceCerb.isClicked(mousePos, mouseClicked)) {
                        data.dante.switchWeapon(2);
                        data.combatLog.push_back("Equipped starting weapon: Cerberus.");
                        data.state = GameState::Room1Intro;
                    }
                } else if (data.state == GameState::Room1Intro) {
                    if (btnEnterRoom.isClicked(mousePos, mouseClicked)) {
                        data.state = GameState::Room1Combat;
                        data.combatLog.push_back("--- Entered Room 1: Grand Entrance Hall ---");
                    }
                } else if (data.state == GameState::Room2Intro) {
                    if (btnEnterRoom.isClicked(mousePos, mouseClicked)) {
                        // Apply room entry effects (dynamic_cast cold aura check)
                        data.throneRoom.applyRoomEntryEffects(data.dante, data.combatLog);
                        data.state = GameState::Room2Combat;
                        data.combatLog.push_back("--- Entered Room 2: Throne of Mundus ---");
                    }
                } else if (data.state == GameState::Room1Complete) {
                    if (btnEnterRoom2.isClicked(mousePos, mouseClicked)) {
                        data.weaponMenuOpen = false;
                        data.state = GameState::Room2Intro;
                    } else if (btnSwitchW.isClicked(mousePos, mouseClicked)) {
                        data.weaponMenuOpen = !data.weaponMenuOpen;
                    } else if (btnDT.isClicked(mousePos, mouseClicked)) {
                        data.dante.activateDevilTrigger();
                        data.combatLog.push_back("Dante toggled Devil Trigger!");
                    } else if (data.weaponMenuOpen) {
                        if (btnWReb.isClicked(mousePos, mouseClicked)) {
                            data.dante.switchWeapon(0);
                            data.combatLog.push_back("Equipped: Rebellion.");
                            data.weaponMenuOpen = false;
                        } else if (btnWEb.isClicked(mousePos, mouseClicked)) {
                            data.dante.switchWeapon(1);
                            data.combatLog.push_back("Equipped: Ebony & Ivory.");
                            data.weaponMenuOpen = false;
                        } else if (btnWCerb.isClicked(mousePos, mouseClicked)) {
                            data.dante.switchWeapon(2);
                            data.combatLog.push_back("Equipped: Cerberus.");
                            data.weaponMenuOpen = false;
                        }
                    }
                } else if (data.state == GameState::Room1Combat || data.state == GameState::Room2Combat) {
                    Castle& currentRoom = (data.state == GameState::Room1Combat) ? data.entrance : data.throneRoom;
                    const auto& demons = currentRoom.getDemons();

                    // Find first alive demon
                    int targetIdx = -1;
                    for (int i = 0; i < (int)demons.size(); ++i) {
                        if (demons[i] && demons[i]->isAlive()) {
                            targetIdx = i;
                            break;
                        }
                    }

                    if (btnAttack.isClicked(mousePos, mouseClicked)) {
                        data.weaponMenuOpen = false;
                        if (targetIdx != -1) {
                            currentRoom.executeCombatRound(data.dante, *demons[targetIdx], data.combatLog);
                            
                            // Check game over
                            if (!data.dante.isAlive()) {
                                data.state = GameState::GameOverScreen;
                            }
                            // Check room clear
                            else if (currentRoom.getAliveDemonCount() == 0) {
                                if (data.state == GameState::Room1Combat) {
                                    data.state = GameState::Room1Complete;
                                } else {
                                    data.state = GameState::VictoryScreen;
                                }
                            }
                        }
                    } else if (btnSwitchW.isClicked(mousePos, mouseClicked)) {
                        data.weaponMenuOpen = !data.weaponMenuOpen;
                    } else if (btnDT.isClicked(mousePos, mouseClicked)) {
                        data.weaponMenuOpen = false;
                        data.dante.activateDevilTrigger();
                        if (data.dante.isDevilTriggerActive()) {
                            data.combatLog.push_back("Dante activated DEVIL TRIGGER! Attacks deal 2x damage, takes 50% less damage.");
                        } else {
                            data.combatLog.push_back("Dante deactivated Devil Trigger.");
                        }
                    } else if (data.weaponMenuOpen) {
                        if (btnWReb.isClicked(mousePos, mouseClicked)) {
                            data.dante.switchWeapon(0);
                            data.combatLog.push_back("Equipped: Rebellion.");
                            data.weaponMenuOpen = false;
                        } else if (btnWEb.isClicked(mousePos, mouseClicked)) {
                            data.dante.switchWeapon(1);
                            data.combatLog.push_back("Equipped: Ebony & Ivory.");
                            data.weaponMenuOpen = false;
                        } else if (btnWCerb.isClicked(mousePos, mouseClicked)) {
                            data.dante.switchWeapon(2);
                            data.combatLog.push_back("Equipped: Cerberus.");
                            data.weaponMenuOpen = false;
                        }
                    }
                } else if (data.state == GameState::VictoryScreen || data.state == GameState::GameOverScreen) {
                    if (btnRetry.isClicked(mousePos, mouseClicked)) {
                        data.reset();
                    } else if (btnExit.isClicked(mousePos, mouseClicked)) {
                        window.close();
                    }
                }
            } catch (const DMCGameException& e) {
                // Caught Game Exception -> visual banner popup
                errMessage = e.what();
                errClock.restart();
                data.weaponMenuOpen = false;
            }
        }

        // ============================================================
        // DRAWING
        // ============================================================
        window.clear(sf::Color(18, 18, 18));

        // Draw title or state background
        if (data.state == GameState::TitleScreen) {
            sf::Text title("DEVIL MAY CRY", font, 54);
            title.setFillColor(sf::Color(180, 0, 0));
            title.setStyle(sf::Text::Bold);
            sf::FloatRect tr = title.getLocalBounds();
            title.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
            title.setPosition(512.f, 220.f);
            window.draw(title);

            sf::Text subtitle("Castle of the Damned [SFML Edition]", font, 22);
            subtitle.setFillColor(sf::Color::White);
            sf::FloatRect sr = subtitle.getLocalBounds();
            subtitle.setOrigin(sr.left + sr.width/2.f, sr.top + sr.height/2.f);
            subtitle.setPosition(512.f, 290.f);
            window.draw(subtitle);

            btnStart.draw(window);
        }
        else if (data.state == GameState::WeaponSelection) {
            sf::Text title("CHOOSE YOUR STARTING WEAPON", font, 32);
            title.setFillColor(sf::Color::White);
            title.setStyle(sf::Text::Bold);
            sf::FloatRect tr = title.getLocalBounds();
            title.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
            title.setPosition(512.f, 80.f);
            window.draw(title);

            btnChoiceReb.draw(window);
            btnChoiceEb.draw(window);
            btnChoiceCerb.draw(window);
        }
        else if (data.state == GameState::Room1Intro || data.state == GameState::Room2Intro) {
            std::string rName = (data.state == GameState::Room1Intro) ? "ROOM 1: Grand Entrance Hall" : "ROOM 2: Throne of Mundus";
            sf::Text title(rName, font, 36);
            title.setFillColor(sf::Color(180, 0, 0));
            title.setStyle(sf::Text::Bold);
            sf::FloatRect tr = title.getLocalBounds();
            title.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
            title.setPosition(512.f, 200.f);
            window.draw(title);

            std::string desc = (data.state == GameState::Room1Intro)
                ? "Dante approaches the entrance of the Underworld domain.\nA Scarecrow and a Hell Pride demon bar the path!\nPrepare for battle."
                : "Dante enters the Mundus Throne Room.\nA freezing aura sweeps through the area.\nA powerful Frost demon stands shield-ready!";
            sf::Text textDesc(desc, font, 20);
            textDesc.setFillColor(sf::Color::White);
            sf::FloatRect dr = textDesc.getLocalBounds();
            textDesc.setOrigin(dr.left + dr.width/2.f, dr.top + dr.height/2.f);
            textDesc.setPosition(512.f, 320.f);
            window.draw(textDesc);

            btnEnterRoom.draw(window);
        }
        else if (data.state == GameState::Room1Complete) {
            sf::Text title("ROOM 1 CLEARED!", font, 36);
            title.setFillColor(sf::Color(212, 175, 55));
            title.setStyle(sf::Text::Bold);
            sf::FloatRect tr = title.getLocalBounds();
            title.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
            title.setPosition(512.f, 100.f);
            window.draw(title);

            std::string summary = "You discovered the artifact: Sparda's Amulet!\n\nUse this respite to change weapons or toggle Devil Trigger.";
            sf::Text textSum(summary, font, 20);
            textSum.setFillColor(sf::Color::White);
            sf::FloatRect sr = textSum.getLocalBounds();
            textSum.setOrigin(sr.left + sr.width/2.f, sr.top + sr.height/2.f);
            textSum.setPosition(512.f, 200.f);
            window.draw(textSum);

            // Draw status HUD in center
            sf::RectangleShape hudBack(sf::Vector2f(450.f, 200.f));
            hudBack.setFillColor(sf::Color(30, 30, 30));
            hudBack.setOutlineColor(sf::Color(139, 0, 0));
            hudBack.setOutlineThickness(2.f);
            hudBack.setPosition(287.f, 280.f);
            window.draw(hudBack);

            sf::Text nameText(data.dante.getName() + " (Equipped: " + data.dante.getActiveWeapon().getName() + ")", font, 20);
            nameText.setFillColor(sf::Color::White);
            nameText.setPosition(307.f, 300.f);
            window.draw(nameText);

            // HP Bar
            sf::RectangleShape hpBack(sf::Vector2f(410.f, 24.f));
            hpBack.setFillColor(sf::Color(60, 0, 0));
            hpBack.setPosition(307.f, 340.f);
            window.draw(hpBack);

            float hpWidth = 410.f * (static_cast<float>(data.dante.getCurrentHealth()) / data.dante.getMaxHealth());
            sf::RectangleShape hpFill(sf::Vector2f(hpWidth, 24.f));
            hpFill.setFillColor(sf::Color(180, 0, 0));
            hpFill.setPosition(307.f, 340.f);
            window.draw(hpFill);

            sf::Text hpText("HP: " + std::to_string(data.dante.getCurrentHealth()) + "/" + std::to_string(data.dante.getMaxHealth()), font, 14);
            hpText.setFillColor(sf::Color::White);
            sf::FloatRect hpTR = hpText.getLocalBounds();
            hpText.setPosition(307.f + 205.f - hpTR.width/2.f, 343.f);
            window.draw(hpText);

            // DT Bar
            sf::RectangleShape dtBack(sf::Vector2f(410.f, 24.f));
            dtBack.setFillColor(sf::Color(0, 0, 60));
            dtBack.setPosition(307.f, 380.f);
            window.draw(dtBack);

            float dtWidth = 410.f * (static_cast<float>(data.dante.getDevilTriggerBar()) / 100.f);
            sf::RectangleShape dtFill(sf::Vector2f(dtWidth, 24.f));
            dtFill.setFillColor(sf::Color(0, 191, 255));
            dtFill.setPosition(307.f, 380.f);
            window.draw(dtFill);

            sf::Text dtText("Devil Trigger: " + std::to_string(static_cast<int>(data.dante.getDevilTriggerBar())) + "/100", font, 14);
            dtText.setFillColor(sf::Color::White);
            sf::FloatRect dtTR = dtText.getLocalBounds();
            dtText.setPosition(307.f + 205.f - dtTR.width/2.f, 383.f);
            window.draw(dtText);

            // Switch weapon sub-buttons if menu is open
            btnSwitchW.draw(window);
            btnDT.draw(window);
            btnEnterRoom2.draw(window);

            if (data.weaponMenuOpen) {
                btnWReb.draw(window);
                btnWEb.draw(window);
                btnWCerb.draw(window);
            }
        }
        else if (data.state == GameState::Room1Combat || data.state == GameState::Room2Combat) {
            Castle& currentRoom = (data.state == GameState::Room1Combat) ? data.entrance : data.throneRoom;
            const auto& demons = currentRoom.getDemons();

            // Find current target
            int targetIdx = -1;
            for (int i = 0; i < (int)demons.size(); ++i) {
                if (demons[i] && demons[i]->isAlive()) {
                    targetIdx = i;
                    break;
                }
            }

            // Room title
            sf::Text title(currentRoom.getRoomName(), font, 24);
            title.setFillColor(sf::Color::White);
            title.setStyle(sf::Text::Bold);
            title.setPosition(50.f, 40.f);
            window.draw(title);

            // ==================== DANTE PANEL ====================
            sf::RectangleShape pBack(sf::Vector2f(440.f, 300.f));
            pBack.setFillColor(sf::Color(30, 30, 30));
            pBack.setOutlineColor(sf::Color(139, 0, 0));
            pBack.setOutlineThickness(2.f);
            pBack.setPosition(50.f, 100.f);
            window.draw(pBack);

            sf::Text danteName(data.dante.getName() + " (Equipped Weapon)", font, 20);
            danteName.setFillColor(sf::Color::White);
            danteName.setPosition(70.f, 120.f);
            window.draw(danteName);

            // HP
            sf::RectangleShape hpBack(sf::Vector2f(400.f, 26.f));
            hpBack.setFillColor(sf::Color(60, 0, 0));
            hpBack.setPosition(70.f, 160.f);
            window.draw(hpBack);

            float hpWidth = 400.f * (static_cast<float>(data.dante.getCurrentHealth()) / data.dante.getMaxHealth());
            sf::RectangleShape hpFill(sf::Vector2f(hpWidth, 26.f));
            hpFill.setFillColor(sf::Color(180, 0, 0));
            hpFill.setPosition(70.f, 160.f);
            window.draw(hpFill);

            sf::Text hpText("HP: " + std::to_string(data.dante.getCurrentHealth()) + "/" + std::to_string(data.dante.getMaxHealth()), font, 14);
            hpText.setFillColor(sf::Color::White);
            sf::FloatRect hpTR = hpText.getLocalBounds();
            hpText.setPosition(70.f + 200.f - hpTR.width/2.f, 165.f);
            window.draw(hpText);

            // DT
            sf::RectangleShape dtBack(sf::Vector2f(400.f, 26.f));
            dtBack.setFillColor(sf::Color(0, 0, 60));
            dtBack.setPosition(70.f, 205.f);
            window.draw(dtBack);

            float dtWidth = 400.f * (static_cast<float>(data.dante.getDevilTriggerBar()) / 100.f);
            sf::RectangleShape dtFill(sf::Vector2f(dtWidth, 26.f));
            dtFill.setFillColor(sf::Color(0, 191, 255));
            dtFill.setPosition(70.f, 205.f);
            window.draw(dtFill);

            sf::Text dtText("Devil Trigger: " + std::to_string(static_cast<int>(data.dante.getDevilTriggerBar())) + "/100", font, 14);
            dtText.setFillColor(sf::Color::White);
            sf::FloatRect dtTR = dtText.getLocalBounds();
            dtText.setPosition(70.f + 200.f - dtTR.width/2.f, 210.f);
            window.draw(dtText);

            // Active Weapon Info
            sf::Text wInfo("Weapon: " + data.dante.getActiveWeapon().getName(), font, 18);
            wInfo.setFillColor(sf::Color(200, 200, 200));
            wInfo.setPosition(70.f, 260.f);
            window.draw(wInfo);

            sf::Text wStats("Damage: " + std::to_string(data.dante.getActiveWeapon().getDamage()) + " | DPS: " + std::to_string(static_cast<int>(data.dante.getActiveWeapon().calculateDPS())), font, 16);
            wStats.setFillColor(sf::Color(150, 150, 150));
            wStats.setPosition(70.f, 285.f);
            window.draw(wStats);

            if (data.dante.isDevilTriggerActive()) {
                sf::Text dtActiveText("[DT BOOST: 2X DMG, 50% RESIST]", font, 14);
                dtActiveText.setFillColor(sf::Color(148, 0, 211));
                dtActiveText.setPosition(70.f, 320.f);
                window.draw(dtActiveText);
            }

            // ==================== ENEMIES PANEL ====================
            sf::RectangleShape eBack(sf::Vector2f(440.f, 300.f));
            eBack.setFillColor(sf::Color(30, 30, 30));
            eBack.setOutlineColor(sf::Color(139, 0, 0));
            eBack.setOutlineThickness(2.f);
            eBack.setPosition(534.f, 100.f);
            window.draw(eBack);

            float yOffset = 120.f;
            for (int i = 0; i < (int)demons.size(); ++i) {
                if (!demons[i]) continue;

                // Draw target glow
                if (i == targetIdx) {
                    sf::RectangleShape targetBorder(sf::Vector2f(410.f, 80.f));
                    targetBorder.setFillColor(sf::Color::Transparent);
                    targetBorder.setOutlineColor(sf::Color(212, 175, 55)); // gold glow
                    targetBorder.setOutlineThickness(2.f);
                    targetBorder.setPosition(549.f, yOffset - 5.f);
                    window.draw(targetBorder);
                }

                if (!demons[i]->isAlive()) {
                    sf::Text deadText("[DEFEATED] " + demons[i]->getName(), font, 18);
                    deadText.setFillColor(sf::Color(100, 100, 100));
                    deadText.setPosition(560.f, yOffset + 20.f);
                    window.draw(deadText);
                } else {
                    sf::Text demName(demons[i]->getName() + " (" + demons[i]->getThreatLevel().substr(demons[i]->getName().length()) + ")", font, 16);
                    demName.setFillColor(sf::Color::White);
                    demName.setPosition(560.f, yOffset);
                    window.draw(demName);

                    // HP Bar
                    sf::RectangleShape dHPBack(sf::Vector2f(380.f, 14.f));
                    dHPBack.setFillColor(sf::Color(50, 0, 0));
                    dHPBack.setPosition(560.f, yOffset + 28.f);
                    window.draw(dHPBack);

                    float dHPWidth = 380.f * (static_cast<float>(demons[i]->getCurrentHealth()) / demons[i]->getMaxHealth());
                    sf::RectangleShape dHPFill(sf::Vector2f(dHPWidth, 14.f));
                    dHPFill.setFillColor(sf::Color(150, 0, 0));
                    dHPFill.setPosition(560.f, yOffset + 28.f);
                    window.draw(dHPFill);

                    // Check for Frost shield
                    if (auto* frostDem = dynamic_cast<FrostDemon*>(demons[i].get())) {
                        if (frostDem->getShieldHP() > 0) {
                            sf::RectangleShape shBack(sf::Vector2f(380.f, 10.f));
                            shBack.setFillColor(sf::Color(0, 40, 40));
                            shBack.setPosition(560.f, yOffset + 48.f);
                            window.draw(shBack);

                            float shWidth = 380.f * (static_cast<float>(frostDem->getShieldHP()) / 40.f); // Base shield is 40
                            if (shWidth > 380.f) shWidth = 380.f;
                            sf::RectangleShape shFill(sf::Vector2f(shWidth, 10.f));
                            shFill.setFillColor(sf::Color(224, 255, 255));
                            shFill.setPosition(560.f, yOffset + 48.f);
                            window.draw(shFill);

                            sf::Text shText("Ice Shield: " + std::to_string(frostDem->getShieldHP()) + " HP", font, 10);
                            shText.setFillColor(sf::Color::White);
                            shText.setPosition(560.f, yOffset + 58.f);
                            window.draw(shText);
                        }
                    }
                }

                yOffset += 100.f;
            }

            // ==================== COMBAT LOG ====================
            sf::RectangleShape lBack(sf::Vector2f(924.f, 180.f));
            lBack.setFillColor(sf::Color(20, 20, 20));
            lBack.setOutlineColor(sf::Color(139, 0, 0));
            lBack.setOutlineThickness(2.f);
            lBack.setPosition(50.f, 430.f);
            window.draw(lBack);

            int startIdx = std::max(0, (int)data.combatLog.size() - 7);
            float logY = 440.f;
            for (int i = startIdx; i < (int)data.combatLog.size(); ++i) {
                sf::Text logText(data.combatLog[i], font, 14);
                
                // Color coding based on logs
                if (data.combatLog[i].find("CAUGHT") != std::string::npos || data.combatLog[i].find("WARNING:") != std::string::npos) {
                    logText.setFillColor(sf::Color(220, 20, 60)); // red
                } else if (data.combatLog[i].find("DT BOOST") != std::string::npos || data.combatLog[i].find("DEVIL TRIGGER") != std::string::npos) {
                    logText.setFillColor(sf::Color(0, 191, 255)); // cyan
                } else if (data.combatLog[i].find("SPECIAL ABILITY") != std::string::npos || data.combatLog[i].find("SUMMONS") != std::string::npos) {
                    logText.setFillColor(sf::Color(255, 140, 0)); // orange
                } else if (data.combatLog[i].find("defeated!") != std::string::npos || data.combatLog[i].find("discovered") != std::string::npos) {
                    logText.setFillColor(sf::Color(212, 175, 55)); // gold
                } else if (data.combatLog[i].find("parries") != std::string::npos || data.combatLog[i].find("misses") != std::string::npos) {
                    logText.setFillColor(sf::Color(46, 139, 87)); // green
                } else {
                    logText.setFillColor(sf::Color(220, 220, 220)); // light gray
                }

                logText.setPosition(70.f, logY);
                window.draw(logText);
                logY += 22.f;
            }

            // Draw action buttons
            btnAttack.draw(window);
            btnSwitchW.draw(window);
            btnDT.draw(window);

            if (data.weaponMenuOpen) {
                btnWReb.draw(window);
                btnWEb.draw(window);
                btnWCerb.draw(window);
            }
        }
        else if (data.state == GameState::VictoryScreen) {
            sf::Text title("MISSION COMPLETE - VICTORY!", font, 42);
            title.setFillColor(sf::Color(212, 175, 55)); // gold
            title.setStyle(sf::Text::Bold);
            sf::FloatRect tr = title.getLocalBounds();
            title.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
            title.setPosition(512.f, 200.f);
            window.draw(title);

            sf::Text summary("Dante has collected Sparda's Amulet and the Force Edge Sword.\nThe Underworld has been sealed.\nAll demons are defeated!", font, 20);
            summary.setFillColor(sf::Color::White);
            sf::FloatRect sr = summary.getLocalBounds();
            summary.setOrigin(sr.left + sr.width/2.f, sr.top + sr.height/2.f);
            summary.setPosition(512.f, 320.f);
            window.draw(summary);

            btnRetry.draw(window);
            btnExit.draw(window);
        }
        else if (data.state == GameState::GameOverScreen) {
            sf::Text title("GAME OVER - YOU DIED", font, 42);
            title.setFillColor(sf::Color(180, 0, 0));
            title.setStyle(sf::Text::Bold);
            sf::FloatRect tr = title.getLocalBounds();
            title.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
            title.setPosition(512.f, 200.f);
            window.draw(title);

            sf::Text summary("Dante has fallen in battle against the Underworld hordes.\nThe human world is doomed.", font, 20);
            summary.setFillColor(sf::Color::White);
            sf::FloatRect sr = summary.getLocalBounds();
            summary.setOrigin(sr.left + sr.width/2.f, sr.top + sr.height/2.f);
            summary.setPosition(512.f, 320.f);
            window.draw(summary);

            btnRetry.draw(window);
            btnExit.draw(window);
        }

        // ==================== VISUAL ERROR POPUP BANNER ====================
        if (!errMessage.empty() && errClock.getElapsedTime().asSeconds() < 3.0f) {
            sf::RectangleShape banner(sf::Vector2f(1024.f, 50.f));
            banner.setFillColor(sf::Color(220, 20, 60, 235)); // semi-transparent deep red
            banner.setPosition(0.f, 0.f);

            sf::Text errText(errMessage, font, 18);
            errText.setFillColor(sf::Color::White);
            sf::FloatRect r = errText.getLocalBounds();
            errText.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
            errText.setPosition(512.f, 25.f);

            window.draw(banner);
            window.draw(errText);
        } else {
            errMessage = "";
        }

        window.display();
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 1. Run console outputs for Homework evaluation
    runHomeworkConsoleDemos();

    // 2. Launch SFML graphical interface
    runSFMLGame();

    return 0;
}

#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    std::cout << "SFML test starting..." << std::endl;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    std::cout << "Desktop resolution: " << desktop.width << "x" << desktop.height << std::endl;
    return 0;
}

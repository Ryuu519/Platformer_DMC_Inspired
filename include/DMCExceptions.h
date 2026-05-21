#ifndef OOP_DMCEXCEPTIONS_H
#define OOP_DMCEXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

// Base custom exception class inheriting from std::exception
class DMCGameException : public std::exception {
protected:
    std::string message;

public:
    explicit DMCGameException(std::string msg) : message(std::move(msg)) {}
    
    // Override what() const noexcept from std::exception
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 1. Category 1: InvalidEntityException (Thrown in Player/Demon constructors for invalid stats)
class InvalidEntityException : public DMCGameException {
public:
    explicit InvalidEntityException(const std::string& msg) 
        : DMCGameException("InvalidEntityException: " + msg) {}
};

// 2. Category 2: InvalidWeaponException (Thrown in Weapon constructor or weapon equip logic)
class InvalidWeaponException : public DMCGameException {
public:
    explicit InvalidWeaponException(const std::string& msg) 
        : DMCGameException("InvalidWeaponException: " + msg) {}
};

// 3. Category 3: CombatStateException (Thrown during combat, like invalid DT activation or out-of-bound weapons)
class CombatStateException : public DMCGameException {
public:
    explicit CombatStateException(const std::string& msg) 
        : DMCGameException("CombatStateException: " + msg) {}
};

#endif // OOP_DMCEXCEPTIONS_H
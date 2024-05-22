#pragma once
// Simple struct to manage coordinates
struct FCoord {
    float x, y;
    FCoord(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct IComponent {
    virtual ~IComponent() = default;
};

struct PositionComponent : IComponent {
    FCoord _position;
    PositionComponent(FCoord position = {1.0f, 1.0f}) : _position(position) {};
};

struct PhysicsComponent : IComponent {
    FCoord _velocity;
    float _mass;
    PhysicsComponent(FCoord velocity, float mass) : _velocity(velocity), _mass(mass) {};
};

struct HealthComponent : IComponent {
    int health;
};

struct ConsoleCharacterComponent : IComponent {
    wchar_t _character;
    ConsoleCharacterComponent(wchar_t character) :_character(character) {};
};

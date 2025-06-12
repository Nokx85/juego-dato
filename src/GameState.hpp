#pragma once
#include <SFML/Graphics.hpp>

/// Identificadores de estados posibles
enum class StateID { None, Menu, Play };

/// Interfaz común para cada pantalla (menú, juego, etc.)
class GameState {
public:
    virtual ~GameState() {}

    /// Procesa eventos de SFML (teclas, clicks…)
    virtual void handleEvent(const sf::Event& event) = 0;

    /// Lógica “por frame” (animaciones, temporizadores…)
    virtual void update(sf::Time dt) = 0;

    /// Dibuja todo lo necesario en la ventana
    virtual void draw(sf::RenderWindow& window) = 0;

    /// Indica si el estado quiere cambiar a otro
    virtual StateID nextState() const { return StateID::None; }
};

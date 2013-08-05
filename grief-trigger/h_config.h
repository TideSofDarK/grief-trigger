#ifndef CONFIG_INCLUDE
#define CONFIG_INCLUDE

#include "string"

#include <SFML/System.hpp>

#define UP sf::Keyboard::Key::Up
#define RIGHT sf::Keyboard::Key::Right
#define DOWN sf::Keyboard::Key::Down
#define LEFT sf::Keyboard::Key::Left
#define _Z sf::Keyboard::Key::Z
#define _X sf::Keyboard::Key::X

static const sf::Uint32 TILE_SIZE = 32;
static const sf::Uint32 CHARACTER_SIZE = 32;

static const float playerMoveSpeed = 76.8f;

static const sf::Uint32 scrollAreaX = 1280 / 3;
static const sf::Uint32 scrollAreaY = 720 / 3;

static const sf::Uint32 WIDTH = 1280;
static const sf::Uint32 HEIGHT = 720;

static const sf::Uint32 HALF_WIDTH = WIDTH / 2;
static const sf::Uint32 HALF_HEIGHT = HEIGHT / 2;

static const sf::Uint32 DIR_IDLE = -1;
static const sf::Uint32 DIR_UP = 0;
static const sf::Uint32 DIR_RIGHT = 1;
static const sf::Uint32 DIR_LEFT = 2;
static const sf::Uint32 DIR_DOWN = 3;

static const std::string fontPath = "assets/fonts/default.TTF";

#endif
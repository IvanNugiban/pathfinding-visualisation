
#pragma once 

#include "resources.h"

namespace settings {

	// Window
	const inline sf::Vector2f windowSize{1920, 1080};
	constexpr int fps{144};

	// Grid
	const inline sf::Vector2f gridSize{ 1600, 800 };
	constexpr inline std::array rowDirections = { 0, 0, -1, 1 };
	constexpr inline std::array colDirections = { -1, 1, 0, 0 };
	constexpr inline int gridRows{10};
	constexpr inline int gridColumns{ gridRows * 2};
	constexpr inline float gridGap{3};
	constexpr inline int randomGridMaxCoverage{50};
	const inline sf::Color tileColor{ 197, 199, 200};
	const inline sf::Color tileHoveredColor{ 197, 199, 200, 200};
	const inline sf::Color tileObstacleColor{ 197, 199, 200, 80 };
	const inline sf::Color startTileColor{ sf::Color::Green };
	const inline sf::Color finishTileColor{ sf::Color::Red };
	const inline sf::Color pathTileColor{ sf::Color::Color(153, 206, 255)};
	const inline sf::Color checkedTileColor{ sf::Color::Color(160, 160, 160) };

}

#pragma once

#include "resources.h"


namespace engine {
	namespace window {
		// Window
		extern const sf::Vector2f windowSize;
		extern std::unique_ptr<sf::RenderWindow> windowPtr;
		extern sf::Vector2i mousePos;
		extern sf::Image icon;

		// Animation
		extern float animationFrame;
		extern float animationSpeed;

		void create();
		void initWindow();
		void update();
		void close();

		// Drawing
		void startDrawing();
		void endDrawing();
	}
}
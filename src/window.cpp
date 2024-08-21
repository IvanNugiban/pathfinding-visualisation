#include "../include/window.h"
#include "../include/ui.h"
#include "../include/grid.h"
#include "../include/settings.h"

namespace engine {
	namespace window {

		const sf::Vector2f windowSize{settings::windowSize};
		std::unique_ptr<sf::RenderWindow> windowPtr{};
		sf::Vector2i mousePos{};
		sf::Image icon{};
		float animationFrame{};
		float animationSpeed{};

		void create() {
			windowPtr = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), "Pathfinding Visualisation", sf::Style::Fullscreen);
			initWindow();
		}
		void initWindow()
		{
			windowPtr->setFramerateLimit(settings::fps);
			engine::ui::gui.setWindow(*windowPtr);

			// Set icon
			icon.loadFromFile("../resources/Icon.png");
			windowPtr->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
		}

		void update()
		{

			for (auto event = sf::Event{}; windowPtr->pollEvent(event);)
			{
				// Close window
				if (event.type == sf::Event::Closed)
				{
					windowPtr->close();
				}

				// Key events
				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code)
					{
					case sf::Keyboard::Escape:
						windowPtr->close();
						break;

					case sf::Keyboard::Enter:
						engine::ui::onStartButtonClick();
					default:
						break;
					}
				}

				// Update mouse position
				if (event.type == sf::Event::MouseMoved) {
					mousePos = { event.mouseMove.x, event.mouseMove.y };
				}


				// Mouse press events
				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						engine::grid.leftClick(sf::Vector2i{ event.mouseButton.x, event.mouseButton.y });
					}

					if (event.mouseButton.button == sf::Mouse::Right) {
						engine::grid.rightClick();
					}
				}

				// Mouse release events
				if (event.type == sf::Event::MouseButtonReleased) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						engine::grid.leftReleased(sf::Vector2i{ event.mouseButton.x, event.mouseButton.y });
					}

					if (event.mouseButton.button == sf::Mouse::Right) {
						engine::grid.rightReleased();
					}
				}
				
				// Handle ui events
				engine::ui::gui.handleEvent(event);
			}

			animationFrame += engine::ui::speedSlider->getValue();
		}

		void close()
		{
			windowPtr->close();
		}

		void startDrawing()
		{
			windowPtr->clear(sf::Color::White);
		}

		void endDrawing()
		{
			windowPtr->display();
		}
	}
}
#include "../include/ui.h"
#include "../include/window.h"
#include "../include/settings.h"
#include "../include/grid.h"

namespace engine {
	namespace ui {

		// Gui
		tgui::Gui gui{};

		// UI elements
		tgui::HorizontalWrap::Ptr layout;
		tgui::VerticalLayout::Ptr titleWrapper;
		tgui::Label::Ptr titleText;
		tgui::VerticalLayout::Ptr algorithmWrapper;
		tgui::Label::Ptr algorithmText;
		tgui::ComboBox::Ptr algorithmSelector;
		tgui::VerticalLayout::Ptr speedWrapper;
		tgui::Label::Ptr speedText;
		tgui::Slider::Ptr speedSlider;
		tgui::VerticalLayout::Ptr audioWrapper;
		tgui::Label::Ptr audioText;
		tgui::Slider::Ptr audioSlider;
		tgui::BitmapButton::Ptr startButton;
		tgui::VerticalLayout::Ptr buttonsWrapper;
		tgui::Button::Ptr clearGridButton;
		tgui::Button::Ptr fillGridButton;
		tgui::Button::Ptr randomGridButton;
		tgui::SeparatorLine::Ptr line;

		bool inProcess{};

		void initialize()
		{
			layout = tgui::HorizontalWrap::create();
			layout->setSize({ 1600, 75 });
			layout->setPosition({settings::windowSize.x / 2 - layout->getSize().x / 2, 50});
			layout->getRenderer()->setSpaceBetweenWidgets(20);
			gui.add(layout);

			titleWrapper = tgui::VerticalLayout::create();
			titleWrapper->addSpace(0.25);
			titleWrapper->setSize({ 400 , 80 });
			layout->add(titleWrapper);

			titleText = tgui::Label::create("Pathfinding visualisation");
			titleText->setVerticalAlignment(tgui::VerticalAlignment::Top);
			titleText->setTextSize(30);
			titleWrapper->add(titleText);

			algorithmWrapper = tgui::VerticalLayout::create();
			algorithmWrapper->setSize({200, 80 });
			layout->add(algorithmWrapper);

			algorithmText = tgui::Label::create("Algorithm");
			algorithmText->setTextSize(20);
			algorithmWrapper->add(algorithmText);

			algorithmSelector = tgui::ComboBox::create();
			algorithmSelector->getRenderer()->setTextSize(15);
			algorithmSelector->addItem("Breadth First Search");
			algorithmSelector->addItem("A* Search");
			algorithmSelector->setSelectedItemByIndex(0);

			algorithmWrapper->add(algorithmSelector);
			algorithmWrapper->addSpace(0.8);

			speedWrapper = tgui::VerticalLayout::create();
			speedWrapper->setSize({ 200, 80 });
			layout->add(speedWrapper);

			speedText = tgui::Label::create("Speed");
			speedText->setTextSize(20);
			speedWrapper->add(speedText);

			speedSlider = tgui::Slider::create();
			speedSlider->setMinimum(0);
			speedSlider->setMaximum(2);
			speedSlider->setStep(0.1);
			speedSlider->setValue(1);
			speedWrapper->add(speedSlider);
			speedWrapper->addSpace(0.3);

			audioWrapper = tgui::VerticalLayout::create();
			audioWrapper->setSize({ 200, 80 });
			layout->add(audioWrapper);

			audioText = tgui::Label::create("Volume");
			audioText->setTextSize(20);
			audioWrapper->add(audioText);

			audioSlider = tgui::Slider::create();
			audioSlider->setMinimum(0);
			audioSlider->setMaximum(100);
			audioSlider->setStep(5);
			audioSlider->setValue(50);
			audioWrapper->add(audioSlider);
			audioWrapper->addSpace(0.3);

			startButton = tgui::BitmapButton::create();
			startButton->setSize({60, 60});
			startButton->getRenderer()->setBackgroundColor(sf::Color::Green);
			startButton->getRenderer()->setBorderColor(sf::Color::White);
			startButton->getRenderer()->setRoundedBorderRadius(50);
			startButton->getRenderer()->setBackgroundColorHover(sf::Color::Color(0, 255, 0, 150));
			startButton->getRenderer()->setBackgroundColorDown(sf::Color::Color(0, 255, 0, 120));
			startButton->setImage("resources/StartIcon.png");
			startButton->onClick(&onStartButtonClick);
			layout->add(startButton);

			auto space = tgui::HorizontalLayout::create();
			space->setSize({ 20, 0 });
			layout->add(space);

			buttonsWrapper = tgui::VerticalLayout::create();
			buttonsWrapper->setSize({ 110, 50 });
			buttonsWrapper->addSpace(0.5);
			layout->add(buttonsWrapper);
			auto buttonsWrapper2 = tgui::VerticalLayout::copy(buttonsWrapper);
			layout->add(buttonsWrapper2);
			auto buttonsWrapper3 = tgui::VerticalLayout::copy(buttonsWrapper);
			layout->add(buttonsWrapper3);

			clearGridButton = tgui::Button::create();
			clearGridButton->setText("Clear grid");
			clearGridButton->getRenderer()->setTextSize(15);
			clearGridButton->onClick(&engine::Grid::clearGrid, &engine::grid);
			buttonsWrapper->add(clearGridButton);

			fillGridButton = tgui::Button::copy(clearGridButton);
			fillGridButton->setText("Fill grid");
			fillGridButton->onClick(&engine::Grid::fillGrid, &engine::grid);
			buttonsWrapper2->add(fillGridButton);

			randomGridButton = tgui::Button::copy(clearGridButton);
			randomGridButton->setText("Random grid");
			randomGridButton->onClick(&engine::Grid::randomGrid, &engine::grid);
			buttonsWrapper3->add(randomGridButton);

			line = tgui::SeparatorLine::create();
			line->setSize({"100%", 2});
			line->setPosition({0, layout->getPosition().y + layout->getSize().y});
			gui.add(line);
		}

		void onStartButtonClick()
		{
			inProcess = !inProcess;

			if (inProcess) engine::grid.findPath(static_cast<engine::PathfindingMethod>(algorithmSelector->getSelectedItemIndex()));
			else engine::grid.clearPath();

			updateButton();
		}

		void setProcessState(bool state)
		{
			inProcess = state;
			updateButton();
		}

		void updateButton()
		{
			startButton->setImage(inProcess ? "resources/StopIcon.png" : "resources/StartIcon.png");
		}

		void render()
		{
			gui.draw();
		}
	}
}
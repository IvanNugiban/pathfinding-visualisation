
#pragma once

#include "../include/resources.h"

namespace engine {
	namespace ui {
		extern tgui::Gui gui;

		extern tgui::HorizontalWrap::Ptr layout;
		extern tgui::VerticalLayout::Ptr titleWrapper;
		extern tgui::Label::Ptr titleText;
		extern tgui::VerticalLayout::Ptr algorithmWrapper;
		extern tgui::Label::Ptr algorithmText;
		extern tgui::ComboBox::Ptr algorithmSelector;
		extern tgui::VerticalLayout::Ptr speedWrapper;
		extern tgui::Label::Ptr speedText;
		extern tgui::Slider::Ptr speedSlider; 
		extern tgui::VerticalLayout::Ptr audioWrapper;
		extern tgui::Label::Ptr audioText;
		extern tgui::Slider::Ptr audioSlider;
		extern tgui::BitmapButton::Ptr startButton;
		extern tgui::VerticalLayout::Ptr buttonsWrapper;
		extern tgui::Button::Ptr clearGridButton;
		extern tgui::Button::Ptr fillGridButton;
		extern tgui::Button::Ptr randomGridButton;
		extern tgui::SeparatorLine::Ptr line;

		// Is search in process
		extern bool inProcess;

		// Ui initialisation
		void initialize();

		// Ui functions
		void setProcessState(bool state);
		void updateButton();
		void onStartButtonClick();

		void render();
	}
}
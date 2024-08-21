
#include "../include/window.h"
#include "../include/ui.h"
#include "../include/grid.h"
#include "../include/audio.h"

void handleEvents() {
    engine::window::update();
    engine::grid.update();
}

void draw() {
    engine::window::startDrawing();
    engine::ui::render();
    engine::grid.render();
    engine::window::endDrawing();
}

int main()
{
    srand(time(NULL));

    engine::window::create();

    engine::ui::initialize();
    engine::audio::initialize();

    while (engine::window::windowPtr->isOpen())
    {
        handleEvents();
        draw();
    }

}

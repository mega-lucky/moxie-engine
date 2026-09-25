#include <setup.h>

const extern engine_config _config;

int main() {
    Engine engine(_config);

    setup_game(engine);

    while (!engine.MainWindow.ShouldClose()) {
        engine.Update();
    }
}
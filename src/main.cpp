#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "SFML/Graphics/CircleShape.hpp"
#include <imgui-SFML.h>
#include <imgui.h>

void controlPanel() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("This is where controls for the simulation go");
  ImGui::Button("Button");
  ImGui::End();
}

int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "BPHO Computational Challenge");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  sf::Clock deltaClock;
  while (window.isOpen()) {

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100.f, 100.f);

    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    controlPanel();

    window.clear();
    ImGui::SFML::Render(window);
        window.draw(shape);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}

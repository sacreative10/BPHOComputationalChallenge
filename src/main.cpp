#include <imgui-SFML.h>
#include <imgui.h>
#include <math.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#define X_AXIS_OFFSET 20
#define Y_AXIS_OFFSET 20

struct point {
  float x;
  float y;
};

void controlPanel(float &timestep, float &launchSpeed, float &strengthOfGravity,
                  float &launchHeight, int &launchAngle, float &scale) {
  ImGui::Begin("Challenge 1");
  ImGui::Text("General Settings");
  ImGui::SliderFloat("Timestep", &timestep, 0.01, 0.02);
  ImGui::SliderFloat("Strength of Gravity", &strengthOfGravity, 0.1, 10);

  ImGui::Text("Projectile Settings");
  ImGui::SliderInt("Launch Angle", &launchAngle, 0, 90);
  ImGui::SliderFloat("Launch Speed", &launchSpeed, 0, 500);
  ImGui::SliderFloat("Launch Height", &launchHeight, 0, 500);
  ImGui::SliderFloat("Scale Factor", &scale, 0, 100);
  ImGui::End();
}

void challenge1(sf::RenderWindow &window) {
  static float timestep = 0.01;
  static float launchSpeed = 20.f;
  static float strengthOfGravity = 9.81;
  static float launchHeight = 2;
  static int launchAngle = 45;
  static float scale = 1.f;

  controlPanel(timestep, launchSpeed, strengthOfGravity, launchHeight,
               launchAngle, scale);

  std::vector<point> points;

  float launchAngle_rad = launchAngle * 3.14159f / 180.f;
  float t = 0;
  float x = X_AXIS_OFFSET;
  float ux = launchSpeed * std::cos(launchAngle_rad);
  float uy = launchSpeed * std::sin(launchAngle_rad);
  float y = launchHeight + 20;

  points.push_back({x * scale, y * scale});

  while (y >= 0) {
    t += timestep;
    x = X_AXIS_OFFSET + (ux * t * scale);
    y = Y_AXIS_OFFSET +
        (launchHeight + uy * t - 0.5f * strengthOfGravity * t * t) * scale;
    points.push_back({x, y});
  }

  for (point p : points) {
    sf::CircleShape ball(2.f);
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(p.x, 720 - p.y);
    window.draw(ball);
  }
  sf::CircleShape ball(5.f);
  ball.setOrigin(5.f, 5.f);
  ball.setFillColor(sf::Color::Red);
  ball.setPosition(points[0].x, 720 - points[0].y);
  window.draw(ball);
}

void controlPanel2(float &launchSpeed, float &strengthOfGravity,
                   float &launchHeight, int &launchAngle, float &scale,
                   int &numPoints) {
  ImGui::Begin("Challenge 2");
  ImGui::Text("General Settings");
  ImGui::SliderFloat("Strength of Gravity", &strengthOfGravity, 0.1, 10);
  ImGui::SliderInt("Number of Points", &numPoints, 10, 1000);

  ImGui::Text("Projectile Settings");
  ImGui::SliderInt("Launch Angle", &launchAngle, 0, 90);
  ImGui::SliderFloat("Launch Speed", &launchSpeed, 0, 100);
  ImGui::SliderFloat("Launch Height", &launchHeight, 0, 100);
  ImGui::SliderFloat("Scale Factor", &scale, 0, 100);

  float range = launchSpeed * launchSpeed / strengthOfGravity *
                (std::sin(launchAngle * 3.14159f / 180.f) *
                     std::cos(launchAngle * 3.14159f / 180.f) +
                 std::cos(launchAngle * 3.14159f / 180.f) *
                     std::sqrt(std::sin(launchAngle * 3.14159f / 180.f) *
                                   std::sin(launchAngle * 3.14159f / 180.f) +
                               2.f * strengthOfGravity * launchHeight /
                                   (launchSpeed * launchSpeed)));
  ImGui::Text("Range: %f", range);

  float apogeex = ((launchSpeed * launchSpeed) / strengthOfGravity) *
                  (std::sin(launchAngle * 3.14159f / 180.f) *
                   std::cos(launchAngle * 3.14159f / 180.f));
  float apogeey =
      launchHeight + ((launchSpeed * launchSpeed) / (2 * strengthOfGravity)) *
                         (std::sin(launchAngle * 3.14159f / 180.f) *
                          std::sin(launchAngle * 3.14159f / 180.f));

  ImGui::Text("Apogee: (%f, %f)", apogeex, apogeey);

  ImGui::End();
}
void challenge2(sf::RenderWindow &window) {
  static float launchSpeed = 20.f;
  static float strengthOfGravity = 9.81;
  static float launchHeight = 2;
  static int launchAngle = 45;
  static float scale = 1.f;
  static int numPoints = 100;

  controlPanel2(launchSpeed, strengthOfGravity, launchHeight, launchAngle,
                scale, numPoints);

  std::vector<point> points;

  float launchAngle_rad = launchAngle * 3.14159f / 180.f;
  float range =
      launchSpeed * launchSpeed / strengthOfGravity *
      (std::sin(launchAngle_rad) * std::cos(launchAngle_rad) +
       std::cos(launchAngle_rad) *
           std::sqrt(std::sin(launchAngle_rad) * std::sin(launchAngle_rad) +
                     2.f * strengthOfGravity * launchHeight /
                         (launchSpeed * launchSpeed)));
  float fraction_range = 1.f / numPoints;
  for (int i = 0; i < numPoints; i++) {
    float x = X_AXIS_OFFSET + scale * range * i * fraction_range;
    float y =
        X_AXIS_OFFSET + scale * launchHeight + x * std::tan(launchAngle_rad) -
        strengthOfGravity / (2 * launchSpeed * launchSpeed) *
            (1 + std::tan(launchAngle_rad) * std::tan(launchAngle_rad)) * x * x;

    points.push_back({x, y});
  }
  for (point p : points) {
    sf::CircleShape ball(2.f);
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(p.x, 720 - p.y);
    window.draw(ball);
  }
  // apogee calculations

  float xa = scale * (((launchSpeed * launchSpeed) / strengthOfGravity) *
                      (std::sin(launchAngle_rad) * std::cos(launchAngle_rad)));
  float ya =
      Y_AXIS_OFFSET +
      scale * (launchHeight +
               ((launchSpeed * launchSpeed) / (2 * strengthOfGravity)) *
                   (std::sin(launchAngle_rad) * std::sin(launchAngle_rad)));
  sf::CircleShape ball(5.f);
  ball.setOrigin(5.f, 5.f);
  ball.setFillColor(sf::Color::Blue);
  ball.setPosition(xa, 720 - ya);
  window.draw(ball);
}

int RENDER_WIDTH = 1280;
int RENDER_HEIGHT = 720;
void drawGrid(sf::RenderWindow &window) {
  // minor grid lines y axis
  for (int i = 0; i < RENDER_WIDTH; i += Y_AXIS_OFFSET) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(i, 0), sf::Color(255, 255, 255, 50)),
        sf::Vertex(sf::Vector2f(i, RENDER_HEIGHT),
                   sf::Color(255, 255, 255, 50)),
    };
    window.draw(line, 2, sf::Lines);
  }

  // minor grid lines x axis
  for (int i = 0; i < RENDER_HEIGHT; i += X_AXIS_OFFSET) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(0, i), sf::Color(255, 255, 255, 50)),
        sf::Vertex(sf::Vector2f(RENDER_WIDTH, i), sf::Color(255, 255, 255, 50)),
    };
    window.draw(line, 2, sf::Lines);
  }
  sf::Vertex xaxis[] = {
      sf::Vertex(sf::Vector2f(0, 720 - X_AXIS_OFFSET), sf::Color::White),
      sf::Vertex(sf::Vector2f(1280, 720 - X_AXIS_OFFSET), sf::Color::White),
  };
  // y axis
  sf::Vertex yaxis[] = {
      sf::Vertex(sf::Vector2f(Y_AXIS_OFFSET, 0), sf::Color::White),
      sf::Vertex(sf::Vector2f(Y_AXIS_OFFSET, 720), sf::Color::White),
  };

  window.draw(xaxis, 2, sf::Lines);
  window.draw(yaxis, 2, sf::Lines);
}

int main() {
  sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH, RENDER_HEIGHT),
                          "BPHO Computational Challenge");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  int currentChallenge = 0;

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::Resized) {
        RENDER_WIDTH = event.size.width;
        RENDER_HEIGHT = event.size.height;
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    window.clear();

    drawGrid(window);

    ImGui::Begin("Challenge Choice Window");
    ImGui::Text("Choose a challenge to simulate");
    if (ImGui::Button("Challenge 1")) {
      currentChallenge = 1;
    } else if (ImGui::Button("Challenge 2")) {
      currentChallenge = 2;
    }
    if (currentChallenge != 0) {
      if (ImGui::Button("Reset")) {
        currentChallenge = 0;
      }
    }
    ImGui::End();

    switch (currentChallenge) {
      case 1:
        challenge1(window);
        break;
      case 2:
        challenge2(window);
        break;
    }

    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}

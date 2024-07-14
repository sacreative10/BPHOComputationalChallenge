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

std::vector<point> cartesianProjectile(float angle_deg, float strengthOfGravity,
                                       float launchSpeed, float launchHeight,
                                       int numPoints, float scale) {
  std::vector<point> points;

  float launchAngle_rad = angle_deg * 3.14159f / 180.f;
  float range =
      launchSpeed * launchSpeed / strengthOfGravity *
      (std::sin(launchAngle_rad) * std::cos(launchAngle_rad) +
       std::cos(launchAngle_rad) *
           std::sqrt(std::sin(launchAngle_rad) * std::sin(launchAngle_rad) +
                     2.f * strengthOfGravity * launchHeight /
                         (launchSpeed * launchSpeed)));
  float fraction_range = 1.f / numPoints;
  for (int i = 0; i < numPoints; i++) {
    float x = scale * range * i * fraction_range;
    float y =
        scale * launchHeight + x * std::tan(launchAngle_rad) -
        strengthOfGravity / (2 * launchSpeed * launchSpeed) *
            (1 + std::tan(launchAngle_rad) * std::tan(launchAngle_rad)) * x * x;

    points.push_back({x + X_AXIS_OFFSET, y + Y_AXIS_OFFSET});
  }

  return points;
}

void drawPoints(sf::RenderWindow &window, std::vector<point> points,
                sf::Color color = sf::Color::Green) {
  for (point p : points) {
    sf::CircleShape ball(2.f);
    ball.setFillColor(color);
    ball.setPosition(p.x, 720 - p.y);
    window.draw(ball);
  }
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

  points = cartesianProjectile(launchAngle, strengthOfGravity, launchSpeed,
                               launchHeight, numPoints, scale);

  drawPoints(window, points);

  // apogee calculations

  float launchAngle_rad = launchAngle * 3.14159f / 180.f;

  float xa = scale * (((launchSpeed * launchSpeed) / strengthOfGravity) *
                      (std::sin(launchAngle_rad) * std::cos(launchAngle_rad)));
  float ya =
      scale * (launchHeight +
               ((launchSpeed * launchSpeed) / (2 * strengthOfGravity)) *
                   (std::sin(launchAngle_rad) * std::sin(launchAngle_rad)));
  sf::CircleShape ball(5.f);
  ball.setOrigin(5.f, 5.f);
  ball.setFillColor(sf::Color::Blue);
  ball.setPosition(xa + X_AXIS_OFFSET, 720 - ya - Y_AXIS_OFFSET);
  window.draw(ball);
}

bool choosePoint = false;

void controlpanel3(int *targetX, int *targetY, float *strengthOfGravity,
                   float *launchSpeed, float minSpeed, float * launchHeight) {
  ImGui::Begin("Challenge 3");

  ImGui::Text("Legend");
  ImGui::TextColored(ImColor(1.f, 0.f, 1.f, 1.f), "Magenta: Target");
  ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Blue: High Ball");
  ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Red: Low Ball");
  ImGui::TextColored(ImColor(0.f, 1.f, 0.f, 1.f), "Green: Minimum Speed");

  ImGui::Text("General Settings");
  ImGui::InputInt("Target X", targetX, 0, 1000);
  ImGui::InputInt("Target Y", targetY, 0, 1000);
  if (ImGui::Button(choosePoint ? "Choosing point using mouse, Left Click to confirm"
    : "Choose Point using mouse")) {
    choosePoint = !choosePoint;
  }
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.1, 10);
  ImGui::Text("Minimum Launch Speed: %f", minSpeed);
  ImGui::SliderFloat("Launch Speed", launchSpeed, minSpeed, minSpeed + (minSpeed * 2));
  ImGui::SliderFloat("Launch Height", launchHeight, 0, 500);


  ImGui::End();
}

void challenge3(sf::RenderWindow &window, int& targetX, int& targetY) {
  // inputs
  static float strengthOfGravity = 9.81;
  static float launchHeight = 0;

  float tempTargetY = targetY - launchHeight;
  float minimumLaunchSpeed =
      sqrt(strengthOfGravity) *
      sqrt(tempTargetY + sqrt(tempTargetY * tempTargetY + targetX * targetX));
  static float launchSpeed = minimumLaunchSpeed;
  controlpanel3(&targetX, &targetY, &strengthOfGravity, &launchSpeed,
                minimumLaunchSpeed, &launchHeight);

  // calculations
  float minimumspeedangle =
      atan((tempTargetY + sqrt(tempTargetY * tempTargetY + targetX * targetX)) / targetX);


  std::vector<point> minimumspeedpoints =
      cartesianProjectile(minimumspeedangle * 180.f / 3.14159f,
                          strengthOfGravity, minimumLaunchSpeed, launchHeight, 100, 1.f);

  drawPoints(window, minimumspeedpoints);

  float a =
      targetX * targetX * (strengthOfGravity / (2 * launchSpeed * launchSpeed));
  float b = -targetX;
  float c = targetY - launchHeight + ((strengthOfGravity * targetX * targetX) /
                          (2 * launchSpeed * launchSpeed));

  float discriminant = b * b - 4 * a * c;
  float mintantheta = (-b + sqrt(discriminant)) / (2 * a);
  float maxtantheta = (-b - sqrt(discriminant)) / (2 * a);

  float minangle = atan(mintantheta);
  float maxangle = atan(maxtantheta);

  std::vector<point> highBallpoints = cartesianProjectile(
      maxangle * 180.f / 3.14159f, strengthOfGravity, launchSpeed, launchHeight, 100, 1.f);
  std::vector<point> lowBallpoints = cartesianProjectile(
      minangle * 180.f / 3.14159f, strengthOfGravity, launchSpeed, launchHeight, 100, 1.f);

  drawPoints(window, highBallpoints, sf::Color::Red);
  drawPoints(window, lowBallpoints, sf::Color::Blue);


  sf::CircleShape ball(5.f);
  ball.setOrigin(5.f, 5.f);
  ball.setFillColor(sf::Color::Magenta);
  ball.setPosition(targetX + X_AXIS_OFFSET, 720 - targetY - Y_AXIS_OFFSET);
  window.draw(ball);
}


void controlPanel4(float* launchSpeed, float* launchHeight, float* strengthOfGravity, int* launchAngle, float bestlaunchAngle = 0, float currentRange = 0, float maxRange = 0)
{
  ImGui::Begin("Challenge 4 Controls");
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.1, 10);

  ImGui::Text("Projectile Settings");
  ImGui::SliderInt("Launch Angle", launchAngle, 0, 90);
  ImGui::SliderFloat("Launch Speed", launchSpeed, 0, 500);
  ImGui::SliderFloat("Launch Height", launchHeight, 0, 500);

  ImGui::Text("Current Range: %f", currentRange);
  ImGui::Text("Best Launch Angle: %f", bestlaunchAngle);
  ImGui::Text("Best Range: %f", maxRange);
  ImGui::End();
}

void challenge4(sf::RenderWindow& window)
{
  static float launchSpeed = 20.f;
  static float strengthOfGravity = 9.81;
  static float launchHeight = 0;
  static int launchAngle = 45;
  static float scale = 1.f;
  static int numPoints = 100;
  static float launchAngle_rad = launchAngle * 3.14159f / 180.f;

  // Range calculations
  float current_Range = (launchSpeed * launchSpeed)/strengthOfGravity * (std::sin(launchAngle_rad) * std::cos(launchAngle_rad) + std::cos(launchAngle_rad) * sqrt(std::sin(launchAngle_rad) * std::sin(launchAngle_rad) +2.f * strengthOfGravity * launchHeight/(launchSpeed * launchSpeed)));
  float max_theta_rad = std::asin(1.f/(sqrt(2.f+2.f*strengthOfGravity* launchHeight/(launchSpeed*launchSpeed))));
  float max_theta_deg = max_theta_rad * 180.f/3.14159f;
  float max_Range = launchSpeed * launchSpeed/strengthOfGravity * sqrt(1.f+2.f * strengthOfGravity * launchHeight/(launchSpeed * launchSpeed));

  float bestLaunchAngle = max_theta_deg;

  controlPanel4(&launchSpeed, &launchHeight, &strengthOfGravity, &launchAngle, bestLaunchAngle, current_Range, max_Range);

  std::vector<point> userParabola = cartesianProjectile(launchAngle, strengthOfGravity, launchSpeed, launchHeight, numPoints, scale);
  std::vector<point> bestParabola = cartesianProjectile(bestLaunchAngle, strengthOfGravity, launchSpeed, launchHeight, numPoints, scale);

  drawPoints(window, userParabola, sf::Color::Green);
  drawPoints(window, bestParabola, sf::Color::Magenta);
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
    static int targetX = X_AXIS_OFFSET + 100;
    static int targetY = Y_AXIS_OFFSET + 100;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::Resized) {
        RENDER_WIDTH = event.size.width;
        RENDER_HEIGHT = event.size.height;
      }
      if (choosePoint) {
        if (event.type == sf::Event::MouseMoved) {
          targetX = event.mouseMove.x - X_AXIS_OFFSET;
          targetY = RENDER_HEIGHT - event.mouseMove.y - Y_AXIS_OFFSET;
        }
        if (event.type == sf::Event::MouseButtonPressed)
          choosePoint = false;
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
    } else if(ImGui::Button("Challenge 3")) {
      currentChallenge = 3;
    } else if(ImGui::Button("Challenge 4")) {
      currentChallenge = 4;
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
      case 3:
        challenge3(window, targetX, targetY);
        break;
      case 4:
        challenge4(window);
    }

    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}
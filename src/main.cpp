#include <imgui-SFML.h>
#include <imgui.h>
#include <math.h>
#include <iostream>

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

//
struct point {
  float x;
  float y;
};

int RENDER_WIDTH = 1280;
int RENDER_HEIGHT = 720;


inline void SetupImGuiStyle(bool bStyleDark_, float alpha_) {
  ImGuiStyle& style = ImGui::GetStyle();

  // light style from Pacôme Danhiez (user itamago)
  // https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
  style.Alpha = 1.0f;
  style.FrameRounding = 3.0f;
  style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] =
      ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

  if (bStyleDark_) {
    for (int i = 0; i <= ImGuiCol_COUNT; i++) {
      ImVec4& col = style.Colors[i];
      float H, S, V;
      ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

      if (S < 0.1f) {
        V = 1.0f - V;
      }
      ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
      if (col.w < 1.00f) {
        col.w *= alpha_;
      }
    }
  } else {
    for (int i = 0; i <= ImGuiCol_COUNT; i++) {
      ImVec4& col = style.Colors[i];
      if (col.w < 1.00f) {
        col.x *= alpha_;
        col.y *= alpha_;
        col.z *= alpha_;
        col.w *= alpha_;
      }
    }
  }
}


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
    float y = scale * launchHeight + x * std::tan(launchAngle_rad) -
              strengthOfGravity / (2 * launchSpeed * launchSpeed) *
                  (1 + std::tan(launchAngle_rad) * std::tan(launchAngle_rad)) *
                  x * x;

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
                   float *launchSpeed, float minSpeed, float *launchHeight) {
  ImGui::Begin("Challenge 3");

  ImGui::Text("Legend");
  ImGui::TextColored(ImColor(1.f, 0.f, 1.f, 1.f), "Magenta: Target");
  ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Blue: High Ball");
  ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Red: Low Ball");
  ImGui::TextColored(ImColor(0.f, 1.f, 0.f, 1.f), "Green: Minimum Speed");

  ImGui::Text("General Settings");
  ImGui::InputInt("Target X", targetX, 0, 1000);
  ImGui::InputInt("Target Y", targetY, 0, 1000);
  if (ImGui::Button(choosePoint
                        ? "Choosing point using mouse, Left Click to confirm"
                        : "Choose Point using mouse")) {
    choosePoint = !choosePoint;
  }
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.1, 10);
  ImGui::Text("Minimum Launch Speed: %f", minSpeed);
  ImGui::SliderFloat("Launch Speed", launchSpeed, minSpeed,
                     minSpeed + (minSpeed * 2));
  ImGui::SliderFloat("Launch Height", launchHeight, 0, 500);

  ImGui::End();
}

void challenge3(sf::RenderWindow &window, int &targetX, int &targetY) {
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
      atan((tempTargetY + sqrt(tempTargetY * tempTargetY + targetX * targetX)) /
           targetX);

  std::vector<point> minimumspeedpoints = cartesianProjectile(
      minimumspeedangle * 180.f / 3.14159f, strengthOfGravity,
      minimumLaunchSpeed, launchHeight, 100, 1.f);

  drawPoints(window, minimumspeedpoints);

  float a =
      targetX * targetX * (strengthOfGravity / (2 * launchSpeed * launchSpeed));
  float b = -targetX;
  float c = targetY - launchHeight +
            ((strengthOfGravity * targetX * targetX) /
             (2 * launchSpeed * launchSpeed));

  float discriminant = b * b - 4 * a * c;
  float mintantheta = (-b + sqrt(discriminant)) / (2 * a);
  float maxtantheta = (-b - sqrt(discriminant)) / (2 * a);

  float minangle = atan(mintantheta);
  float maxangle = atan(maxtantheta);

  std::vector<point> highBallpoints =
      cartesianProjectile(maxangle * 180.f / 3.14159f, strengthOfGravity,
                          launchSpeed, launchHeight, 100, 1.f);
  std::vector<point> lowBallpoints =
      cartesianProjectile(minangle * 180.f / 3.14159f, strengthOfGravity,
                          launchSpeed, launchHeight, 100, 1.f);

  drawPoints(window, highBallpoints, sf::Color::Red);
  drawPoints(window, lowBallpoints, sf::Color::Blue);

  sf::CircleShape ball(5.f);
  ball.setOrigin(5.f, 5.f);
  ball.setFillColor(sf::Color::Magenta);
  ball.setPosition(targetX + X_AXIS_OFFSET, 720 - targetY - Y_AXIS_OFFSET);
  window.draw(ball);
}

void controlPanel4(float *launchSpeed, float *launchHeight,
                   float *strengthOfGravity, int *launchAngle,
                   float bestlaunchAngle = 0, float currentRange = 0,
                   float maxRange = 0) {
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

void challenge4(sf::RenderWindow &window) {
  static float launchSpeed = 20.f;
  static float strengthOfGravity = 9.81;
  static float launchHeight = 0;
  static int launchAngle = 45;
  static float scale = 1.f;
  static int numPoints = 100;
  static float launchAngle_rad = launchAngle * 3.14159f / 180.f;

  // Range calculations
  float current_Range =
      launchSpeed * launchSpeed / strengthOfGravity *
      (std::sin(launchAngle * 3.14159f / 180.f) *
           std::cos(launchAngle * 3.14159f / 180.f) +
       std::cos(launchAngle * 3.14159f / 180.f) *
           std::sqrt(std::sin(launchAngle * 3.14159f / 180.f) *
                         std::sin(launchAngle * 3.14159f / 180.f) +
                     2.f * strengthOfGravity * launchHeight /
                         (launchSpeed * launchSpeed)));
  float max_theta_rad =
      std::asin(1.f / (sqrt(2.f + 2.f * strengthOfGravity * launchHeight /
                                      (launchSpeed * launchSpeed))));
  float max_theta_deg = max_theta_rad * 180.f / 3.14159f;
  float max_Range = launchSpeed * launchSpeed / strengthOfGravity *
                    sqrt(1.f + 2.f * strengthOfGravity * launchHeight /
                                   (launchSpeed * launchSpeed));

  float bestLaunchAngle = max_theta_deg;

  controlPanel4(&launchSpeed, &launchHeight, &strengthOfGravity, &launchAngle,
                bestLaunchAngle, current_Range, max_Range);

  std::vector<point> userParabola =
      cartesianProjectile(launchAngle, strengthOfGravity, launchSpeed,
                          launchHeight, numPoints, scale);
  std::vector<point> bestParabola =
      cartesianProjectile(bestLaunchAngle, strengthOfGravity, launchSpeed,
                          launchHeight, numPoints, scale);

  drawPoints(window, userParabola, sf::Color::Green);
  drawPoints(window, bestParabola, sf::Color::Magenta);
}

std::vector<point> boundingParabola(float strengthOfGravity, float launchSpeed,
                                    float launchHeight, int numPoints) {
  std::vector<point> points;

  float range = sqrt(
      (2.f * launchSpeed * launchSpeed *
       (launchSpeed * launchSpeed / (2.f * strengthOfGravity) + launchHeight)) /
      strengthOfGravity);
  float fraction_range = 1.f / numPoints;
  for (int i = 0; i < numPoints; i++) {
    float x = range * i * fraction_range;
    float y = launchHeight +
              launchSpeed * launchSpeed / (2.f * strengthOfGravity) -
              strengthOfGravity / (2.f * launchSpeed * launchSpeed) * x * x;

    points.push_back({x + X_AXIS_OFFSET, y + Y_AXIS_OFFSET});
  }

  return points;
}

void controlpanel5(int *targetX, int *targetY, float *strengthOfGravity,
                   float *launchSpeed, float minSpeed, float *launchHeight) {
  ImGui::Begin("Challenge 5");

  ImGui::Text("Legend");
  ImGui::TextColored(ImColor(1.f, 0.f, 1.f, 1.f), "Purple: Bounding Parabola");
  ImGui::TextColored(ImColor(1.f, 0.f, 0.f, 1.f), "Red: Max Range");
  ImGui::TextColored(ImColor(0.f, 0.f, 1.f, 1.f), "Blue: High ball");
  ImGui::TextColored(ImColor(0.f, 1.f, 0.f, 1.f), "Green: Low ball");
  ImGui::TextColored(ImColor(1.f, 1.f, 1.f, 1.f), "White: Min U");

  ImGui::Text("General Settings");
  ImGui::InputInt("Target X", targetX, 0, 1000);
  ImGui::InputInt("Target Y", targetY, 0, 1000);
  if (ImGui::Button(choosePoint
                        ? "Choosing point using mouse, Left Click to confirm"
                        : "Choose Point using mouse")) {
    choosePoint = !choosePoint;
  }
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.1, 10);
  ImGui::Text("Minimum Launch Speed: %f", minSpeed);
  ImGui::SliderFloat("Launch Speed", launchSpeed, minSpeed,
                     minSpeed + (minSpeed * 2));
  ImGui::SliderFloat("Launch Height", launchHeight, 0, 500);

  ImGui::End();
}

void challenge5(sf::RenderWindow &window, int &targetX, int &targetY) {
  // inputs
  static float strengthOfGravity = 9.81;
  static float launchHeight = 0;
  int numPoints = 100;
  int scale = 1.f;

  float tempTargetY = targetY - launchHeight;
  float minimumLaunchSpeed =
      sqrt(strengthOfGravity) *
      sqrt(tempTargetY + sqrt(tempTargetY * tempTargetY + targetX * targetX));
  static float launchSpeed = minimumLaunchSpeed;
  controlpanel5(&targetX, &targetY, &strengthOfGravity, &launchSpeed,
                minimumLaunchSpeed, &launchHeight);

  // calculations
  float minimumspeedangle =
      atan((tempTargetY + sqrt(tempTargetY * tempTargetY + targetX * targetX)) /
           targetX);

  std::vector<point> minimumspeedpoints = cartesianProjectile(
      minimumspeedangle * 180.f / 3.14159f, strengthOfGravity,
      minimumLaunchSpeed, launchHeight, 100, 1.f);

  float a =
      targetX * targetX * (strengthOfGravity / (2 * launchSpeed * launchSpeed));
  float b = -targetX;
  float c = targetY - launchHeight +
            ((strengthOfGravity * targetX * targetX) /
             (2 * launchSpeed * launchSpeed));

  float discriminant = b * b - 4 * a * c;
  float mintantheta = (-b + sqrt(discriminant)) / (2 * a);
  float maxtantheta = (-b - sqrt(discriminant)) / (2 * a);

  float minangle = atan(mintantheta);
  float maxangle = atan(maxtantheta);

  std::vector<point> highBallpoints =
      cartesianProjectile(maxangle * 180.f / 3.14159f, strengthOfGravity,
                          launchSpeed, launchHeight, 100, 1.f);
  std::vector<point> lowBallpoints =
      cartesianProjectile(minangle * 180.f / 3.14159f, strengthOfGravity,
                          launchSpeed, launchHeight, 100, 1.f);

  // max range

  float max_theta_rad =
      std::asin(1.f / (sqrt(2.f + 2.f * strengthOfGravity * launchHeight /
                                      (launchSpeed * launchSpeed))));
  float max_theta_deg = max_theta_rad * 180.f / 3.14159f;
  std::vector<point> bestParabola =
      cartesianProjectile(max_theta_deg, strengthOfGravity, launchSpeed,
                          launchHeight, numPoints, scale);

  // bounding parabola
  std::vector<point> boundingParabola_ =
      boundingParabola(strengthOfGravity, launchSpeed, launchHeight, numPoints);

  drawPoints(window, minimumspeedpoints, sf::Color::White);
  drawPoints(window, highBallpoints, sf::Color::Blue);
  drawPoints(window, lowBallpoints, sf::Color::Green);
  drawPoints(window, bestParabola, sf::Color::Red);
  drawPoints(window, boundingParabola_, sf::Color::Magenta);

  sf::CircleShape ball(5.f);
  ball.setOrigin(5.f, 5.f);
  ball.setFillColor(sf::Color::Magenta);
  ball.setPosition(targetX + X_AXIS_OFFSET, 720 - targetY - Y_AXIS_OFFSET);
  window.draw(ball);
}

void controlPanel6(float *launchSpeed, float *launchHeight,
                   float *strengthOfGravity, int *launchAngle,
                   float bestlaunchAngle = 0, float currentRange = 0,
                   float maxRange = 0, float currentDistance = 0,
                   float distanceMax = 0) {
  ImGui::Begin("Challenge 6 Controls");
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.1, 10);

  ImGui::Text("Projectile Settings");
  ImGui::SliderInt("Launch Angle", launchAngle, 0, 90);
  ImGui::SliderFloat("Launch Speed", launchSpeed, 0, 500);
  ImGui::SliderFloat("Launch Height", launchHeight, 0, 500);

  ImGui::Text("Current Range: %f", currentRange);
  ImGui::Text("Current Distance: %f", currentDistance);
  ImGui::Text("Best Launch Angle: %f", bestlaunchAngle);
  ImGui::Text("Best Range: %f", maxRange);
  ImGui::Text("Best Distance: %f", distanceMax);
  ImGui::End();
}

float integral(float x) {
  return 0.5 * log(abs(sqrt(1 + x * x) + x)) + 0.5 * x * sqrt(1 + x * x);
}

void challenge6(sf::RenderWindow &window) {
  static float launchSpeed = 20.f;
  static float strengthOfGravity = 9.81;
  static float launchHeight = 0;
  static int launchAngle = 45;
  static float scale = 1.f;
  static int numPoints = 100;
  static float launchAngle_rad = launchAngle * 3.14159f / 180.f;

  // Range calculations
  float current_Range =
      launchSpeed * launchSpeed / strengthOfGravity *
      (std::sin(launchAngle * 3.14159f / 180.f) *
           std::cos(launchAngle * 3.14159f / 180.f) +
       std::cos(launchAngle * 3.14159f / 180.f) *
           std::sqrt(std::sin(launchAngle * 3.14159f / 180.f) *
                         std::sin(launchAngle * 3.14159f / 180.f) +
                     2.f * strengthOfGravity * launchHeight /
                         (launchSpeed * launchSpeed)));
  float max_theta_rad =
      std::asin(1.f / (sqrt(2.f + 2.f * strengthOfGravity * launchHeight /
                                      (launchSpeed * launchSpeed))));
  float max_theta_deg = max_theta_rad * 180.f / 3.14159f;
  float max_Range = launchSpeed * launchSpeed / strengthOfGravity *
                    sqrt(1.f + 2.f * strengthOfGravity * launchHeight /
                                   (launchSpeed * launchSpeed));

  float bestLaunchAngle = max_theta_deg;

  float currentDistanceIntegrand =
      integral(std::tan(launchAngle_rad)) -
      integral(
          std::tan(launchAngle_rad) -
          ((strengthOfGravity * current_Range) / (launchSpeed * launchSpeed)) *
              (1 + (std::tan(launchAngle_rad) * std::tan(launchAngle_rad))));

  float bestDistanceIntegrand =
      integral(std::tan(max_theta_rad)) -
      integral(std::tan(max_theta_rad) -
               ((strengthOfGravity * max_Range) / (launchSpeed * launchSpeed)) *
                   (1 + std::tan(max_theta_rad) * std::tan(max_theta_rad)));

  float currentDistance =
      ((launchSpeed * launchSpeed) /
       (strengthOfGravity *
        (1 + std::tan(launchAngle_rad) * std::tan(launchAngle_rad)))) *
      currentDistanceIntegrand;
  float bestDistance =
      ((launchSpeed * launchSpeed) /
       (strengthOfGravity *
        (1 + std::tan(max_theta_rad) * std::tan(max_theta_rad)))) *
      bestDistanceIntegrand;

  controlPanel6(&launchSpeed, &launchHeight, &strengthOfGravity, &launchAngle,
                bestLaunchAngle, current_Range, max_Range, currentDistance,
                bestDistance);

  std::vector<point> userParabola =
      cartesianProjectile(launchAngle, strengthOfGravity, launchSpeed,
                          launchHeight, numPoints, scale);
  std::vector<point> bestParabola =
      cartesianProjectile(bestLaunchAngle, strengthOfGravity, launchSpeed,
                          launchHeight, numPoints, scale);

  drawPoints(window, userParabola, sf::Color::Green);
  drawPoints(window, bestParabola, sf::Color::Magenta);
}

std::vector<point> rangePlots(float launchSpeed, float launchHeight,
                              int num_Points, float deg_angle,
                              float strength_of_gravity) {
  std::vector<point> points;

  float max_time = 10;
  float angle_rad = deg_angle * 3.14159f / 180.f;

  float t_to_x_axis = (float)RENDER_WIDTH / (float)max_time;
  // Suggestion: work out absolute scale, by working out the best range from the
  // given configuration
  float range_to_y_axis = (float)RENDER_HEIGHT / (float)100;

  float dt = max_time / num_Points;
  for (float i = 0; i <= max_time; i += dt) {
    float r = sqrt((launchSpeed * launchSpeed * i * i * std::cos(angle_rad) *
                    std::cos(angle_rad)) +
                   (launchSpeed * i * std::sin(angle_rad) -
                    0.5f * strength_of_gravity * i * i + launchHeight) *
                       (launchSpeed * i * std::sin(angle_rad) -
                        0.5f * strength_of_gravity * i * i + launchHeight));
    points.push_back({((i * t_to_x_axis) + X_AXIS_OFFSET),
                      ((r * range_to_y_axis) + Y_AXIS_OFFSET)});
  };
  return points;
}

void controlPanel7(float *launchSpeed, float *launchHeight,
                   float *strengthOfGravity, bool *seeRangePlot) {
  ImGui::Begin("Challenge 7 Controls");
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.1, 20);
  ImGui::SliderFloat("Launch Speed", launchSpeed, 0.1, 1000);
  ImGui::SliderFloat("Launch Height", launchHeight, 0.1, 1000);
  if (ImGui::Button("Toggle Range Plot")) *seeRangePlot = !(*seeRangePlot);

  ImGui::End();
}

void challenge7(sf::RenderWindow &window) {
  std::vector<float> angles = {30, 45, 60, 70.5, 78, 85};

  std::vector<sf::Color> colors = {sf::Color::Blue,    sf::Color::Green,
                                   sf::Color::Red,     sf::Color::Cyan,
                                   sf::Color::Magenta, sf::Color::Yellow};

  static float launchSpeed = 20.f;
  static float launchHeight = 0;
  static float strengthOfGravity = 10.f;
  static bool seeRangePlot = true;

  controlPanel7(&launchSpeed, &launchHeight, &strengthOfGravity, &seeRangePlot);

  if (seeRangePlot) {
    for (int i = 0; i <= 6; i++) {
      float angle = angles[i];
      auto color = colors[i];

      auto points =
          rangePlots(launchSpeed, launchHeight, 1000, angle, strengthOfGravity);
      drawPoints(window, points, color);
    }

  } else {
    for (int i = 0; i <= 6; i++) {
      float angle = angles[i];
      auto color = colors[i];

      auto points =
          cartesianProjectile(angle, 9.81, launchSpeed, launchHeight, 100, 1);
      drawPoints(window, points, color);
    }
  }
}

std::vector<point> cartesianProjectile(float angle_deg, float strengthOfGravity,
                                       float launchSpeed, float launchHeight,
                                       int numPoints, float scale,
                                       float startingX) {
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
    float x = startingX + (scale * range * i * fraction_range);
    float y = scale * launchHeight + x * std::tan(launchAngle_rad) -
              strengthOfGravity / (2 * launchSpeed * launchSpeed) *
                  (1 + std::tan(launchAngle_rad) * std::tan(launchAngle_rad)) *
                  x * x;

    points.push_back({x + X_AXIS_OFFSET, y + Y_AXIS_OFFSET});
  }

  return points;
}

float quadratic(float a, float b, float c) {
  return (-b + sqrt((b * b) + (4 * a * c))) / (2.f * a);
}
void controlPanel8(int *max_bounces, float *coeffRes, float *launchSpeed,
                   float *launchHeight, float *launchAngle,
                   float *strengthOfGravity) {
  ImGui::Begin("Challenge 8");
  ImGui::InputInt("Max Bounces: ", max_bounces);
  ImGui::SliderFloat("Coefficient of Res: ", coeffRes, 0.f, 1.f);
  ImGui::SliderFloat("Launch Speed", launchSpeed, 0.f, 1000.f);
  ImGui::SliderFloat("Launch Height", launchHeight, 0.f, (float)RENDER_HEIGHT);
  ImGui::SliderFloat("Launch Angle", launchAngle, 0.f, 90.f);
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.f, 20.f);

  ImGui::End();
}


std::vector<point> challenge8computation(sf::RenderWindow& window) {
  static int max_bounces = 10;
  static float launchSpeed = 20;
  static float coeffRes = 0.7f;
  static float launchHeight = 20;
  static float launchAngle = 45;
  static float strengthOfGravity = 9.81;

  controlPanel8(&max_bounces, &coeffRes, &launchSpeed, &launchHeight,
                &launchAngle, &strengthOfGravity);

  std::vector<point> points;

  float dt = 0.001;
  float theta = launchAngle * 3.14159f / 180.f;

  float velX = std::cos(theta) * launchSpeed;
  float velY = std::sin(theta) * launchSpeed;

  float posX = Y_AXIS_OFFSET;
  float posY = launchHeight + X_AXIS_OFFSET;

  int numBounces = 0;

  while (numBounces <= max_bounces)
  {
    posX += velX * dt;
    posY += velY * dt - 0.5f * dt * dt * strengthOfGravity;

    velX = velX;
    velY += -strengthOfGravity * dt;

    if (posY < X_AXIS_OFFSET)
    {
      numBounces++;
      posY = X_AXIS_OFFSET;
      velY = -coeffRes * velY;
    }   

    points.push_back({posX, posY});
  }

  drawPoints(window, points);

  return points;
}

void controlPanel9(float *launchSpeed, float *launchHeight, float *launchAngle,
                   float *strengthOfGravity, float *dragCoeef, float *crossA, float* airDensity, float* objectMass, int *plotToggle) {
  ImGui::Begin("Challenge 9");
  ImGui::SliderFloat("Launch Speed", launchSpeed, 0.f, 1000.f);
  ImGui::SliderFloat("Launch Height", launchHeight, 0.f, (float)RENDER_HEIGHT);
  ImGui::SliderFloat("Launch Angle", launchAngle, 0.f, 90.f);
  ImGui::SliderFloat("Strength of Gravity", strengthOfGravity, 0.f, 20.f);
  ImGui::SliderFloat("Drag Coefficient", dragCoeef, 0.f, 1.f);
  ImGui::SliderFloat("Cross Area", crossA, 0.f, 1.f);
  ImGui::SliderFloat("Air Density", airDensity, 0.f, 10.f);
  ImGui::SliderFloat("Object Mass", objectMass, 0.f, 90.f);
  ImGui::SliderInt("Plot Toggle", plotToggle, 0, 4);
  ImGui::Text("Plot Toggle Information:");
  ImGui::Text("0: Vertical vs Horizontal Displacement");
  ImGui::Text("1: Vertical Displacement vs Time");
  ImGui::Text("2: Horizontal velocity vs Time");
  ImGui::Text("3: Vertical velocity vs Time");
  ImGui::Text("4: Overall Velocity vs Time");
  ImGui::End();
}
void challenge9(sf::RenderWindow& window) {
  static float launchSpeed = 20;
  static float launchHeight = 20;
  static float launchAngle = 45;
  static float strengthOfGravity = 9.81;
  static float dragCoeff = 0.1f;
  static float crossArea = 0.1f;
  static float airDensity = 1.f;
  static float objectMass = 0.1f;
  static int plotToggle = 0;


  controlPanel9(&launchSpeed, &launchHeight,
                &launchAngle, &strengthOfGravity, &dragCoeff, &crossArea, &airDensity, &objectMass, &plotToggle);

  std::vector<point> points;

  float k = 0.5f * dragCoeff * airDensity * crossArea * (1.f / objectMass);

  float dt = 0.01;
  float theta = launchAngle * 3.14159f / 180.f;

  float velX = std::cos(theta) * launchSpeed;
  float velY = std::sin(theta) * launchSpeed;
  float vel = launchSpeed;

  float posX = Y_AXIS_OFFSET;
  float posY = launchHeight + X_AXIS_OFFSET;

  float accX = -velX / vel * k * vel * vel;
  float accY = -strengthOfGravity -velY / vel * k * vel * vel;
  int dtCounter = 0;


  while (posY >= X_AXIS_OFFSET)
  {
  
    posX += velX * dt;
    posY += velY * dt - 0.5f * dt * dt * strengthOfGravity;

    velX += accX * dt;
    velY += accY * dt;

    vel = sqrt(velX * velX + velY * velY);

    accX = -(velX / vel) * k * vel * vel;
    accY = -strengthOfGravity - (velY / vel) * k * vel * vel;
    dtCounter ++;

    switch(plotToggle){
      case 0:
        points.push_back({posX, posY});
        break;
      case 1:
        points.push_back({dtCounter+X_AXIS_OFFSET, posY});
        break;
      case 2:
        points.push_back({dtCounter+X_AXIS_OFFSET, velX+Y_AXIS_OFFSET});
        break;
      case 3:
        points.push_back({dtCounter+X_AXIS_OFFSET, velY+Y_AXIS_OFFSET});
        break;
      case 4:
        points.push_back({dtCounter+X_AXIS_OFFSET, vel+Y_AXIS_OFFSET});
        break;
    }
  }

  std::vector<point> cartesian_points;
  dtCounter = 0;
  dt = 0.01;
  theta = launchAngle * 3.14159f / 180.f;

  velX = std::cos(theta) * launchSpeed;
  velY = std::sin(theta) * launchSpeed;
  vel = launchSpeed;

  posX = Y_AXIS_OFFSET;
  posY = launchHeight + X_AXIS_OFFSET;

  accX = 0;
  accY = -strengthOfGravity;
  while (posY >=Y_AXIS_OFFSET){
    posX += velX * dt;
    posY += velY * dt - 0.5f * dt * dt * strengthOfGravity;

    velX += accX * dt;
    velY += accY * dt;
    vel = sqrt(velX * velX + velY * velY);

    dtCounter ++;

    switch(plotToggle){
      case 0:
        cartesian_points.push_back({posX, posY});
        break;
      case 1:
        cartesian_points.push_back({dtCounter+X_AXIS_OFFSET, posY});
        break;
      case 2:
        cartesian_points.push_back({dtCounter+X_AXIS_OFFSET, velX+Y_AXIS_OFFSET});
        break;
      case 3:
        cartesian_points.push_back({dtCounter+X_AXIS_OFFSET, velY+Y_AXIS_OFFSET});
        break;
      case 4:
        cartesian_points.push_back({dtCounter+X_AXIS_OFFSET, vel+Y_AXIS_OFFSET});
        break;
    }
  
  }

  drawPoints(window, points);
  drawPoints(window, cartesian_points, sf::Color::Red);
}



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
  ImGui::SFML::Init(window, false);

  ImGuiIO& io = ImGui::GetIO();
  ImGui::CreateContext();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = "resources/imgui.ini";
  io.Fonts->AddFontFromFileTTF("resources/JetBrainsMono-Medium.ttf", 17);
  // io.Fonts->AddFontDefault();
  io.Fonts->Build();
  ImGui::SFML::UpdateFontTexture();

  SetupImGuiStyle(true, 0.5f);


  int currentChallenge = 0;

  sf::Clock deltaClock;
  sf::Clock challenge8Clock;
  challenge8Clock.restart();
  std::vector<point> challenge8points;
  int challenge8iter = 0;
  while (window.isOpen()) {
    sf::Event event;
    static int targetX = X_AXIS_OFFSET + 100;
    static int targetY = Y_AXIS_OFFSET + 100;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.KeyPressed == sf::Keyboard::Escape) {
          window.close();
        }
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
        if (event.type == sf::Event::MouseButtonPressed) choosePoint = false;
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
    } else if (ImGui::Button("Challenge 3")) {
      currentChallenge = 3;
    } else if (ImGui::Button("Challenge 4")) {
      currentChallenge = 4;
    } else if (ImGui::Button("Challenge 5")) {
      currentChallenge = 5;
    } else if (ImGui::Button("Challenge 6")) {
      currentChallenge = 6;
    } else if (ImGui::Button("Challenge 7")) {
      currentChallenge = 7;
    } else if (ImGui::Button("Challenge 8 Config")) {
      currentChallenge = 8;
    } else if (ImGui::Button("Challenge 8 Animate")) {
      currentChallenge = 80;
      challenge8iter = 0;
    }  else if (ImGui::Button("Challenge 9")) {
      currentChallenge = 9;
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
        break;
      case 5:
        challenge5(window, targetX, targetY);
        break;
      case 6:
        challenge6(window);
        break;
      case 7:
        challenge7(window);
        break;
      case 8:
        challenge8points = challenge8computation(window);
        break;
      case 9:
        challenge9(window);
        break;
      case 80:

        for(int i = 0; i < challenge8iter; i++)
        {
          point p = challenge8points[i];
          sf::CircleShape ball(2.f);
          ball.setFillColor(sf::Color::Green);
          ball.setPosition(p.x, 720 - p.y);
          window.draw(ball);
        }
        if (!(challenge8Clock.getElapsedTime().asSeconds() <= 0.0001))
        {
          challenge8iter += 50;
          challenge8Clock.restart();
        }

    }

    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}//hello
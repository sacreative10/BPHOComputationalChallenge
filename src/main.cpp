#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
#include <vector>

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_INTRINSICS
#include <algorithm>
#include <execution>
#include <future>
#include <iostream>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

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

const float EPSILON = 0.001;

int RENDER_WIDTH = 1280;
int RENDER_HEIGHT = 720;

const char* maps[] = {"Earth", "Earth - Night", "Mars", "Moon"};
const char* mapsPaths[] = {"earthmapDay.jpg", "earth_nightmap.jpg", "mars.jpg",
                           "moon.jpg"};

struct point {
  float x;
  float y;
};

struct triangle {
  glm::vec3 p1;
  glm::vec3 p2;
  glm::vec3 p3;
};
struct triangle2D {
  glm::vec2 p1;
  glm::vec2 p2;
  glm::vec2 p3;
};

struct modelSettingsStruct {
  glm::vec3 modelPos = glm::vec3(0, 0, 0);
  float rotateAngle = 0.00f;
  float rotateBy = 0.01;
  int depth = 5;
  float radius = 10.0f;
  float scale = 0.1f;
};

struct cameraSettingsStruct {
  glm::vec3 cameraPos = glm::vec3(0, 0, 5);
  glm::vec3 lookingAt = glm::vec3(0, 0, 0);
};

struct launchControlStruct {
  glm::vec2 launchLatLon = glm::vec2(0, 0);
  float launchAngle = 0.0f;  // measured in degrees from the north pole (.i.e.
                             // the positive y axis)
  float elevationAngle = 0.0f;
  float launchVelocity = 0.0f;
  float radius = 0.0f;
  float bigG = 6.67430f * std::pow(10, -10);
  float angularVelocity = 0.0f;
  bool isLaunched = false;
  bool isAnimated = false;
};

std::vector<triangle> octahedronPoints = {
    // top
    triangle{glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)},
    triangle{glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1)},
    triangle{glm::vec3(0, -1, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)},
    triangle{glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)},
    // bottom
    triangle{glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1)},
    triangle{glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)},
    triangle{glm::vec3(0, -1, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, -1)},
    triangle{glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)},
};

// this function will subdivide to a given depth
// this only works for regular, equilateral triangles/surfaces (.i.e. platonic
// solids) like the octahedron
std::vector<triangle> subdivideDepth(std::vector<triangle>& verts, int depth) {
  if (depth == 0) return verts;

  std::vector<triangle> newTriangles;

  for (auto& triangle_ : verts) {
    glm::vec3 p1 = (triangle_.p1 + triangle_.p2) / 2.0f;
    glm::vec3 p2 = (triangle_.p2 + triangle_.p3) / 2.0f;
    glm::vec3 p3 = (triangle_.p3 + triangle_.p1) / 2.0f;

    newTriangles.push_back(triangle{triangle_.p1, p1, p3});
    newTriangles.push_back(triangle{p1, triangle_.p2, p2});
    newTriangles.push_back(triangle{p3, p2, triangle_.p3});
    newTriangles.push_back(triangle{p1, p2, p3});
  }

  return subdivideDepth(newTriangles, depth - 1);
}

glm::vec3 normaliseLength(glm::vec3 a, glm::vec3 b, float length) {
  glm::vec3 dV = b - a;
  float len = glm::length(dV);
  dV *= (length) / len;
  return a + dV;
}

void normaliseTriangleWithRespectToLength(std::vector<triangle>& triangles,
                                          glm::vec3 origin, float length) {
  for (auto& tri : triangles) {
    tri = triangle{normaliseLength(origin, tri.p1, length),
                   normaliseLength(origin, tri.p2, length),
                   normaliseLength(origin, tri.p3, length)};
  }
}

point getUV(glm::vec3& p) {
  glm::vec3 p_ = glm::normalize(p);
  float theta = std::acos(-p_.y);
  float phi = glm::pi<float>() + std::atan2(-p_.z, p_.x);

  auto p__ = point{phi / (2 * glm::pi<float>()), theta / glm::pi<float>()};

  if (p__.x > 1.f - EPSILON)
    p__.x -= EPSILON;
  else if (phi < EPSILON)
    p__.x += EPSILON;

  return p__;
}

float averageDepth(const triangle& tri) {
  return (tri.p1.z + tri.p2.z + tri.p3.z) / 3.0f;
}

struct depthPair {
  float depth;
  int index;
};

void drawPolygons(sf::RenderWindow& window, glm::mat4& mvp,
                  std::vector<triangle>& triangles, sf::Texture* texture) {
  auto computedMesh = triangles;
  for (auto& tri : computedMesh) {
    tri = triangle{mvp * glm::vec4(tri.p1, 1.0f), mvp * glm::vec4(tri.p2, 1.0f),
                   mvp * glm::vec4(tri.p3, 1.0f)};
  }

  std::vector<depthPair> depthPairs = std::vector<depthPair>(triangles.size());
  for (int i = 0; i < triangles.size(); i++) {
    depthPairs[i] = depthPair{averageDepth(computedMesh[i]), i};
  }

  // sort by depth
  std::sort(depthPairs.begin(), depthPairs.end(),
            [](depthPair a, depthPair b) { return a.depth < b.depth; });

  for (int i = 0; i < depthPairs.size(); i++) {
    glm::vec3 p1 = computedMesh[depthPairs[i].index].p1;
    glm::vec3 p2 = computedMesh[depthPairs[i].index].p2;
    glm::vec3 p3 = computedMesh[depthPairs[i].index].p3;

    // translate to screen space
    p1.x = (p1.x + 1.0f) * 0.5f * RENDER_WIDTH;
    p1.y = (-p1.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    p2.x = (p2.x + 1.0f) * 0.5f * RENDER_WIDTH;
    p2.y = (-p2.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    p3.x = (p3.x + 1.0f) * 0.5f * RENDER_WIDTH;
    p3.y = (-p3.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    std::vector<sf::Vertex> verts;
    verts.reserve(4);
    verts.emplace_back(sf::Vector2f(p1.x, p1.y));
    verts.emplace_back(sf::Vector2f(p2.x, p2.y));
    verts.emplace_back(sf::Vector2f(p3.x, p3.y));
    verts.emplace_back(sf::Vector2f(p1.x, p1.y));

    // texture coordinates
    auto tri = triangles[depthPairs[i].index];
    auto uv1 = getUV(tri.p1);
    auto uv2 = getUV(tri.p2);
    auto uv3 = getUV(tri.p3);

    verts[0].texCoords = sf::Vector2f((uv1.x) * (float)texture->getSize().x,
                                      (uv1.y) * (float)texture->getSize().y);
    verts[1].texCoords = sf::Vector2f((uv2.x) * (float)texture->getSize().x,
                                      (uv2.y) * (float)texture->getSize().y);
    verts[2].texCoords = sf::Vector2f((uv3.x) * (float)texture->getSize().x,
                                      (uv3.y) * (float)texture->getSize().y);
    verts[3].texCoords = sf::Vector2f((uv1.x) * (float)texture->getSize().x,
                                      (uv1.y) * (float)texture->getSize().y);

    // Render texture
    window.draw(verts.data(), verts.size(), sf::Triangles, texture);
    // Render wireframe
    // window.draw(verts.data(), verts.size(), sf::Lines);
  };
}

glm::vec3 getCartesian(float lat, float lon, float radius) {
  glm::vec3 point;
  auto phi = glm::radians(-lon);
  auto theta = glm::radians(lat);

  // phi -> longtitude
  // theta -> latitude

  point.x = radius * std::cos(theta) * std::cos(phi);
  point.y = radius * std::sin(theta);
  point.z = radius * std::sin(phi) * std::cos(theta);

  return point;
}

// This function calculates the current spherical coordinates of the projectile,
// And takes away some component with respect to the angular velocity
glm::vec3 adjustforRotation(glm::vec3 currentPos, float angularVel, float dt,
                            int pointIndex) {
  float length = glm::length(currentPos);
  currentPos = glm::normalize(currentPos);
  float theta = std::acos(-currentPos.y) - glm::pi<float>() / 2.f;
  float phi = std::atan2(-currentPos.z, currentPos.x);
  phi -= angularVel * dt * pointIndex;
  return getCartesian(glm::degrees(theta), glm::degrees(phi), 1) * length;
}

void drawPointOnEarth(sf::RenderWindow& window, float lat, float lon,
                      float radius, glm::mat4 mvp) {
  auto point = getCartesian(lat, lon, radius);

  point = mvp * glm::vec4(point, 1.0f);

  point.x = (point.x + 1.0f) * 0.5f * RENDER_WIDTH;
  point.y = (point.y + 1.0f) * 0.5f * RENDER_HEIGHT;

  sf::CircleShape circle(2);
  circle.setPosition(point.x, point.y);
  if (point.z > 4.8) {
    circle.setFillColor(sf::Color::Red);
  } else {
    circle.setFillColor(sf::Color::Magenta);
  }
  window.draw(circle);
}

// assumes v1 is normalised
void createCoordinateSystem(glm::vec3& v1, glm::vec3* v2, glm::vec3* v3) {
  *v2 = glm::vec3(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);

  *v3 = glm::cross(v1, *v2);
}

glm::vec3 workoutVelocityVector(float launchAngle, float elevationAngle,
                                float launchVelocity, glm::vec3 xyzPosition) {
  glm::vec3 v2 = glm::vec3(0, 0, 0);
  glm::vec3 v3 = glm::vec3(0, 0, 0);

  createCoordinateSystem(xyzPosition, &v2, &v3);

  float v1component = launchVelocity * std::sin(glm::radians(elevationAngle));
  float v2component = launchVelocity * std::cos(glm::radians(elevationAngle)) *
                      std::cos(glm::radians(launchAngle));
  float v3component = launchVelocity * std::cos(glm::radians(elevationAngle)) *
                      std::sin(glm::radians(launchAngle));

  glm::vec3 velocityVector =
      (v1component * xyzPosition) + (v2component * v2) + (v3component * v3);

  return velocityVector;
}

std::vector<glm::vec3> getProjectileArc(
    launchControlStruct launchControlSettings) {
  std::vector<glm::vec3> points;
  // get starting location
  glm::vec3 startingLocation = getCartesian(
      launchControlSettings.launchLatLon.x,
      launchControlSettings.launchLatLon.y, launchControlSettings.radius);

  glm::vec3 xyzPosition = startingLocation;

  // Basically, we create a sphere of possibility around the point of the earth
  // think of a las vegas sphere around the point of where we throw the ball

  // x y z components of the velocity vector would be
  glm::vec3 xyzVelocity = workoutVelocityVector(
      launchControlSettings.launchAngle, launchControlSettings.elevationAngle,
      launchControlSettings.launchVelocity, glm::normalize(xyzPosition));

  // planet mass is scaled
  float planetMass = 1.46981 * std::pow(10, 13);

  // in the beginning, the acceleration is just the gravitational acceleration
  float g = launchControlSettings.bigG * planetMass /
            (launchControlSettings.radius * launchControlSettings.radius);

  glm::vec3 difference = glm::normalize(xyzPosition);
  glm::vec3 acceleration = -g * difference;

  int numPoints = 0;
  int maxPoints = 1000;

  float dt = 0.001f;
  while (glm::distance(glm::vec3(0, 0, 0), xyzPosition) >=
             launchControlSettings.radius &&
         numPoints < maxPoints) {
    // update our position
    xyzPosition += xyzVelocity * dt + 0.5f * acceleration * dt * dt;

    // adjust our position based on rotation
    xyzPosition = adjustforRotation(
        xyzPosition, launchControlSettings.angularVelocity, dt, numPoints);

    // update our velocity
    xyzVelocity += acceleration * dt;

    // update our acceleration
    difference = glm::normalize(xyzPosition);

    // Acceleration is calculated by working out which component of the velocity
    // will be affected the most of immediate effect of gravity
    // by calculating the normaised difference between the position and the
    // center of the earth
    float distanceFromCenter = glm::distance(glm::vec3(0, 0, 0), xyzPosition);
    g = launchControlSettings.bigG * planetMass /
        (distanceFromCenter * distanceFromCenter);

    acceleration = -g * difference;

    points.push_back(xyzPosition);
    numPoints++;
  }

  return points;
}

void drawProjectileArc(sf::RenderWindow& window, glm::mat4 mvp,
                       std::vector<glm::vec3>& points) {
  auto copy = points;
  for (auto& point : copy) {
    glm::vec3 transformedPoint = mvp * glm::vec4(point, 1.0f);

    transformedPoint.x = (transformedPoint.x + 1.0f) * 0.5f * RENDER_WIDTH;
    transformedPoint.y = (transformedPoint.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    sf::CircleShape circle(2);
    if (transformedPoint.z > 4.8) {
      circle.setFillColor(sf::Color::Red);
    } else {
      circle.setFillColor(sf::Color::Magenta);
    }
    circle.setPosition(transformedPoint.x, transformedPoint.y);
    window.draw(circle);
  }
  // window.draw(lines);
}

void loadTextureFromName(sf::Texture* tex, int index) {
  std::string filename = mapsPaths[index];
  std::string filepath = "resources/" + filename;

  if (!tex->loadFromFile(filepath)) exit(1);
  tex->setSmooth(true);
  tex->setRepeated(true);
}

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH, RENDER_HEIGHT),
                          "BPHO Computational Challenge", sf::Style::Default,
                          settings);
  window.setFramerateLimit(120);

  ImGui::SFML::Init(window, false);

  ImGuiIO& io = ImGui::GetIO();
  ImGui::CreateContext();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = "resources/imgui.ini";
 io.Fonts->AddFontFromFileTTF("resources/JetBrainsMono-Medium.ttf", 17);
 // io.Fonts->AddFontDefault();
  io.Fonts->Build();
  ImGui::SFML::UpdateFontTexture();

  SetupImGuiStyle(true, 0.5);

  auto cameraSettings = cameraSettingsStruct();
  auto modelSettings = modelSettingsStruct();
  auto launchControlSettings = launchControlStruct();
  launchControlSettings.radius = modelSettings.radius;

  glm::mat4 projectionMatrix = glm::perspective(
      glm::radians(90.0f), (float)RENDER_WIDTH / (float)RENDER_HEIGHT, 0.1f,
      100.0f);

  glm::mat4 viewMatrix = glm::lookAt(
      cameraSettings.cameraPos, cameraSettings.lookingAt, glm::vec3(0, 1, 0));

  glm::mat4 modelMatrix =
      glm::scale(glm::mat4(1.f), glm::vec3(modelSettings.scale)) *
      glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(1, 0, 0)) *
      glm::translate(glm::mat4(1.f), modelSettings.modelPos);

  glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

  sf::Texture texture;
  int mapIndex = 0;
  static const char* currentMap = "Earth";
  loadTextureFromName(&texture, mapIndex);

  auto mesh = subdivideDepth(octahedronPoints, modelSettings.depth);
  normaliseTriangleWithRespectToLength(mesh, glm::vec3{0, 0, 0},
                                       modelSettings.radius);

  auto projectilePath = std::vector<glm::vec3>();
  int currentAnimatedPoint = 0;

  sf::Clock deltaClock;
  sf::Clock animatationClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (sf::Event::KeyPressed == sf::Keyboard::Escape) {
          window.close();
        }
      }
      if (event.type == sf::Event::Resized) {
        RENDER_WIDTH = event.size.width;
        RENDER_HEIGHT = event.size.height;
        projectionMatrix = glm::perspective(
            glm::radians(90.0f), (float)RENDER_WIDTH / (float)RENDER_HEIGHT,
            0.1f, 100.0f);
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Planet Settings");

    if (ImGui::Button("Toggle Rotation")) {
      modelSettings.rotateAngle =
          modelSettings.rotateAngle == 0.0f ? modelSettings.rotateBy : 0.0f;
      launchControlSettings.angularVelocity = modelSettings.rotateAngle;
    }
    ImGui::SetItemTooltip("Don't forget to relaunch the ball!");
    ImGui::SetNextItemWidth(250);
    modelSettings.rotateBy *= 120;
    ImGui::SliderFloat("Rotation Model (rad/s)", &modelSettings.rotateBy, 0.f, 1.2f);
    modelSettings.rotateBy = modelSettings.rotateBy / 120.f;
    ImGuiStyle& style = ImGui::GetStyle();
    float w = ImGui::CalcItemWidth();
    float spacing = style.ItemInnerSpacing.x;
    float button_sz = ImGui::GetFrameHeight();
    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
    if (ImGui::BeginCombo("##custom combo", currentMap,
                          ImGuiComboFlags_NoArrowButton)) {
      for (int n = 0; n < IM_ARRAYSIZE(maps); n++) {
        bool selected = (currentMap == maps[n]);
        if (ImGui::Selectable(maps[n], selected)) {
          currentMap = maps[n];
          mapIndex = n;
          loadTextureFromName(&texture, mapIndex);
        }
        if (selected) ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine(0, spacing);
    if (ImGui::ArrowButton("##r", ImGuiDir_Left)) {
      mapIndex -= 1;
      mapIndex = (mapIndex % IM_ARRAYSIZE(maps) + IM_ARRAYSIZE(maps)) %
                 IM_ARRAYSIZE(maps);
      currentMap = maps[mapIndex];
      loadTextureFromName(&texture, mapIndex);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::ArrowButton("##l", ImGuiDir_Right)) {
      mapIndex += 1;
      mapIndex %= IM_ARRAYSIZE(maps);
      currentMap = maps[mapIndex];
      loadTextureFromName(&texture, mapIndex);
    }
    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    ImGui::Text("Planet Map");

    ImGui::End();


    // Yes, this is an ugly way of doing it
    // And no, I will not be listening to any alternative approaches
    ImGui::Begin("Launch Control");
    ImGui::SliderFloat("Latitude", &launchControlSettings.launchLatLon.x, -90,
                       90, reinterpret_cast<const char*>(u8"%.4f\xb0"));
    ImGui::SetItemTooltip(
        "The angle made between the equator and the poles [-90, 90]");
    ImGui::SliderFloat("Longtitude", &launchControlSettings.launchLatLon.y,
                       -180, 180, reinterpret_cast<const char*>(u8"%.4f\xb0"));
    ImGui::SetItemTooltip(
        "The angle from the prime meridian, measuring east-ness or west-ness [-180, 180]");

    ImGui::SliderFloat("Launch Velocity", &launchControlSettings.launchVelocity,
                       0.0f, 100.0f, reinterpret_cast<const char*>("%.4f m/s"));
    ImGui::SetItemTooltip(
        "Velocity to throw the ball at");

    ImGui::SliderFloat("Launch Angle", &launchControlSettings.launchAngle, 0.0f,
                       360.0f, reinterpret_cast<const char*>(u8"%.4f\xb0"));
    ImGui::SetItemTooltip(
        "The angle made from the west axis downward to the compass direction you want to throw the ball to");
    ImGui::SliderFloat("Elevation Angle", &launchControlSettings.elevationAngle,
                       0.0f, 90.0f, reinterpret_cast<const char*>(u8"%.4f\xb0"));
    ImGui::SetItemTooltip(
        "The angle from the ground you want to throw the ball to");

    ImGui::Text("Number of Points %d", projectilePath.size());

    if (ImGui::Button("Launch!")) {
      launchControlSettings.isLaunched = true;
      currentAnimatedPoint = 0;
    }
    ImGui::SameLine(0, 5);
    if (ImGui::Button("Animate"))
    {
      projectilePath = getProjectileArc(launchControlSettings);
      launchControlSettings.isAnimated = true;
      animatationClock.restart();
    }
    ImGui::End();

    modelMatrix =
        glm::rotate(modelMatrix, modelSettings.rotateAngle, glm::vec3(0, 1, 0));

    mvp = projectionMatrix * viewMatrix * modelMatrix;

    window.clear();

    drawPolygons(window, mvp, mesh, &texture);
    drawPointOnEarth(window, launchControlSettings.launchLatLon.x,
                     launchControlSettings.launchLatLon.y, modelSettings.radius,
                     mvp);

    if (launchControlSettings.isLaunched) {
      projectilePath = getProjectileArc(launchControlSettings);
      launchControlSettings.isLaunched = false;
    }
    if (launchControlSettings.isAnimated)
    {
      float timePerPoint = 5.f / projectilePath.size();
      if (animatationClock.getElapsedTime().asSeconds() >= timePerPoint)
      {
        currentAnimatedPoint++;
        if (currentAnimatedPoint >= projectilePath.size()) launchControlSettings.isAnimated = false;
        animatationClock.restart();
      }
      for (int i = 0; i < currentAnimatedPoint; i++)
      {
		glm::vec3 transformedPoint = mvp * glm::vec4(projectilePath[i], 1.0f);

		transformedPoint.x = (transformedPoint.x + 1.0f) * 0.5f * RENDER_WIDTH;
		transformedPoint.y = (transformedPoint.y + 1.0f) * 0.5f * RENDER_HEIGHT;

		sf::CircleShape circle(2);
		if (transformedPoint.z > 4.8) {
		  circle.setFillColor(sf::Color::Red);
		} else {
		  circle.setFillColor(sf::Color::Magenta);
		}
		circle.setPosition(transformedPoint.x, transformedPoint.y);
		window.draw(circle);
      }
      
    } else {
		drawProjectileArc(window, mvp, projectilePath);
    }
    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  texture.~Texture();

  return 0;
}

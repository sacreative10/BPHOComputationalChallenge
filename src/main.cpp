#include <imgui-SFML.h>
#include <imgui.h>
#include <math.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_INTRINSICS
#include <algorithm>
#include <execution>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

#define X_AXIS_OFFSET 20
#define Y_AXIS_OFFSET 20

// TODO: FIX THE EARTHMAP TEXTURE FINDING

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

int RENDER_WIDTH = 1280;
int RENDER_HEIGHT = 720;

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
// this only works for non-degenerate triangles/surfaces (.i.e. platonic solids)
// like the octahedron
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

glm::vec3 normaliseLength(glm::vec3 a, glm::vec3 b, float length, float t = 0) {
  glm::vec3 dV = b - a;
  float len = glm::length(dV);
  dV *= (len + ((length - len) * t)) / len;
  return a + dV;
}

void normaliseWithRespectToLength(std::vector<triangle>& triangles,
                                  glm::vec3 origin, float length, float t) {
  for (auto& tri : triangles) {
    tri = triangle{normaliseLength(origin, tri.p1, length, t),
                   normaliseLength(origin, tri.p2, length, t),
                   normaliseLength(origin, tri.p3, length, t)};
  }
}

point getUV(glm::vec3& p) {
  glm::vec3 p_ = glm::normalize(p);
  float theta = std::acos(-p_.y);
  float phi = glm::pi<float>() + std::atan2(-p_.z, p_.x);

  return point{phi / (2 * glm::pi<float>()), theta / glm::pi<float>()};
}

// float averageDepth(const glm::mat4& mvp, const triangle& tri) {
//   glm::vec4 p1 = mvp * glm::vec4(tri.p1, 1.0f);
//   glm::vec4 p2 = mvp * glm::vec4(tri.p2, 1.0f);
//   glm::vec4 p3 = mvp * glm::vec4(tri.p3, 1.0f);
//   return (p1.z + p2.z + p3.z) / 3.0f;
// }

float averageDepth(const triangle& tri) {
  return (tri.p1.z + tri.p2.z + tri.p3.z) / 3.0f;
}

struct depthPair {
  float depth;
  int index;
};

void drawPolygon(sf::RenderWindow& window, glm::mat4& mvp,
                 std::vector<triangle>& triangles, sf::Texture texture) {
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

    // p1 /= p1.z;
    // p2 /= p2.z;
    // p3 /= p3.z;

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

    verts[0].texCoords = sf::Vector2f((uv1.x) * texture.getSize().x,
                                      (uv1.y) * texture.getSize().y);
    verts[1].texCoords = sf::Vector2f((uv2.x) * texture.getSize().x,
                                      (uv2.y) * texture.getSize().y);
    verts[2].texCoords = sf::Vector2f((uv3.x) * texture.getSize().x,
                                      (uv3.y) * texture.getSize().y);

    window.draw(verts.data(), verts.size(), sf::Triangles, &texture);
  };
}

struct modelSettingsStruct {
  glm::vec3 modelPos = glm::vec3(0, 0, 0);
  float rotateAngle = 0.00f;
  int depth = 5;
  float radius = 10.0f;
  float scale = 0.1f;
};

struct cameraSettingsStruct {
  glm::vec3 cameraPos = glm::vec3(0, 0, 5);
  glm::vec3 lookingAt = glm::vec3(0, 0, 0);
};

void drawPointOnEarth(sf::RenderWindow& window, float lat, float lon,
                      float radius, glm::mat4 mvp) {
  glm::vec3 point;
  auto phi = glm::radians(-lon);
  auto theta = glm::radians(lat);

  // phi -> longitude
  // theta -> latitude

  point.x = radius * std::cos(theta) * std::cos(phi);
  point.y = radius * std::sin(theta);
  point.z = radius * std::cos(theta) * std::sin(phi);

  point = mvp * glm::vec4(point, 1.0f);

  point.x = (point.x + 1.0f) * 0.5f * RENDER_WIDTH;
  point.y = (point.y + 1.0f) * 0.5f * RENDER_HEIGHT;

  sf::CircleShape circle(2);
  circle.setPosition(point.x, point.y);
  circle.setFillColor(sf::Color::Red);
  window.draw(circle);
}

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  settings.depthBits = 24;
  sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH, RENDER_HEIGHT),
                          "BPHO Computational Challenge", sf::Style::Default,
                          settings);
  window.setFramerateLimit(120);
  ImGui::SFML::Init(window);

  auto cameraSettings = cameraSettingsStruct();
  auto modelSettings = modelSettingsStruct();

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
  texture.loadFromFile("../../earthmap.jpg");

  auto mesh = subdivideDepth(octahedronPoints, modelSettings.depth);
  normaliseWithRespectToLength(mesh, glm::vec3{0, 0, 0}, modelSettings.radius,
                               1.f);

  float lat = -28.37;
  float lon = 77;

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClearDepth(1.f);
  glDepthFunc(GL_LESS);
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
      if (event.type == sf::Event::KeyPressed) {
        if (event.KeyPressed == sf::Keyboard::Escape) {
          window.close();
        }
      }
      if (event.type == sf::Event::Resized) {
        RENDER_WIDTH = event.size.width;
        RENDER_HEIGHT = event.size.height;
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Camera Controls");
    ImGui::SliderFloat3("Camera Pos", &cameraSettings.cameraPos.x, -100.0f,
                        100.0f);
    ImGui::SliderFloat3("Looking At", &cameraSettings.lookingAt.x, -100.0f,
                        100.0f);
    ImGui::End();

    ImGui::Begin("Earth Controls");
    if (ImGui::Button("Toggle Rotation")) {
      modelSettings.rotateAngle =
          modelSettings.rotateAngle == 0.0f ? 0.01f : 0.0f;
    }
    ImGui::End();

    ImGui::Begin("Launch Control");
    ImGui::SliderFloat("Latitude", &lat, -90, 90);
    ImGui::SliderFloat("Longtitude", &lon, -180, 180);

    ImGui::End();

    projectionMatrix = glm::perspective(
        glm::radians(90.0f), (float)RENDER_WIDTH / (float)RENDER_HEIGHT, 0.1f,
        100.0f);

    viewMatrix = glm::lookAt(cameraSettings.cameraPos, cameraSettings.lookingAt,
                             glm::vec3(0, 1, 0));

    modelMatrix =
        glm::rotate(modelMatrix, modelSettings.rotateAngle, glm::vec3(0, 1, 0));

    mvp = projectionMatrix * viewMatrix * modelMatrix;

    window.clear();

    drawPolygon(window, mvp, mesh, texture);
    drawPointOnEarth(window, lat, lon, modelSettings.radius, mvp);
    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  texture.~Texture();

  return 0;
}

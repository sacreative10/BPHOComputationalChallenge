#include <imgui-SFML.h>
#include <imgui.h>
#include <math.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#define GLM_SWIZZLE
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

#define X_AXIS_OFFSET 20
#define Y_AXIS_OFFSET 20

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

float cross2D(glm::vec2 a, glm::vec2 b) { return a.x * b.y - a.y * b.x; }

// true if ccw, false if cw
bool determineWindingOrder(triangle2D& tri) {
  float cross = cross2D(tri.p2 - tri.p1, tri.p3 - tri.p1);
  if (cross > 0) return false;
  return true;
}

void drawPolygon(sf::RenderWindow& window, glm::mat4& mvp,
                 std::vector<triangle>& triangles, sf::Texture texture) {
  for (auto& triangle : triangles) {
    // check if winding order is correct
    // if not, we skip the triangle

    glm::vec3 p1 = mvp * glm::vec4(triangle.p1, 1.0f);
    glm::vec3 p2 = mvp * glm::vec4(triangle.p2, 1.0f);
    glm::vec3 p3 = mvp * glm::vec4(triangle.p3, 1.0f);

    // translate to screen space
    p1.x = (p1.x + 1.0f) * 0.5f * RENDER_WIDTH;
    p1.y = (p1.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    p2.x = (p2.x + 1.0f) * 0.5f * RENDER_WIDTH;
    p2.y = (p2.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    p3.x = (p3.x + 1.0f) * 0.5f * RENDER_WIDTH;
    p3.y = (p3.y + 1.0f) * 0.5f * RENDER_HEIGHT;

    triangle2D tri2D = triangle2D{glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y),
                                  glm::vec2(p3.x, p3.y)};

    std::vector<sf::Vertex> verts;
    verts.push_back(sf::Vertex(sf::Vector2f(p1.x, p1.y)));
    verts.push_back(sf::Vertex(sf::Vector2f(p2.x, p2.y)));
    verts.push_back(sf::Vertex(sf::Vector2f(p3.x, p3.y)));
    verts.push_back(sf::Vertex(sf::Vector2f(p1.x, p1.y)));

    // texture coordinates
    point uv1 = getUV(triangle.p1);
    point uv2 = getUV(triangle.p2);
    point uv3 = getUV(triangle.p3);

    verts[0].texCoords =
        sf::Vector2f(uv1.x * texture.getSize().x, uv1.y * texture.getSize().y);
    verts[1].texCoords =
        sf::Vector2f(uv2.x * texture.getSize().x, uv2.y * texture.getSize().y);
    verts[2].texCoords =
        sf::Vector2f(uv3.x * texture.getSize().x, uv3.y * texture.getSize().y);

    window.draw(verts.data(), verts.size(), sf::Triangles, &texture);
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH, RENDER_HEIGHT),
                          "BPHO Computational Challenge");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  glm::mat4 projectionMatrix = glm::perspective(
      glm::radians(90.0f), (float)RENDER_WIDTH / (float)RENDER_HEIGHT, 0.1f,
      100.0f);

  glm::vec3 cameraPos = glm::vec3(0, 0, 5);
  glm::vec3 lookingAt = glm::vec3(0, 0, 0);

  glm::mat4 viewMatrix = glm::lookAt(cameraPos, lookingAt, glm::vec3(0, 1, 0));

  float rotateAngle = 0.0f;
  int depth = 0;
  float t = 0.0f;
  float length = 5.0f;
  float scale = 0.5f;

  glm::mat4 modelMatrix = glm::mat4(1.0f);
  // scale the model

  glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

  sf::Clock deltaClock;

  sf::Texture texture;
  texture.loadFromFile("../../earthmap.jpg");

  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);
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
    ImGui::SliderFloat3("Camera Pos", &cameraPos.x, -100.0f, 100.0f);
    ImGui::SliderFloat3("Looking At", &lookingAt.x, -100.0f, 100.0f);
    ImGui::End();

    ImGui::Begin("Model Controls");
    ImGui::SliderFloat("Rotate Angle", &rotateAngle, 0.0f, 360.0f);
    ImGui::SliderInt("Depth", &depth, 0, 10);
    if (ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f))
      modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale));
    ImGui::SliderFloat("T", &t, 0.0f, 1.0f);
    ImGui::SliderFloat("Length", &length, 0.0f, 10.0f);

    ImGui::End();

    projectionMatrix = glm::perspective(
        glm::radians(90.0f), (float)RENDER_WIDTH / (float)RENDER_HEIGHT, 0.1f,
        100.0f);
    viewMatrix = glm::lookAt(cameraPos, lookingAt, glm::vec3(0, 1, 0));

    modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));

    mvp = projectionMatrix * viewMatrix * modelMatrix;

    window.clear();

    ImGui::SFML::Render(window);
    auto subdivided = subdivideDepth(octahedronPoints, depth);
    normaliseWithRespectToLength(subdivided, glm::vec3(0, 0, 0), length, t);

    drawPolygon(window, mvp, subdivided, texture);

    // window.draw(shape);

    window.display();
  }

  ImGui::SFML::Shutdown();

  texture.~Texture();

  return 0;
}
#pragma once

#include <string>
#include <unordered_map>
#include <gl/glew.h>
#include <glm/vec2.hpp>

#include "../lightweightutils/identifiable.hpp"

class Font : public Identifiable<unsigned int> {
  private:
    struct Character {
      unsigned int TextureID; // ID handle of the glyph texture
      glm::ivec2   Size;      // Size of glyph
      glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
      unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
    std::string fontName;
    std::string fontPath;
    //std::unordered_map<GLchar, Character> characterMap;
  public:
    Font(std::string fontPath);
};

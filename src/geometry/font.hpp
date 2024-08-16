#pragma once

#include <string>
#include <unordered_map>
#include <gl/glew.h>
#include <glm/vec2.hpp>

#include "../lightweightutils/identifiable.hpp"

/**
 * A class representing a font.
 */
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
    /**
     * Constructs a font given a fontpath.
     * @param fontPath    path of the font inside the filesystem
     */
    Font(std::string fontPath);
};

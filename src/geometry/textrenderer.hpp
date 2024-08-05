#pragma once

#include <string>
#include <unordered_map>
#include <gl/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "mesh.hpp"
#include "label.hpp"
#include "../lightweightutils/idprovider.hpp"
#include "../lightweightutils/identifiable.hpp"

class TextRenderer {
  private:
    struct Character {
      unsigned int TextureID; // ID handle of the glyph texture
      glm::ivec2   Size;      // Size of glyph
      glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
      unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
    IDProvider idProvider;
    unsigned int guiShaderId;
    unsigned int vao;
    unsigned int vbo;
    std::unordered_map<GLchar, Character> Characters;
    FT_Library freeTypeLib;
    FT_Face fontFace;
  public:
    TextRenderer(unsigned int guiShaderId, std::string fontName, std::string fontPath);
    void renderText(std::list<std::shared_ptr<Label>> textToRender, glm::mat4& world);
};

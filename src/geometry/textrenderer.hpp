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

/**
 * A class that represents a text renderer.
 */
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
    /**
     * Construct a textrenderer.
     * @param guiShaderId     the shader's identifier to be used during the rendering of text
     * @param fontName        the font to be used to render the text
     * @param fontPath        a string containing the path of the font file in the filesystem
     */
    TextRenderer(unsigned int guiShaderId, std::string fontName, std::string fontPath);
    /**
     * Renders the text inside a given list.
     * @param textToRender    a list of labels to be rendered
     * @param world           world matrix to be used during the rendering
     */
    void renderText(std::list<std::shared_ptr<Label>> textToRender, glm::mat4& world);
};

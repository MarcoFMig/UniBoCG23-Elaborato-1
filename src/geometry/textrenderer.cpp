#include "textrenderer.hpp"

#include <memory>
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "../exceptions/initializationexception.hpp"

TextRenderer::TextRenderer(unsigned int guiShaderId, std::string fontName, std::string fontPath) {
  this->guiShaderId = guiShaderId;
  glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	FT_Error operationCode = FT_Init_FreeType(&this->freeTypeLib);
  if (operationCode) {
    throw InitializationException("Error while initializing module FreeType - init");
  }
  operationCode = FT_New_Face(freeTypeLib, fontPath.c_str(), 0, &this->fontFace);
  if (operationCode) {
    throw InitializationException("Error while initializing module Freetype - font");
  }
	FT_Set_Pixel_Sizes(this->fontFace, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++) {
	  // Load character glyph 
		if (FT_Load_Char(this->fontFace, c, FT_LOAD_RENDER)) {
      printf("ERROR::FREETYTPE: Failed to load Glyph \n");
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			this->fontFace->glyph->bitmap.width,
			this->fontFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			this->fontFace->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(this->fontFace->glyph->bitmap.width, this->fontFace->glyph->bitmap.rows),
			glm::ivec2(this->fontFace->glyph->bitmap_left, this->fontFace->glyph->bitmap_top),
			static_cast<unsigned int>(this->fontFace->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
  FT_Done_Face(this->fontFace);
	FT_Done_FreeType(freeTypeLib);
}

void TextRenderer::renderText(std::list<std::shared_ptr<Label>> textToRender, glm::mat4& world) {
	for (std::shared_ptr<Label> label : textToRender) {
		// activate corresponding render state	
    //printf("USing program %i\n", this->guiShaderId);
		glUseProgram(this->guiShaderId);
		glUniform3f(glGetUniformLocation(this->guiShaderId, "textColor"), label->getColor().r, label->getColor().g, label->getColor().b);
		glUniformMatrix4fv(glGetUniformLocation(this->guiShaderId, "Projection_text"), 1, GL_FALSE, glm::value_ptr(world));

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(this->vao);

		// iterate through all characters
		std::string::const_iterator c;
		unsigned int tries = 0;
		float tmpX = label->getPosition().x;
		for (char& character : label->getText())
		{
			//printf("Printing character: %c %d\n", *c, c != label->getText().end());
			Character ch = Characters[character];

			float xpos = tmpX + ch.Bearing.x * label->getScale().x;
			float ypos = label->getPosition().y - (ch.Size.y - ch.Bearing.y) * label->getScale().y;

			float w = ch.Size.x * label->getScale().x;
			float h = ch.Size.y * label->getScale().y;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			tmpX += (ch.Advance >> 6)* label->getScale().x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

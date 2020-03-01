#include "fps_counter.hpp"

#include <string>
#include <iostream>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.hpp"
#include "shaders.hpp"


struct Character {
    GLuint     textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint     advance;
};

std::map<GLchar, Character> Characters;

void LoadCharacters()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    // FIXME: dont hard code the font
    if (FT_New_Face(ft, "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", 0, &face)) {
        std::cout << "Failed to load font" << std::endl;  
    }

    FT_Set_Pixel_Sizes(face, 0, 40);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
    for (GLubyte c = '0'; c <= '9'; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Failed to load Glyph for character: " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void RenderText(GLuint program, GLuint vertex_array, GLuint vertex_buffer,
                std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertex_array);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        GLfloat vertices[] = {
            xpos,     ypos + h,  0.0, 0.0,
            xpos,     ypos,      0.0, 1.0,
            xpos + w, ypos,      1.0, 1.0,
            xpos,     ypos + h,  0.0, 0.0,
            xpos + w, ypos,      1.0, 1.0,
            xpos + w, ypos + h,  1.0, 0.0
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FpsCounter::FpsCounter(uint window_width, uint window_height) :
        current_fps_(0.0), last_update_time_(glfwGetTime()),
        window_width_(window_width), window_height_(window_height),
        shader_program_(CreateProgram(shaders::FontVertexSource, shaders::FontFragmentSource))
{
    if (Characters.empty())
        LoadCharacters();

    glUseProgram(shader_program_);
    glm::mat4 projection = glm::ortho(0.0f, (float)window_width, 0.0f, (float)window_height);
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "projection"),
                       1, GL_FALSE, &projection[0][0]);

    // Assumes GL_BLEND is not disabled anywhere else
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &vertex_buffer_);
    glBindVertexArray(vertex_array_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);      
}

FpsCounter::~FpsCounter()
{
    glDeleteProgram(shader_program_);
    glDeleteVertexArrays(1, &vertex_array_);
    glDeleteBuffers(1, &vertex_buffer_);
}

void FpsCounter::update()
{
    const double weight = 0.1;

    double current_time = glfwGetTime();
    current_fps_ = weight / (current_time - last_update_time_) + current_fps_ * (1.0 - weight);
    last_update_time_ = current_time;
}

int FpsCounter::currentFps() const
{
    return current_fps_;
}

void FpsCounter::draw() const
{
    std::string fps_string = std::to_string((int)std::round(current_fps_));
    RenderText(shader_program_, vertex_array_, vertex_buffer_, fps_string,
               5.0f, window_height_ - 45.0f, 1.0f, {0.0f, 1.0f, 0.0f});
}


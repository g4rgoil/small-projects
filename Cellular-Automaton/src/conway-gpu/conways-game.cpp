#include "conways-game.hpp"

#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <SFML/Graphics.hpp>
// #include <SFML/OpenGL.hpp>
#include <GL/glew.h>

using namespace conway_gpu;


const std::string vertex_shader_source =
    "uniform vec2 gridSize;                                         \n"
    "                                                               \n"
    "void main()                                                    \n"
    "{                                                              \n"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;    \n"
    "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0 / \n"
    "        vec4(gridSize, 0.0, 0.0);                              \n"
    "}                                                              \n";

const std::string fragment_shader_source = 
    "uniform sampler2D grid;                                                \n"
    "                                                                       \n"
    "void main() {                                                          \n"
    "    gl_FragColor = vec4(texture2D(grid, gl_TexCoord[0].xy).x * 255.0); \n"
    "}                                                                      \n";


bool CreateRandomTexture(size_t size_x, size_t size_y, sf::Texture *texture)
{
    sf::Image image;
    image.create(size_x, size_y);

    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> distribution(0, 1);

    for (int x = 0; x < size_x; ++x) {
        for (int y = 0; y < size_y; ++y) {
            image.setPixel(x, y, sf::Color(distribution(random_engine), 0, 0));
        }
    }

    return texture->loadFromImage(image);
}

bool CreateUpdateShader(GLuint *program)
{
    /*
    *program = glCreateProgram();
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

    std::ifstream stream("/home/pascal/repositories/small-projects/Cellular-Automaton/shaders/update.glsl");
    std::string source((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    const char *c_source = source.c_str();

    glShaderSource(shader, 1, &c_source, NULL);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        std::string log(errorLog.begin(), errorLog.end());
        std::cout << log << std::endl;
        glDeleteShader(shader); // Don't leak the shader.k

        return false;
    }

    glAttachShader(*program, shader);
    */
    return true;
}

ConwaysGame::ConwaysGame(size_t size_x, size_t size_y)
    : size_x_(size_x), size_y_(size_y),
      current_texture_(textures_), alternate_texture_(textures_ + 1)
{
    for (int i = 0; i < 2; ++i) {
        textures_[i].create(size_x, size_y);
    }

    if (!CreateRandomTexture(size_x_, size_y_, current_texture_)) {
        exit(EXIT_FAILURE);
    }

    if (!sf::Shader::isAvailable()) {
        exit(EXIT_FAILURE);
    }

    draw_shader_.loadFromMemory(vertex_shader_source, fragment_shader_source);
    draw_shader_.setUniform("gridSize", sf::Vector2f(size_x_, size_y));

    if (!CreateUpdateShader(&update_program_)) {
        exit(1);
    }
}

sf::Vector2<size_t> ConwaysGame::getSize() const
{
    return {size_x_, size_y_};
}

void ConwaysGame::draw(sf::RenderTarget *target, const sf::Drawable &drawable)
{
    draw_shader_.setUniform("grid", *current_texture_);
    target->draw(drawable, &draw_shader_);
}

void ConwaysGame::nextGeneration()
{
    /*
    GLint source_grid = glGetUniformLocation(update_program_, "sourceGrid");
    GLint target_grid = glGetUniformLocation(update_program_, "targetGrid");
    glUseProgram(update_program_);

    glUniform1i(source_grid, 0);
    glUniform1i(target_grid, 1);

    // glActiveTexture(GL_TEXTURE0 + 0);
    glBindImageTexture(0, current_texture_->getNativeHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32UI);
    glBindImageTexture(1, alternate_texture_->getNativeHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

    // glDispatchCompute(size_x_/32, size_y_, 1);
    std::swap(current_texture_, alternate_texture_);
    */
}
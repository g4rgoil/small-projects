#ifndef SHADERS_HPP_INCLUDED
#define SHADERS_HPP_INCLUDED

#include <string>

namespace shaders {

const std::string ConwayVertexSource = 
    "#version 330 core                                                  \n"
    "                                                                   \n"
    "layout (location = 0) in vec2 position;                            \n"
    "layout (location = 1) in vec2 texCoord_in;                         \n"
    "                                                                   \n"
    "out vec2 texCoord;                                                 \n"
    "                                                                   \n"
    "uniform mat4 projection;                                           \n"
    "uniform mat4 texProjection;                                        \n"
    "                                                                   \n"
    "void main() {                                                      \n"
    "    gl_Position = projection * vec4(position, 0.0, 1.0);           \n"
    "    texCoord = vec2(texProjection * vec4(texCoord_in, 0.0, 1.0));  \n"
    "}                                                                  ";

const std::string ConwayFragmentSource =
    "#version 330 core                                                \n"
    "                                                                 \n"
    "in vec2 texCoord;                                                \n"
    "                                                                 \n"
    "out vec4 fragColor;                                              \n"
    "                                                                 \n"
    "uniform isampler2D grid;                                         \n"
    "                                                                 \n"
    "void main() {                                                    \n"
    "    fragColor = vec4(vec3(1 - texture(grid, texCoord).r), 1.0);  \n"
    "}                                                                ";


const std::string FontVertexSource =
    "#version 330 core                                          \n"
    "layout (location = 0) in vec4 vertex;                      \n"
    "out vec2 TexCoords;                                        \n"
    "                                                           \n"
    "uniform mat4 projection;                                   \n"
    "                                                           \n"
    "void main()                                                \n"
    "{                                                          \n"
    "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);  \n"
    "    TexCoords = vertex.zw;                                 \n"
    "}                                                          ";

const std::string FontFragmentSource =
    "#version 330 core                                                    \n"
    "in vec2 TexCoords;                                                   \n"
    "out vec4 color;                                                      \n"
    "                                                                     \n"
    "uniform sampler2D text;                                              \n"
    "uniform vec3 textColor;                                              \n"
    "                                                                     \n"
    "void main()                                                          \n"
    "{                                                                    \n"
    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);  \n"
    "    color = vec4(textColor, 1.0) * sampled;                          \n"
    "}                                                                    ";

};

#endif  // SHADERS_HPP_INCLUDED
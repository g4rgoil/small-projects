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


const std::string ConwayUpdateSource = 
    "#version 430 core                                                                   \n"
    "                                                                                    \n"
    "layout(local_size_x = 16, local_size_y = 16) in;                                    \n"
    "                                                                                    \n"
    "layout(r8i, binding = 0) uniform readonly iimage2D sourceGrid;                      \n"
    "layout(r8i, binding = 1) uniform writeonly iimage2D targetGrid;                     \n"
    "                                                                                    \n"
    "void countNeighbours(const in ivec2 coordinate, out int neighbours) {               \n"
    "    for (int dx = -1; dx <= 1; ++dx) {                                              \n"
    "        for (int dy = -1; dy <= 1; ++dy) {                                          \n"
    "            if (dx != 0 || dy != 0) {                                               \n"
    "                neighbours += imageLoad(sourceGrid, coordinate + ivec2(dx, dy)).x;  \n"
    "            }                                                                       \n"
    "        }                                                                           \n"
    "    }                                                                               \n"
    "}                                                                                   \n"
    "                                                                                    \n"
    "void writeNewState(const in ivec2 coordinate, const in int neighbours) {            \n"
    "    if (neighbours == 3) {                                                          \n"
    "        imageStore(targetGrid, coordinate, ivec4(1));                               \n"
    "    } else if (neighbours == 2 && imageLoad(sourceGrid, coordinate).x == 1) {       \n"
    "        imageStore(targetGrid, coordinate, ivec4(1));                               \n"
    "    } else {                                                                        \n"
    "        imageStore(targetGrid, coordinate, ivec4(0));                               \n"
    "    }                                                                               \n"
    "}                                                                                   \n"
    "                                                                                    \n"
    "void main() {                                                                       \n"
    "    ivec2 coordinate = ivec2(gl_GlobalInvocationID.xy);                             \n"
    "                                                                                    \n"
    "    int neighbours = 0;                                                             \n"
    "    countNeighbours(coordinate, neighbours);                                        \n"
    "    writeNewState(coordinate, neighbours);                                          \n"
    "}                                                                                   ";



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
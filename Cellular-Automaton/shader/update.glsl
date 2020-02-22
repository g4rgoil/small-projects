#version 430 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(r8i, binding = 0) uniform readonly iimage2D sourceGrid;
layout(r8i, binding = 1) uniform writeonly iimage2D targetGrid;

void main() {
    ivec2 coordinate = ivec2(gl_GlobalInvocationID.xy);

    // TODO add guard is exceeding image
    int neighbours = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx != 0 || dy != 0) {
                neighbours += imageLoad(sourceGrid, coordinate + ivec2(dx, dy)).x;
            }
        }
    }

    if (neighbours == 3) {
        imageStore(targetGrid, coordinate, ivec4(1));
    } else if (neighbours == 2 && imageLoad(sourceGrid, coordinate).x == 1) {
        imageStore(targetGrid, coordinate, ivec4(1));
    } else {
        imageStore(targetGrid, coordinate, ivec4(0));
    }
}
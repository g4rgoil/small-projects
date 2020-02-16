#version 430

precision mediump uimage2D;

layout(rgba32ui) uniform readonly uimage2D sourceGrid;
layout(rgba32ui) uniform writeonly uimage2D targetGrid;

// layout(local_size_x = 16, local_size_y = 16​) in;
void main() {
    imageStore(targetGrid, ivec2(gl_GlobalInvocationID.xy), uvec4(0, 0, 0, 0));
    /*
    if (gl_GlobalInvocationID.xy < imageSize(sourceGrid)) {
        int neighbours = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx != 0 || dy != 0) {
                    neighbours += imageLoad(sourceGrid, gl_GlobalInvocationID.xy + ivec2(dx, dy)).x;
                }
            }
        }

        if (neighbours == 3) {
            imageStore(targetGrid, gl_GlobalInvocationID.xy, vec4(1.0, 0.0, 0.0, 0.0));
        } else if (neighbours == 2 && imageLoad(sourceGrid, gl_GlobalInvocationID.xy)) {
            imageStore(targetGrid, gl_GlobalInvocationID.xy, vec4(1.0, 0.0, 0.0, 0.0));
        } else {
            imageStore(targetGrid, gl_GlobalInvocationID.xy, vec4(0.0, 0.0, 0.0, 0.0));
        }
    }
    */
}
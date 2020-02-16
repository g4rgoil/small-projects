uniform vec2 gridSize;
uniform sampler2D grid;

void main() {
    // vec2 gridCoord = gl_TexCoord[0].xy / gridSize;
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);

    /*
    float neighbours = 0.0;

    for (float x = -1.0; x <= 1.0; ++x) {
        for (float y = -1.0; y <= 1.0; ++y) {
            if (x != 0.0 || y != 0.0) {
                neighbours += texture2D(grid, (gridCoord + vec2(x, y) / gridSize)).x;
            }
        }
    }

    if (neighbours == 3.0) {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
    } else if (neighbours == 2.0 && texture2D(grid, gridCoord / gridSize).x == 1.0) {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
    } else {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    */
}
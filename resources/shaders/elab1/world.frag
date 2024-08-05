#version 330 core

uniform float time;
uniform vec2 resolution;
uniform bool isBackground;
in vec4 outColor;
out vec4 FragColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);

    float res = mix(
        mix(rand(ip), rand(ip + vec2(1.0, 0.0)), u.x),
        mix(rand(ip + vec2(0.0, 1.0)), rand(ip + vec2(1.0, 1.0)), u.x),
        u.y);
    return res*res;
}

void main() {
    vec2 p = gl_FragCoord.xy / resolution;
    if (isBackground) {
        if (p.y > 0.3) { // Spostato il confine per dare più spazio all'erba
            float x = p.x + time * 0.1;
            float n = noise(vec2(x * 10.0, p.y * 10.0));
            vec3 skyColor = vec3(0.5, 0.7, 0.9);
            vec3 cloudColor = vec3(0.9, 0.9, 0.95);
            vec3 finalColor = mix(skyColor, cloudColor, n);
            FragColor = vec4(finalColor, 1.0);
        } else if (p.y > 0.25) { // Zona d'erba
            float n = noise(vec2(p.x * 50.0, p.y * 50.0)); // Rumore più fine per l'erba
            vec3 grassColorBase = vec3(0.25, 0.5, 0.2); // Verde erba
            vec3 grassDetail = vec3(0.2, 0.45, 0.15); // Variazioni di colore
            vec3 finalGrassColor = mix(grassColorBase, grassDetail, n);
            FragColor = vec4(finalGrassColor, 1.0);
        } else { // Terreno
            float n = noise(vec2(p.x * 25.0, p.y * 25.0));
            vec3 groundBaseColor = vec3(0.3, 0.2, 0.1);
            vec3 groundDetailColor = vec3(0.4, 0.3, 0.2);
            vec3 finalGroundColor = mix(groundBaseColor, groundDetailColor, n);
            FragColor = vec4(finalGroundColor, 1.0);
        }
    } else {
        FragColor = outColor;
    }
}

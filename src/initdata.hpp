#pragma once

#define DEFAULT_WINDOW_WIDTH        1280
#define DEFAULT_WINDOW_HEIGHT       720
#define DEFAULT_FONT_LOCATION       "./fonts/eurostile.ttf"
#define VERTEX_SHADER_NAME          "basic_vertex.vert"
#define FRAGMENT_SHADER_NAME        "basic_fragment.frag"
#define VERTEX_TEXT_SHADER_NAME     "text.vert"
#define FRAGMENT_TEXT_SHADER_NAME   "text.frag"
#define BASIC_VERTEX_SHADER_CODE    "\
#version 330 core\n\
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0\n\
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1\n\
out vec4 outColor; // output a color to the fragment shader\n\
uniform mat4 Projection;  //vARIABILE DI TIPO uniform, rimane la stessa per ogni vertice della primitiva e viene passata dall'esterno\n\
uniform mat4 Model;\n\
\n\
void main() {\n\
  //Trasformazione dei vertici dalle coordinate nel sistema di riferimento dell'oggetto (apos), al sistema di riferimento del mondo (premoltiplicazione\n\
  // per Model) e successivamente proiettate nel cubo di centro l'origine e lato lungo 2, con x,y,z che variano tra -1 ed 1- (premoltiplicazione\n\
  //per la matrice Projection)\n\
  gl_Position = Projection*Model*vec4(aPos, 1.0);\n\
  outColor = aColor; // set ourColor to the input color we got from the vertex data\n\
}\n\
\n\
"
#define BASIC_FRAGMENT_SHADER_CODE "\
#version 330 core\n\
\n\
uniform float time;\n\
uniform vec2 resolution;\n\
uniform bool isBackground;\n\
in vec4 outColor;\n\
out vec4 FragColor;\n\
\n\
float rand(vec2 co){\n\
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);\n\
}\n\
\n\
float noise(vec2 p) {\n\
    vec2 ip = floor(p);\n\
    vec2 u = fract(p);\n\
    u = u*u*(3.0-2.0*u);\n\
\n\
    float res = mix(\n\
        mix(rand(ip), rand(ip + vec2(1.0, 0.0)), u.x),\n\
        mix(rand(ip + vec2(0.0, 1.0)), rand(ip + vec2(1.0, 1.0)), u.x),\n\
        u.y);\n\
    return res*res;\n\
}\n\
void main() {\n\
    vec2 p = gl_FragCoord.xy / resolution;\n\
    if (isBackground) {\n\
        if (p.y > 0.3) { // Spostato il confine per dare più spazio all'erba\n\
            float x = p.x + time * 0.1;\n\
            float n = noise(vec2(x * 10.0, p.y * 10.0));\n\
            vec3 skyColor = vec3(0.5, 0.7, 0.9);\n\
            vec3 cloudColor = vec3(0.9, 0.9, 0.95);\n\
            vec3 finalColor = mix(skyColor, cloudColor, n);\n\
            FragColor = vec4(finalColor, 1.0);\n\
        } else if (p.y > 0.25) { // Zona d'erba\n\
            float n = noise(vec2(p.x * 50.0, p.y * 50.0)); // Rumore più fine per l'erba\n\
            vec3 grassColorBase = vec3(0.25, 0.5, 0.2); // Verde erba\n\
            vec3 grassDetail = vec3(0.2, 0.45, 0.15); // Variazioni di colore\n\
            vec3 finalGrassColor = mix(grassColorBase, grassDetail, n);\n\
            FragColor = vec4(finalGrassColor, 1.0);\n\
        } else { // Terreno\n\
            float n = noise(vec2(p.x * 25.0, p.y * 25.0));\n\
            vec3 groundBaseColor = vec3(0.3, 0.2, 0.1);\n\
            vec3 groundDetailColor = vec3(0.4, 0.3, 0.2);\n\
            vec3 finalGroundColor = mix(groundBaseColor, groundDetailColor, n);\n\
            FragColor = vec4(finalGroundColor, 1.0);\n\
        }\n\
    } else {\n\
        FragColor = outColor;\n\
    }\n\
}\n\
"
#define BASIC_VERTEX_TEXT_SHADER_CODES "\
#version 330 core\n\
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n\
out vec2 TexCoords;\n\
\n\
uniform mat4 Projection_text;\n\
\n\
void main() {\n\
  gl_Position = Projection_text * vec4(vertex.xy, 0.0, 1.0);\n\
  TexCoords = vertex.zw;\n\
}\n\
"
#define BASIC_FRAGMENT_TEXT_SHADER_CODE "\
#version 330 core\n\
in vec2 TexCoords;\n\
out vec4 color;\n\
\n\
uniform sampler2D text;\n\
uniform vec3 textColor;\n\
\n\
void main() {\n\
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n\
    color = vec4(textColor, 1.0) * sampled;\n\
}\n\
"

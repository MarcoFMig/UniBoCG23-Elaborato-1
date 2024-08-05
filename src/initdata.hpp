#pragma once

#define DEFAULT_WINDOW_WIDTH        1280
#define DEFAULT_WINDOW_HEIGHT       720
#define DEFAULT_FONT_LOCATION       "./fonts/eurostile.ttf"
#define VERTEX_SHADER_NAME          "basic_vertex.vert"
#define FRAGMENT_SHADER_NAME        "basic_fragment.frag"
#define BASIC_VERTEX_SHADER_CODE    "\
#version 330 core\n\
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0\n\
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1\n\
out vec4 ourColor; // output a color to the fragment shader\n\
uniform mat4 Projection;  //vARIABILE DI TIPO uniform, rimane la stessa per ogni vertice della primitiva e viene passata dall'esterno\n\
uniform mat4 Model;\n\
\n\
void main() {\n\
  //Trasformazione dei vertici dalle coordinate nel sistema di riferimento dell'oggetto (apos), al sistema di riferimento del mondo (premoltiplicazione\n\
  // per Model) e successivamente proiettate nel cubo di centro l'origine e lato lungo 2, con x,y,z che variano tra -1 ed 1- (premoltiplicazione\n\
  //per la matrice Projection)\n\
  gl_Position = Projection*Model*vec4(aPos, 1.0);\n\
  ourColor = aColor; // set ourColor to the input color we got from the vertex data\n\
}"
#define BASIC_FRAGMENT_SHADER_CODE "\
#version 330 core\n\
in vec4 ourColor;\n\
out vec4 FragColor;\n\
\n\
void main() {\n\
  FragColor = vec4(ourColor);\n\
}"

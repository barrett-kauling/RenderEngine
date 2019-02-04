#version 450

in  vec4  gl_Vertex;
in  mat4  gl_ModelViewMatrix;
in  mat4  gl_ProjectionMatrix;
out vec4  gl_FrontColor;
out vec4  gl_BackColor;
out vec4  gl_Position;

void main(void)
{ 

    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

}

//unused

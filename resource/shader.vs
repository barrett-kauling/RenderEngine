# version 130

in          vec3    Vertex;
in          vec3    Normal;
in          vec3    FrontColor;
in          vec3    BackColor;
in          vec2    Texture;


uniform     mat4    ViewMatrix;
uniform     mat4    ModelMatrix;
uniform     mat4    ProjectionMatrix;

out         vec4    vs_Position;
out         vec4    vs_FrontColor;
out         vec4    vs_BackColor;
out         vec2    vs_Coordinate;
out         vec4    vs_Normal;


void main()
{

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Vertex, 1.0);
    vs_FrontColor = vec4(FrontColor, 1.0);
    vs_BackColor = vec4(BackColor, 1.0);
    vs_Coordinate = Texture;
    vs_Normal = vec4(Normal, 1.0);
}



//    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Vertex, 1.0f);


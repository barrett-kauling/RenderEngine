# version 130

in          vec4        vs_Position;
in          vec4        vs_FrontColor;
in          vec4        vs_BackColor;
in          vec4        vs_Normal;

out         vec4        RenderColor;

in          vec2        vs_Coordinate;
uniform     sampler2D   Sampler;


void main()
{

  if (gl_FrontFacing)
    {
        RenderColor = texture2D(Sampler, vs_Coordinate.xy);
    }
  else
    {
        RenderColor = vs_Normal*.5;
    }

}

/*
float near = 1.0; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    RenderColor = vec4(vec3(depth), 1.0f);
}
*/

//layout (location = 0)   uniform     vec3        FragmentColor;
//layout (location = 1)   uniform     sampler2D   Sampler;



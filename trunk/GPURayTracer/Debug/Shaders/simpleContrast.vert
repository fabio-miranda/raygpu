uniform sampler2D text;
uniform float alpha;
uniform vec3 avgLuminance;

void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();
}

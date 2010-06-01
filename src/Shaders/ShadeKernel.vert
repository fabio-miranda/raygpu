uniform sampler2D rayPos;
uniform sampler2D rayDir;

uniform sampler1D grid;
uniform sampler1D triangleList;
uniform sampler1D vertexes;

void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0; //RayPos && RayDir
   gl_TexCoord[1] = gl_MultiTexCoord1; //Grid
   gl_TexCoord[2] = gl_MultiTexCoord2; //TriangleList
   gl_TexCoord[3] = gl_MultiTexCoord3; //Vertexes
//   gl_TexCoord[4] = gl_MultiTexCoord4;
//   gl_TexCoord[5] = gl_MultiTexCoord5;
//   gl_TexCoord[6] = gl_MultiTexCoord6;
//   gl_TexCoord[7] = gl_MultiTexCoord7;

   gl_Position = ftransform();
}

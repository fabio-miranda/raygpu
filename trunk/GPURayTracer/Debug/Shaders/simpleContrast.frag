uniform sampler2D text;
uniform float alpha;
uniform vec3 avgLuminance;

void main()
{
//   luminance = 0.27 red + 0.67 green + 0.06 blue.
   vec4 tex = texture2D(text, gl_TexCoord[0].st);
   vec4 color = vec4(mix(avgLuminance, tex.rgb, alpha), 1.0);

   gl_FragColor = color;
}


uniform sampler2D samplerTex;
uniform float state;

void main(void)
{
	vec4 value = texture2D(samplerTex, gl_TexCoord[0].st);


	if (floor(value.w+0.5) != floor(state+0.5)) discard;

	gl_FragColor = value;
}
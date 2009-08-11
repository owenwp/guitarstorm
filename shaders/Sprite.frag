uniform sampler2D tex;
uniform float edgeSize;
uniform int alphaOnly;

void main()
{
	// perform alpha testing based on input texture
	vec4 texture = texture2D(tex,gl_TexCoord[0].st);
	
	if(alphaOnly == 0)
	{
		gl_FragColor = gl_Color * texture;
	}
	else
	{
		gl_FragColor = gl_Color;
	}
	
	float delta = abs(dFdx(gl_TexCoord[0].s)) + abs(dFdx(gl_TexCoord[0].t));
	float min = 0.5 - delta * edgeSize;
	float max = 0.5 + delta * edgeSize;

	gl_FragColor.a = smoothstep(min, max, texture.a);
}

uniform sampler2D tex;

void main()
{
	// perform alpha testing based on input texture
	float alpha = texture2D(tex,gl_TexCoord[0].st).a;
	
	gl_FragColor = gl_Color;
	
	float delta = abs(abs(dFdx(alpha)) + abs(dFdy(alpha)));
	float min = 0.5 - delta * 2.0;
	float max = 0.5 + delta * 2.0;

	gl_FragColor.a *= smoothstep(min, max, alpha);
}
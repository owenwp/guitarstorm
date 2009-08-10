uniform sampler2D tex;

void main()
{
	// perform alpha testing based on input texture
	float alpha = texture2D(tex,gl_TexCoord[0].st).a;
	gl_FragColor = gl_Color;
	if(alpha < 0.5)
	{
		gl_FragColor.a = 0.0;
	}
}
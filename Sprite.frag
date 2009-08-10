uniform sampler2D tex;

void main()
{
	gl_FragColor = gl_Color;
	gl_FragColor.a = texture2D(tex,gl_TexCoord[0].st);
}
uniform sampler2D tex;
uniform int alphaOnly;

uniform float glowSize;
uniform vec4 glowColor;

uniform float shadowAlpha;

varying float edge;
varying vec4 shadowDirection;

void main()
{
	// perform alpha testing based on input texture
	vec4 texture = texture2D(tex,gl_TexCoord[0].st);
	float shadow;
	vec4 color;
	
	if(shadowAlpha > 0.001)
	{
		shadow = texture2D(tex, gl_TexCoord[0].st + shadowDirection.xy).a * shadowAlpha;
	}
	
	if(alphaOnly == 0)
	{
		color = gl_Color * texture;
	}
	else
	{
		color = gl_Color;
	}
		
	color.a = (texture.a - 0.5) * edge;
	
	if(shadowAlpha > 0.001)
	{
		color.a = max(shadow, color.a);
	}
	
	gl_FragColor = color;
}

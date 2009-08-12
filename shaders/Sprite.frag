uniform sampler2D tex;
uniform float edgeSize;
uniform int alphaOnly;

uniform float glowSize;
uniform vec4 glowColor;

uniform float shadowAlpha;
uniform vec3 shadowPosition;

void main()
{
	// perform alpha testing based on input texture
	vec4 texture = texture2D(tex,gl_TexCoord[0].st);
	float shadow;
	vec4 color;
	
	if(shadowAlpha > 0.001)
	{
		shadow = texture2D(tex, gl_TexCoord[0].st + shadowPosition.xy).a * shadowAlpha;
	}
	
	if(alphaOnly == 0)
	{
		color = gl_Color * texture;
	}
	else
	{
		color = gl_Color;
	}
	
	float delta = abs(dFdx(gl_TexCoord[0].s)) + abs(dFdx(gl_TexCoord[0].t));
	float min = 0.5 - delta * edgeSize;
	float max = 0.5 + delta * edgeSize;
	
	color.a = smoothstep(min, max, texture.a);
		
	if(shadowAlpha > 0.001)
	{
		min = 0.5 - 0.5 * shadowPosition.z;
		max = 0.5 + 0.5 * shadowPosition.z;
		vec4 scolor = vec4(0, 0, 0, smoothstep(min, max, shadow));
		
		color.rgb = mix(scolor.rgb, color.rgb, color.a);
		color.a = max(scolor.a, color.a);
	}
	
	gl_FragColor = color;
}

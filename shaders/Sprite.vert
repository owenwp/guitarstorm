uniform float edgeSize;
uniform vec3 shadowPosition;

varying float edge;
varying vec4 shadowDirection;

void main()
{
	// simply pass on input parameters
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color; 
	gl_Position = ftransform();
	edge = edgeSize * 10.0 * length(gl_ModelViewMatrix * vec4(1, 1, 0, 0));
	shadowDirection = gl_ModelViewMatrix * vec4(shadowPosition, 0) * 0.001;
}

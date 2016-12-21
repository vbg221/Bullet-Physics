uniform sampler2D screenFramebuffer3;
varying vec2 texCoordVar;

void main()
{
	
		const float gamma = 2.2;
vec3 hdrColor = texture2D( screenFramebuffer3, texCoordVar).rgb;
vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
mapped = pow(mapped, vec3(gamma));
gl_FragColor = vec4(mapped, 1.0);
	
}
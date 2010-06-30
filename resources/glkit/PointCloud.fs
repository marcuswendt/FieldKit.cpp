//
// Point particle fragment shader
//
void main() {
	vec2 tc = gl_TexCoord[0].st;
	
	// draw a round circle
	float dist = distance(vec2(0.5, 0.5), tc);
	if(dist > 0.5)
		discard;
	
	gl_FragColor = gl_Color;
}
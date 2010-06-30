//
// Point particle vertex shader
//
attribute vec3 InVertex;
attribute vec4 InColor;
attribute float InSize;

void main() {
	vec4 vertex = vec4(InVertex, 1.0);
	vec4 position = gl_ProjectionMatrix * gl_ModelViewMatrix * vertex;
	gl_Position = position;
	gl_PointSize = InSize;
	gl_FrontColor = InColor;
}
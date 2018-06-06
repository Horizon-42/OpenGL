#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

out vec3 myColor;
out vec2 myPos;

uniform mat4 transform;

//uniform bool isCut;
//uniform mat4 cut;

void main(){
	gl_Position = transform*vec4(aPos,1.0f);
	myColor = aColor;
	myPos = gl_Position.xy;
}

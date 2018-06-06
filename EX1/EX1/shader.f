#version 330 core
in vec3 myColor;
in vec2 myPos;
out vec4 FragColor;

uniform bool isCut;
uniform vec4 cut;

void main(){
	if(isCut&&(myPos.x<cut.x||myPos.x>cut.y||myPos.y<cut.z||myPos.y>cut.w)){
		FragColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);
	}
	else{
		FragColor = vec4(myColor, 1.0f);
	}
}


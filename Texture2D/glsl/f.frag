#version 450
layout(binding=0) uniform sampler2D testImage ;

in VS_OUT{
	vec2 st ;
} vs_in;

out vec4 color ;

void main(void)
{
    color = texture( testImage , vs_in.st ) ;
}


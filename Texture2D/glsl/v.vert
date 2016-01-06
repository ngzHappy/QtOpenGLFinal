#version 450

layout(location=0) in vec4 position ;

out VS_OUT{
	vec2 st ;
} vs_out;

void main(void)
{
    gl_Position = position ; 
	vs_out.st = ((position+1)/2).xy ;
}

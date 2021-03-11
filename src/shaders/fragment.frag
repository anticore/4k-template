#version 130

vec2 resolution = vec2(1920.0, 1080.0);
float time=gl_TexCoord[0].x/44100-12.5;

void main() {
	vec2 uv = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y);
    uv -= 0.5;
    uv /= vec2(resolution.y / resolution.x, 1);
	gl_FragColor=vec4(1., 0., 0., 1.);
}


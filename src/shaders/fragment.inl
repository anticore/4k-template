#pragma data_seg(".shader")
const char* fragment =
 "#version 130\n"
 "vec2 d=vec2(1920,1080);"
 "float r=gl_TexCoord[0].x/44100-12.5;"
 "void main()"
 "{"
   "vec2 r=vec2(gl_FragCoord.x/d.x,gl_FragCoord.y/d.y);"
   "r-=.5;"
   "r/=vec2(d.y/d.x,1);"
   "gl_FragColor=vec4(1,0,0,1);"
 "}";

#define DEBUG 1

#if DEBUG
	#define OPENGL_DEBUG        1
	#define FULLSCREEN          0
	#define STRETCH				0 
#else
	#define OPENGL_DEBUG        0
	#define FULLSCREEN          0
	#define STRETCH				1 
#endif
#define USE_AUDIO    1
#define NO_UNIFORMS  1

#include "definitions.h"
#if OPENGL_DEBUG
	#include "debug.h"
#endif
#include "glext.h"
#include "shaders/fragment.inl"

#pragma data_seg(".pids")
static int pidMain;
static int pidPost;

#pragma code_seg(".main")
void entrypoint(void) {
	#if FULLSCREEN
		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
		ShowCursor(0);
		const HDC hDC = GetDC(CreateWindow((LPCSTR)0xC018, 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0));
	#else
		#if STRETCH
			const HDC hDC = GetDC(CreateWindow((LPCSTR)0xC018, 0, WS_POPUP | WS_VISIBLE , 0, 0, XRES, YRES, 0, 0, 0, 0));
		#else		
			const HDC hDC = GetDC(CreateWindow((LPCSTR)0xC018, 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0));
		#endif
	#endif  

	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));

	((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram")) (((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress("glCreateShaderProgramv"))(GL_FRAGMENT_SHADER, 1, &fragment));

	#if USE_AUDIO
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
		waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
		waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
		waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	#endif
		 
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	do {
		#if USE_AUDIO
			waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));

			#if NO_UNIFORMS
				glTexCoord3d(MMTime.u.sample, 0, 0);
			#else
				((PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i"))(0, MMTime.u.sample);
			#endif
		#endif
				
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, XRES, YRES, 0);
		glRects(-1, -1, 1, 1); 

		SwapBuffers(hDC);
	} while(!GetAsyncKeyState(VK_ESCAPE)
		#if USE_AUDIO
			&& MMTime.u.sample < MAX_SAMPLES
		#endif
	);

	ExitProcess(0);
}

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define VC_LEANMEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <GL/gl.h>

#define XRES 1920
#define YRES 1080

#pragma data_seg(".pixelfmt")
static const PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
	32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
};
 
#pragma data_seg(".screensettings")
static DEVMODE screenSettings = { 
	{0}, 0, 0, sizeof(screenSettings), 0, DM_PELSWIDTH|DM_PELSHEIGHT,
	{0}, 0, 0, 0, 0, 0, {0}, 0, 0, XRES, YRES, 0, 0,
	#if(WINVER >= 0x0400)
		0, 0, 0, 0, 0, 0,
			#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
			0, 0
		#endif
	#endif
};


#if USE_AUDIO
	#include <mmsystem.h>
	#include <mmreg.h>

	#include "4klang/4klang.h"

	#pragma data_seg(".4klangout")
	static SAMPLE_TYPE lpSoundBuffer[MAX_SAMPLES * 2];
	static HWAVEOUT hWaveOut;

	#pragma data_seg(".wavefmt")
	static WAVEFORMATEX WaveFMT =
	{
	#ifdef FLOAT_32BIT	
		WAVE_FORMAT_IEEE_FLOAT,
	#else
		WAVE_FORMAT_PCM,
	#endif		
		2, 
		SAMPLE_RATE,  
		SAMPLE_RATE*sizeof(SAMPLE_TYPE) * 2,
		sizeof(SAMPLE_TYPE) * 2,
		sizeof(SAMPLE_TYPE) * 8,  
		0 
	};

	#pragma data_seg(".wavehdr")
	static WAVEHDR WaveHDR =
	{
		(LPSTR)lpSoundBuffer, MAX_SAMPLES * sizeof(SAMPLE_TYPE) * 2, 0, 0, 0, 0, 0, 0
	};

	static MMTIME MMTime =
	{
		TIME_SAMPLES, 0
	};
#endif
//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2015                                   //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include <mmsystem.h>
#include <mmreg.h>

// #include "glext.h"

#include "../usync.h"
#include "4klang.h"

#define XRES    1280
#define YRES     720

const static char *fragmentShader = \
//"varying vec4 v;"
"void main()"
"{"
"vec2 v=-1.+gl_FragCoord.xy/vec2(400.,300.);"
// tunnel
"float r=pow(pow(abs(v.x),18.)+pow(abs(v.y),10.),1.8/28.);"
"vec2 t=vec2(gl_Color.x+1./r,atan(v.x,v.y));"
// texture
"t=fract(2*t)-.5;"
"vec4 col=(1-pow(dot(t.xy,t.xy),.3))*vec4(2,1.8,1.6,0)+vec4(.3,.2,.1,0)+.12*vec4(v,0,0);"
// final color
"gl_FragColor=col/(2.*r);"
"}";

// MAX_SAMPLES gives you the number of samples for the whole song. we always produce stereo samples, so times 2 for the buffer
SAMPLE_TYPE lpSoundBuffer[MAX_SAMPLES * 2];
HWAVEOUT    hWaveOut;

/////////////////////////////////////////////////////////////////////////////////
// initialized data
/////////////////////////////////////////////////////////////////////////////////

#pragma data_seg(".wavefmt")
WAVEFORMATEX WaveFMT =
{
#ifdef FLOAT_32BIT
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif		
	2, // channels
	SAMPLE_RATE, // samples per sec
	SAMPLE_RATE*sizeof(SAMPLE_TYPE) * 2, // bytes per sec
	sizeof(SAMPLE_TYPE) * 2, // block alignment;
	sizeof(SAMPLE_TYPE) * 8, // bits per sample
	0 // extension not needed
};

#pragma data_seg(".wavehdr")
WAVEHDR WaveHDR = {
	(LPSTR)lpSoundBuffer,
	MAX_SAMPLES*sizeof(SAMPLE_TYPE) * 2,			// MAX_SAMPLES*sizeof(float)*2(stereo)
	0,
	0,
	0,
	0,
	0,
	0
};

MMTIME MMTime = {
	TIME_SAMPLES,
	0
};

HDC hDC;

void init_display()
{
#ifdef SYNC_PLAYER
	static DEVMODE screenSettings = {
		{ 0 }, 0, 0, sizeof(DEVMODE), 0, DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT, { 0 }, 0, 0, 0, 0, 0, { 0 }, 0, 32, XRES, YRES, { 0 }, 0
	};

	// full screen
	if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return;

	ShowCursor(0);

	hDC = GetDC(CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0));
#else
	hDC = GetDC(CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE, 0, 0, XRES, YRES, 0, 0, 0, 0));
#endif

	const static PIXELFORMATDESCRIPTOR pfd = {
		0, 0, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 0
	};

	// init opengl
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));
}

#pragma code_seg(".initsnd")
void  init_sound()
{
#ifdef USE_SOUND_THREAD
	// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
#else
	_4klang_render(lpSoundBuffer);
#endif
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
}

#ifdef SYNC_PLAYER
#include <emmintrin.h>
extern "C" int _fltused = 0;
extern "C" __declspec(naked) void _ftol2_sse()
{
	__asm {
		fstp        dword ptr[esp - 4]
		movss       xmm0, dword ptr[esp - 4]
		cvttps2dq   xmm0, xmm0
		movd        eax, xmm0
		ret
	}
}
#else
static int offset = 0;
#endif

#include <stdlib.h>

float get_row()
{
	waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
#ifndef SYNC_PLAYER
	return (float)(MMTime.u.sample + offset) / SAMPLES_PER_TICK;
#else
	return (float)(MMTime.u.sample) / SAMPLES_PER_TICK;
#endif
}

#define GL_FRAGMENT_SHADER                0x8B30
typedef GLuint(APIENTRY *PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint(APIENTRY *PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const char* const *string, const GLint *length);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC) (GLuint program);

#pragma code_seg(".main")
void entrypoint(void)
{
	if (usync_init() < 0)
		return;

	init_display();
	init_sound();

	// create shader
	const int p = ((PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram"))();
	const int s = ((PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader"))(GL_FRAGMENT_SHADER);
	((PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource"))(s, 1, &fragmentShader, 0);
	((PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader"))(s);
	((PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader"))(p, s);
	((PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram"))(p);
	((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))(p);

#ifndef SYNC_PLAYER
	/* HACK: prefetch tracks - not needed when actually editing */
	usync_get_val(foo);
	usync_update(0.0f);
#endif

	// run
	do {
		float row = get_row();
		usync_update(row);

		float foo = usync_get_val(foo);
		glColor3f(foo, 0, 0);
		glRects(-1, -1, 1, 1);
		SwapBuffers(hDC);
	} while (!GetAsyncKeyState(VK_ESCAPE));

#ifndef SYNC_PLAYER
	usync_export("sync-data.h");
#endif

	ExitProcess(0);
}

#ifndef SYNC_PLAYER

static int paused = 0;
static void pause(void *d, int flag)
{
	if (flag)
		waveOutPause(hWaveOut);
	else
		waveOutRestart(hWaveOut);
	paused = flag;
}

static void set_row(void *d, int row)
{
	if (!paused)
		return;

	waveOutReset(hWaveOut);
	offset = (int)(row * SAMPLES_PER_TICK);
	WaveHDR.lpData = (LPSTR)(lpSoundBuffer + offset);
	WaveHDR.dwBufferLength = (MAX_SAMPLES - offset) * sizeof(SAMPLE_TYPE) * 2;
	waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	waveOutPause(hWaveOut);
}

static int is_playing(void *d)
{
	return !paused;
}

struct sync_cb usync_cb = {
	pause,
	set_row,
	is_playing
};

void *usync_cb_data = NULL;

#endif

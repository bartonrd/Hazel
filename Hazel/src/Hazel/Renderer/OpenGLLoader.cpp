#include "OpenGLLoader.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Function pointer definitions
PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;

void HazelOpenGLInit()
{
#ifdef _WIN32
	// On Windows, we can use wglGetProcAddress to load OpenGL functions
	HMODULE opengl32 = LoadLibraryA("opengl32.dll");
	if (!opengl32)
		return;

	typedef PROC (WINAPI * PFNWGLGETPROCADDRESSPROC)(LPCSTR);
	PFNWGLGETPROCADDRESSPROC wglGetProcAddress = (PFNWGLGETPROCADDRESSPROC)GetProcAddress(opengl32, "wglGetProcAddress");
	
	if (wglGetProcAddress)
	{
		glad_glDepthFunc = (PFNGLDEPTHFUNCPROC)wglGetProcAddress("glDepthFunc");
		glad_glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		glad_glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		glad_glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
	}
	
	// glDepthFunc is actually in OpenGL 1.0, so it might be directly exported
	if (!glad_glDepthFunc)
		glad_glDepthFunc = (PFNGLDEPTHFUNCPROC)GetProcAddress(opengl32, "glDepthFunc");
#endif
}

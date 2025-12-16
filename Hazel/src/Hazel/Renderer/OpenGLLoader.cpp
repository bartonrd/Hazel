#include "OpenGLLoader.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

// Function pointer definitions
PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;

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
		glad_glBlendFunc = (PFNGLBLENDFUNCPROC)wglGetProcAddress("glBlendFunc");
		glad_glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		glad_glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		glad_glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
		glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
		glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
		glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
		glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
		glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
		glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
		glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
		glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
		glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
		glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	}
	
	// OpenGL 1.x functions might be directly exported from opengl32.dll
	if (!glad_glDepthFunc)
		glad_glDepthFunc = (PFNGLDEPTHFUNCPROC)GetProcAddress(opengl32, "glDepthFunc");
	if (!glad_glBlendFunc)
		glad_glBlendFunc = (PFNGLBLENDFUNCPROC)GetProcAddress(opengl32, "glBlendFunc");
#endif
}

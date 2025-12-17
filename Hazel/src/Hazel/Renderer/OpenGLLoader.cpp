#include "OpenGLLoader.h"

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
	// Use ImGui's OpenGL loader to get function pointers
	// This works cross-platform and handles extension loading properly
	glad_glDepthFunc = (PFNGLDEPTHFUNCPROC)imgl3wGetProcAddress("glDepthFunc");
	glad_glBlendFunc = (PFNGLBLENDFUNCPROC)imgl3wGetProcAddress("glBlendFunc");
	glad_glUniform1f = (PFNGLUNIFORM1FPROC)imgl3wGetProcAddress("glUniform1f");
	glad_glUniform3f = (PFNGLUNIFORM3FPROC)imgl3wGetProcAddress("glUniform3f");
	glad_glUniform4f = (PFNGLUNIFORM4FPROC)imgl3wGetProcAddress("glUniform4f");
	glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)imgl3wGetProcAddress("glGenFramebuffers");
	glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)imgl3wGetProcAddress("glBindFramebuffer");
	glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)imgl3wGetProcAddress("glFramebufferTexture2D");
	glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)imgl3wGetProcAddress("glGenRenderbuffers");
	glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)imgl3wGetProcAddress("glBindRenderbuffer");
	glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)imgl3wGetProcAddress("glRenderbufferStorage");
	glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)imgl3wGetProcAddress("glFramebufferRenderbuffer");
	glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)imgl3wGetProcAddress("glCheckFramebufferStatus");
	glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)imgl3wGetProcAddress("glDeleteFramebuffers");
	glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)imgl3wGetProcAddress("glDeleteRenderbuffers");
}

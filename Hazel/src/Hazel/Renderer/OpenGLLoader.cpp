#include "OpenGLLoader.h"

// Function pointer definitions
HAZEL_API PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
HAZEL_API PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
HAZEL_API PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
HAZEL_API PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
HAZEL_API PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
HAZEL_API PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
HAZEL_API PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
HAZEL_API PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
HAZEL_API PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
HAZEL_API PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
HAZEL_API PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
HAZEL_API PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
HAZEL_API PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
HAZEL_API PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
HAZEL_API PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;

HAZEL_API void HazelOpenGLInit()
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

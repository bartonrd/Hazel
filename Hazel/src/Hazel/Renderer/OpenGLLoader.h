#pragma once

// This file extends imgui_impl_opengl3_loader.h with additional OpenGL functions
// needed by the Hazel renderer that aren't used by ImGui itself

#include <imgui_impl_opengl3_loader.h>

#ifdef __cplusplus
extern "C" {
#endif

// Additional OpenGL constants not in ImGui's minimal loader
#ifndef GL_LESS
#define GL_LESS                           0x0201
#endif

#ifndef GL_INT
#define GL_INT                            0x1404
#endif

#ifndef GL_BOOL
#define GL_BOOL                           0x8B56
#endif

#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW                    0x88E4
#endif

#ifndef GL_DEPTH_BUFFER_BIT
#define GL_DEPTH_BUFFER_BIT               0x00000100
#endif

#ifndef GL_COLOR_BUFFER_BIT
#define GL_COLOR_BUFFER_BIT               0x00004000
#endif

// Additional OpenGL function declarations not in ImGui's minimal loader
#ifndef glDepthFunc
typedef void (APIENTRYP PFNGLDEPTHFUNCPROC) (GLenum func);
extern PFNGLDEPTHFUNCPROC glad_glDepthFunc;
#define glDepthFunc glad_glDepthFunc
#endif

#ifndef glUniform1f
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
extern PFNGLUNIFORM1FPROC glad_glUniform1f;
#define glUniform1f glad_glUniform1f
#endif

#ifndef glUniform3f
typedef void (APIENTRYP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern PFNGLUNIFORM3FPROC glad_glUniform3f;
#define glUniform3f glad_glUniform3f
#endif

#ifndef glUniform4f
typedef void (APIENTRYP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern PFNGLUNIFORM4FPROC glad_glUniform4f;
#define glUniform4f glad_glUniform4f
#endif

// Function to initialize our additional OpenGL functions
// Call this after imgl3wInit()
void HazelOpenGLInit();

#ifdef __cplusplus
}
#endif

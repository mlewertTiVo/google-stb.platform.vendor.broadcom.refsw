/*=============================================================================
Copyright (c) 2011 Broadcom Europe Limited.
All rights reserved.

Project  :  khronos
Module   :  Header file

FILE DESCRIPTION
Client-side implementation of the GL_OES_framebuffer_object extension.
http://www.khronos.org/registry/gles/extensions/OES/OES_framebuffer_object.txt
=============================================================================*/

#define GL_GLEXT_PROTOTYPES /* we want the prototypes so the compiler will check that the signatures match */

#include "interface/khronos/common/khrn_client_mangle.h"

#include "interface/khronos/common/khrn_int_common.h"

#include "interface/khronos/glxx/glxx_client.h"

#include "interface/khronos/glxx/glxx_int_impl.h"
#include "interface/khronos/glxx/gl11_int_impl.h"

#include "interface/khronos/include/GLES/gl.h"
#include "interface/khronos/include/GLES/glext.h"

extern GLboolean glxx_client_IsRenderbuffer(GLuint renderbuffer);
extern void glxx_client_BindRenderbuffer(GLenum target, GLuint renderbuffer);
extern void glxx_client_DeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
extern void glxx_client_GenRenderbuffers(GLsizei n, GLuint *renderbuffers);
extern void glxx_client_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void glxx_client_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
extern GLboolean glxx_client_IsFramebuffer(GLuint framebuffer);
extern void glxx_client_BindFramebuffer(GLenum target, GLuint framebuffer);
extern void glxx_client_DeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
extern void glxx_client_GenFramebuffers(GLsizei n, GLuint *framebuffers);
extern GLenum glxx_client_CheckFramebufferStatus(GLenum target);
extern void glxx_client_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void glxx_client_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void glxx_client_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern void glxx_client_GenerateMipmap(GLenum target);

GL_API GLboolean GL_APIENTRY glIsRenderbufferOES (GLuint renderbuffer)
{
   return glxx_client_IsRenderbuffer(renderbuffer);
}

GL_API void GL_APIENTRY glBindRenderbufferOES (GLenum target, GLuint renderbuffer)
{
   glxx_client_BindRenderbuffer(target, renderbuffer);
}

GL_API void GL_APIENTRY glDeleteRenderbuffersOES (GLsizei n, const GLuint* renderbuffers)
{
   glxx_client_DeleteRenderbuffers(n, renderbuffers);
}

GL_API void GL_APIENTRY glGenRenderbuffersOES (GLsizei n, GLuint* renderbuffers)
{
   glxx_client_GenRenderbuffers(n, renderbuffers);
}

GL_API void GL_APIENTRY glRenderbufferStorageOES (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
   glxx_client_RenderbufferStorage(target, internalformat, width, height);
}

GL_API void GL_APIENTRY glGetRenderbufferParameterivOES (GLenum target, GLenum pname, GLint* params)
{
   glxx_client_GetRenderbufferParameteriv(target, pname, params);
}

GL_API GLboolean GL_APIENTRY glIsFramebufferOES (GLuint framebuffer)
{
   return glxx_client_IsFramebuffer(framebuffer);
}

GL_API void GL_APIENTRY glBindFramebufferOES (GLenum target, GLuint framebuffer)
{
   glxx_client_BindFramebuffer(target, framebuffer);
}

GL_API void GL_APIENTRY glDeleteFramebuffersOES (GLsizei n, const GLuint* framebuffers)
{
   glxx_client_DeleteFramebuffers(n, framebuffers);
}

GL_API void GL_APIENTRY glGenFramebuffersOES (GLsizei n, GLuint* framebuffers)
{
   glxx_client_GenFramebuffers(n, framebuffers);
}

GL_API GLenum GL_APIENTRY glCheckFramebufferStatusOES (GLenum target)
{
   return glxx_client_CheckFramebufferStatus(target);
}

GL_API void GL_APIENTRY glFramebufferRenderbufferOES (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   glxx_client_FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

GL_API void GL_APIENTRY glFramebufferTexture2DOES (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   glxx_client_FramebufferTexture2D(target, attachment, textarget, texture, level);
}

GL_API void GL_APIENTRY glGetFramebufferAttachmentParameterivOES (GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
   glxx_client_GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

GL_API void GL_APIENTRY glGenerateMipmapOES (GLenum target)
{
   glxx_client_GenerateMipmap(target);
}

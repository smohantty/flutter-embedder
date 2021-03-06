#include "flutter/shell/platform/linux_embedded/rendering/gl_api.h"

#include <EGL/egl.h>

namespace flutter {

GlApi& GlApi::instance()
{
    static GlApi gInstance;
    return gInstance;
}

GlApi::GlApi()
{
#define GLPROC(name) name = (decltype(name))eglGetProcAddress(#name);

    GLPROC(glActiveTexture)
    GLPROC(glAttachShader)
    GLPROC(glBindAttribLocation)
    GLPROC(glBindBuffer)
    GLPROC(glBindFramebuffer)
    GLPROC(glBindRenderbuffer)
    GLPROC(glBindTexture)
    GLPROC(glBlendColor)
    GLPROC(glBlendEquation)
    GLPROC(glBlendEquationSeparate)
    GLPROC(glBlendFunc)
    GLPROC(glBlendFuncSeparate)
    GLPROC(glBufferData)
    GLPROC(glBufferSubData)
    GLPROC(glCheckFramebufferStatus)
    GLPROC(glClear)
    GLPROC(glClearColor)
    GLPROC(glClearDepthf)
    GLPROC(glClearStencil)
    GLPROC(glColorMask)
    GLPROC(glCompileShader)
    GLPROC(glCompressedTexImage2D)
    GLPROC(glCompressedTexSubImage2D)
    GLPROC(glCopyTexImage2D)
    GLPROC(glCopyTexSubImage2D)
    GLPROC(glCreateProgram)
    GLPROC(glCreateShader)
    GLPROC(glCullFace)
    GLPROC(glDeleteBuffers)
    GLPROC(glDeleteFramebuffers)
    GLPROC(glDeleteProgram)
    GLPROC(glDeleteRenderbuffers)
    GLPROC(glDeleteShader)
    GLPROC(glDeleteTextures)
    GLPROC(glDepthFunc)
    GLPROC(glDepthMask)
    GLPROC(glDepthRangef)
    GLPROC(glDetachShader)
    GLPROC(glDisable)
    GLPROC(glDisableVertexAttribArray)
    GLPROC(glDrawArrays)
    GLPROC(glDrawElements)
    GLPROC(glEnable)
    GLPROC(glEnableVertexAttribArray)
    GLPROC(glFinish)
    GLPROC(glFlush)
    GLPROC(glFramebufferRenderbuffer)
    GLPROC(glFramebufferTexture2D)
    GLPROC(glFrontFace)
    GLPROC(glGenBuffers)
    GLPROC(glGenerateMipmap)
    GLPROC(glGenFramebuffers)
    GLPROC(glGenRenderbuffers)
    GLPROC(glGenTextures)
    GLPROC(glGetActiveAttrib)
    GLPROC(glGetActiveUniform)
    GLPROC(glGetAttachedShaders)
    GLPROC(glGetAttribLocation)
    GLPROC(glGetBooleanv)
    GLPROC(glGetBufferParameteriv)
    GLPROC(glGetError)
    GLPROC(glGetFloatv)
    GLPROC(glGetFramebufferAttachmentParameteriv)
    GLPROC(glGetIntegerv)
    GLPROC(glGetProgramiv)
    GLPROC(glGetProgramInfoLog)
    GLPROC(glGetRenderbufferParameteriv)
    GLPROC(glGetShaderiv)
    GLPROC(glGetShaderInfoLog)
    GLPROC(glGetShaderPrecisionFormat)
    GLPROC(glGetShaderSource)
    GLPROC(glGetString)
    GLPROC(glGetTexParameterfv)
    GLPROC(glGetTexParameteriv)
    GLPROC(glGetUniformfv)
    GLPROC(glGetUniformiv)
    GLPROC(glGetUniformLocation)
    GLPROC(glGetVertexAttribfv)
    GLPROC(glGetVertexAttribiv)
    GLPROC(glGetVertexAttribPointerv)
    GLPROC(glHint)
    GLPROC(glIsBuffer)
    GLPROC(glIsEnabled)
    GLPROC(glIsFramebuffer)
    GLPROC(glIsProgram)
    GLPROC(glIsRenderbuffer)
    GLPROC(glIsShader)
    GLPROC(glIsTexture)
    GLPROC(glLineWidth)
    GLPROC(glLinkProgram)
    GLPROC(glPixelStorei)
    GLPROC(glPolygonOffset)
    GLPROC(glReadPixels)
    GLPROC(glReleaseShaderCompiler)
    GLPROC(glRenderbufferStorage)
    GLPROC(glSampleCoverage)
    GLPROC(glScissor)
    GLPROC(glShaderBinary)
    GLPROC(glShaderSource)
    GLPROC(glStencilFunc)
    GLPROC(glStencilFuncSeparate)
    GLPROC(glStencilMask)
    GLPROC(glStencilMaskSeparate)
    GLPROC(glStencilOp)
    GLPROC(glStencilOpSeparate)
    GLPROC(glTexImage2D)
    GLPROC(glTexParameterf)
    GLPROC(glTexParameterfv)
    GLPROC(glTexParameteri)
    GLPROC(glTexParameteriv)
    GLPROC(glTexSubImage2D)
    GLPROC(glUniform1f)
    GLPROC(glUniform1fv)
    GLPROC(glUniform1i)
    GLPROC(glUniform1iv)
    GLPROC(glUniform2f)
    GLPROC(glUniform2fv)
    GLPROC(glUniform2i)
    GLPROC(glUniform2iv)
    GLPROC(glUniform3f)
    GLPROC(glUniform3fv)
    GLPROC(glUniform3i)
    GLPROC(glUniform3iv)
    GLPROC(glUniform4f)
    GLPROC(glUniform4fv)
    GLPROC(glUniform4i)
    GLPROC(glUniform4iv)
    GLPROC(glUniformMatrix2fv)
    GLPROC(glUniformMatrix3fv)
    GLPROC(glUniformMatrix4fv)
    GLPROC(glUseProgram)
    GLPROC(glValidateProgram)
    GLPROC(glVertexAttrib1f)
    GLPROC(glVertexAttrib1fv)
    GLPROC(glVertexAttrib2f)
    GLPROC(glVertexAttrib2fv)
    GLPROC(glVertexAttrib3f)
    GLPROC(glVertexAttrib3fv)
    GLPROC(glVertexAttrib4f)
    GLPROC(glVertexAttrib4fv)
    GLPROC(glVertexAttribPointer)
    GLPROC(glViewport)

    //GLES 3 apis
    GLPROC(glGenVertexArrays)
    GLPROC(glBindVertexArray)

#undef GLPROC
}

}  // namespace flutter

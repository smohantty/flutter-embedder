#include "flutter/shell/platform/linux_embedded/rendering/frame_buffer.h"
#include "flutter/shell/platform/linux_embedded/rendering/gl_api.h"

#include <iostream>

namespace flutter {

struct GlFrameBuffer::Impl
{
  Impl(size_t width, size_t height)
  :mWidth(width), mHeight(height)
  {
    auto& api = GlApi::instance();

    // Create render texture.
      api.glGenTextures(2, mTextureArray);
      for (auto i = 0u; i < 2 ; i++) {
        api.glBindTexture(GL_TEXTURE_2D, mTextureArray[i]);
        api.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        api.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        api.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        api.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        api.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0,
                       GL_RGB, GL_UNSIGNED_BYTE, 0);
      }

    // Create render target.
    api.glGenFramebuffers(1, &mFramebuffer);
    Bind();
    mValid = true;
  }
  void Bind() const
  {
    auto& api = GlApi::instance();
    api.glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    api.glBindTexture(GL_TEXTURE_2D, mTextureArray[mActiveTextureIndex]);
    api.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               mTextureArray[mActiveTextureIndex], 0);
    std::cout<<"Bind texture for rendering : "<<mTextureArray[mActiveTextureIndex]<<"\n";
  }
  ~Impl()
  {
    auto& api = GlApi::instance();
    api.glDeleteFramebuffers(1, &mFramebuffer);
    api.glDeleteTextures(2, mTextureArray);
  }
  size_t   mWidth{0};
  size_t   mHeight{0};
  GLuint   mFramebuffer;
  GLuint   mTextureArray[2];
  bool     mValid{false};
  uint32_t mActiveTextureIndex{0};
};

GlFrameBuffer::GlFrameBuffer(size_t width, size_t height)
:mImpl(std::make_unique<GlFrameBuffer::Impl>(width, height))
{

}

GlFrameBuffer::~GlFrameBuffer() = default;

void GlFrameBuffer::Bind() const
{
  mImpl->Bind();

}

uint32_t GlFrameBuffer::Id() const
{
    return mImpl->mFramebuffer;
}

uint32_t GlFrameBuffer::Texture()
{
    mImpl->mActiveTextureIndex = (mImpl->mActiveTextureIndex == 0) ? 1 : 0;

    return mImpl->mTextureArray[mImpl->mActiveTextureIndex];
}

}  // namespace flutter

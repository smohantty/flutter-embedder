// Copyright 2021 Sony Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/linux_embedded/surface/surface_gl.h"
#include "flutter/shell/platform/linux_embedded/rendering/frame_buffer.h"
#include "flutter/shell/platform/linux_embedded/rendering/stereo_renderer.h"
#include "flutter/shell/platform/linux_embedded/rendering/gl_api.h"

#include <thread>

namespace flutter {

struct SurfaceGlOffscreen::Impl
{
  std::unique_ptr<GlFrameBuffer> mFlutterSurface;
  std::unique_ptr<GlStereoRenderer> mRenderer;
};

SurfaceGlOffscreen::SurfaceGlOffscreen(std::unique_ptr<ContextEgl> context)
:SurfaceGl(std::move(context)), mImpl(std::make_unique<SurfaceGlOffscreen::Impl>())
{
}

SurfaceGlOffscreen::~SurfaceGlOffscreen() = default;

bool SurfaceGlOffscreen::GLContextMakeCurrent() const {
  ELINUX_LOG(ERROR) << "Context make current "<<std::this_thread::get_id()<<"\n";
  bool status = onscreen_surface_->MakeCurrent();
  if (mImpl->mFlutterSurface)
  {
    mImpl->mFlutterSurface->Bind();
  }

  return status;
}

bool SurfaceGlOffscreen::GLContextClearCurrent() const {
  ELINUX_LOG(ERROR) << "Context clear current "<<std::this_thread::get_id()<<"\n";
  auto& api = GlApi::instance();
  api.glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return context_->ClearCurrent();
}

bool SurfaceGlOffscreen::GLContextPresent(uint32_t fbo_id) const {
  auto tex = mImpl->mFlutterSurface->Texture();
  ELINUX_LOG(ERROR) << "FBO make present "<<std::this_thread::get_id()<< " Texture ID : "<< tex<<"\n";
  mImpl->mRenderer->Render(tex);

  if (!onscreen_surface_->SwapBuffers()) {
    return false;
  }
  native_window_->SwapBuffers();
  auto& api = GlApi::instance();
  api.glBindFramebuffer(GL_FRAMEBUFFER, mImpl->mFlutterSurface->Id());
  return true;
}

uint32_t SurfaceGlOffscreen::GLContextFBO() const {
  if (!mImpl->mFlutterSurface && native_window_->IsValid())
  {
    mImpl->mFlutterSurface = std::make_unique<GlFrameBuffer>(native_window_->Width(), native_window_->Height());
    mImpl->mRenderer = std::make_unique<GlStereoRenderer>(native_window_->Width(), native_window_->Height());
  }
  ELINUX_LOG(ERROR) << "Request FBO for rendering : "<< mImpl->mFlutterSurface->Id() << " thread: "
                    <<std::this_thread::get_id()<<"\n";
  return mImpl->mFlutterSurface->Id();
}

void* SurfaceGlOffscreen::GlProcResolver(const char* name) const {
  return context_->GlProcResolver(name);
}

}  // namespace flutter

#ifndef FLUTTER_SHELL_PLATFORM_LINUX_EMBEDDED_RENDERING_STEREO_RENDERER_H_
#define FLUTTER_SHELL_PLATFORM_LINUX_EMBEDDED_RENDERING_STEREO_RENDERER_H_

#include <memory>

namespace flutter {

class GlStereoRenderer {
 public:
  GlStereoRenderer(size_t width, size_t height);
  ~GlStereoRenderer();
  void Render(uint32_t textureId);
 private:
  struct Impl;
  std::unique_ptr<GlStereoRenderer::Impl> mImpl;
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_LINUX_EMBEDDED_RENDERING_STEREO_RENDERER_H_

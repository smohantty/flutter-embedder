
#ifndef FLUTTER_SHELL_PLATFORM_LINUX_EMBEDDED_RENDERING_FRAME_BUFFER_H_
#define FLUTTER_SHELL_PLATFORM_LINUX_EMBEDDED_RENDERING_FRAME_BUFFER_H_

#include <memory>

namespace flutter {

class GlFrameBuffer {
 public:
  GlFrameBuffer(size_t width, size_t height);
  ~GlFrameBuffer();
  void Bind() const;
  uint32_t Id() const;
  uint32_t Texture();
 private:
  struct Impl;
  std::unique_ptr<GlFrameBuffer::Impl> mImpl;
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_LINUX_EMBEDDED_RENDERING_FRAME_BUFFER_H_

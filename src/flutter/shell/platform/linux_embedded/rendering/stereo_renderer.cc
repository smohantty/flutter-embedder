#include "flutter/shell/platform/linux_embedded/rendering/stereo_renderer.h"
#include "flutter/shell/platform/linux_embedded/rendering/gl_api.h"

#include <math/mat4.h>
#include <iostream>

namespace
{

constexpr const char* kVertexShader =
    R"glsl(
    uniform mat4 u_MVP;
    attribute vec2 a_Position;
    attribute vec2 a_TexCoords;
    varying vec2 v_TexCoords;

    void main() {
      gl_Position = u_MVP * vec4(a_Position, -1, 1);
      v_TexCoords = a_TexCoords;
    })glsl";

constexpr const char* kFragmentShader =
    R"glsl(
    precision mediump float;

    uniform sampler2D u_Texture;
    varying vec2 v_TexCoords;

    void main() {
      gl_FragColor = texture2D(u_Texture, vec2(1.0 -v_TexCoords.x, v_TexCoords.y));
    })glsl";

void CheckGlError(const char* label)
{
  auto &api = flutter::GlApi::instance();

  int gl_error = api.glGetError();
  if (gl_error != GL_NO_ERROR) {
    std::cout<<"GL error "<<label<<" , Error Code : "<<gl_error<<"\n";
  }
}

GLuint LoadShader(GLenum shader_type, const char* source)
{
  auto &api = flutter::GlApi::instance();
  GLuint shader = api.glCreateShader(shader_type);
  api.glShaderSource(shader, 1, &source, nullptr);
  api.glCompileShader(shader);
  CheckGlError("glCompileShader");
  GLint result = GL_FALSE;
  api.glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int log_length;
    api.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length == 0) {
      return 0;
    }

    std::vector<char> log_string(log_length);
    api.glGetShaderInfoLog(shader, log_length, nullptr, log_string.data());
    std::cout<<"Could not compile shader \n";
    // CARDBOARD_LOGE("Could not compile shader of type %d: %s", shader_type,
    //                log_string.data());

    shader = 0;
  }

  return shader;
}

GLuint CreateProgram(const char* vertex, const char* fragment)
{
  auto &api = flutter::GlApi::instance();
  GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, vertex);
  if (vertex_shader == 0) {
    return 0;
  }

  GLuint fragment_shader = LoadShader(GL_FRAGMENT_SHADER, fragment);
  if (fragment_shader == 0) {
    return 0;
  }

  GLuint program = api.glCreateProgram();

  api.glAttachShader(program, vertex_shader);
  api.glAttachShader(program, fragment_shader);
  api.glLinkProgram(program);
  CheckGlError("glLinkProgram");

  GLint result = GL_FALSE;
  api.glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (result == GL_FALSE) {
    int log_length;
    api.glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length == 0) {
      return 0;
    }

    std::vector<char> log_string(log_length);
    api.glGetShaderInfoLog(program, log_length, nullptr, log_string.data());
    std::cout<<"Could not compile program \n";
    //CARDBOARD_LOGE("Could not compile program: %s", log_string.data());

    return 0;
  }

  api.glDetachShader(program, vertex_shader);
  api.glDetachShader(program, fragment_shader);
  api.glDeleteShader(vertex_shader);
  api.glDeleteShader(fragment_shader);
  CheckGlError("GlCreateProgram");

  return program;
}

using Matrix = filament::math::mat4f;
using Float3 = filament::math::float3;
float BONDI_INTER_LENS_DISTANCE = 0.06f;

struct GlRectangle
{
    void Create() {
        auto& api = flutter::GlApi::instance();
        // Generate a vertex buffer
        api.glGenBuffers(1, &mVbo);

        // Define a VAO (Vertex Array Object)
        api.glGenVertexArrays(1, &mVao);
        api.glBindVertexArray(mVao);
        // Vertex buffer
        api.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        // Example vertex layout: vec4 vertex coordinates, vec2 texture coordinates
        // Vertex attrib 0: vertex coordinates
        api.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid *)0);
        api.glEnableVertexAttribArray(0);
        // Vertex attrib 1: texture coordinates
        api.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid *)(2 * sizeof(GLfloat)));
        api.glEnableVertexAttribArray(1);
        // Done
        api.glBindVertexArray(0);
    }
    void Destroy()
    {
        auto& api = flutter::GlApi::instance();
        //api.glDeleteVertexArrays(1, &mVao); mVao = 0;
        api.glDeleteBuffers(1, &mVbo); mVbo = 0;
    }

    struct vertex_data
    {
        GLfloat x, y, u, v;
    };

    void SetRect(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
    {
        auto& api = flutter::GlApi::instance();
        float vertices[16] = { 1.0f, -1.0f, 1, 0 ,
                               -1.0f, -1.0f, 0, 0 ,
                                1.0f, 1.0f, 1 , 1,
                                -1.0f, 1.0f, 0, 1
                              };
        api.glBindVertexArray(mVao);
        api.glBindBuffer( GL_ARRAY_BUFFER, mVbo);
        api.glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(vertex_data), vertices, GL_STATIC_DRAW);
    }
    void Draw()
    {
        auto& api = flutter::GlApi::instance();
        api.glBindVertexArray(mVao);
        api.glBindBuffer( GL_ARRAY_BUFFER, mVbo);
        api.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        api.glBindVertexArray(0);
    }

public:
    GLuint mVao{0};
    GLuint mVbo{0};
};

Matrix left_eye_matrix()
{
   Matrix m = Matrix::translation(Float3{BONDI_INTER_LENS_DISTANCE * 0.5f , 0.f, 0.f});
   return m;
}

Matrix right_eye_matrix()
{
   Matrix m = Matrix::translation(Float3{-BONDI_INTER_LENS_DISTANCE * 0.5f , 0.f, 0.f});
   return m;
}

Matrix left_eye_view(const Matrix& head_view)
{
   return left_eye_matrix() * head_view;
}

Matrix right_eye_view(const Matrix& head_view)
{
   return right_eye_matrix() * head_view;
}

//Struct representing a 3D mesh with 3D vertices and corresponding UV coordinates.
struct Mesh {
  /// Indices buffer.
  int* indices;
  /// Number of indices.
  int n_indices;
  /// Vertices buffer. 2 floats per vertex: x, y.
  float* vertices;
  /// UV coordinates buffer. 2 floats per uv: u, v.
  float* uvs;
  /// Number of vertices.
  int n_vertices;
};

Matrix view_matrix()
{
    Float3 eye = {0, 0, 0};
    Float3 center = {0, 1, 4};
    Float3 up = {0, 1, 0};
    return Matrix::lookAt(eye, center, up);
}
Matrix projection_matrix()
{
    auto fov = 40.f * filament::math::f::DEG_TO_RAD;
    Matrix projection_matrix = Matrix::perspective(fov, 1.0f, 0.01f, 100.f);
    return projection_matrix;
}

} // annonymous namespace

namespace flutter {

struct GlStereoRenderer::Impl
{

  Impl(size_t width, size_t height)
  :mWidth(width), mHeight(height)
  {

  }
    void Setup()
    {
        if (!mSetup)
        {
            mSetup = true;
            mProgram = CreateProgram(kVertexShader, kFragmentShader);
            std::cout<<"program Id : "<< mProgram<<"\n";
            mGlRect.Create();
        }
        mGlRect.SetRect(0, 0, 1, 1);
    }
  void RenderLeft()
  {

  }

  void RenderRight()
  {

  }
  void Render(uint32_t textureId)
  {
    auto& api = GlApi::instance();

    api.glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Setup();

    //api.glEnable(GL_DEPTH_TEST);
    // api.glEnable(GL_CULL_FACE);
    // api.glDisable(GL_SCISSOR_TEST);
    // api.glEnable(GL_BLEND);
    // api.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    api.glClear(GL_COLOR_BUFFER_BIT);

    api.glViewport(0,0, mWidth/2 ,mHeight);
    Float3 eye{0.f , 0.f, 0.f}, center{0, 0, 1}, up{0, 1, 0};
    Matrix lookat_matrix = filament::math::mat4f::lookAt (eye, center, up);

    auto model_matrix = Matrix::translation(Float3{0.0f, 0.0f, 1});
    auto mvp = lookat_matrix * model_matrix ;

    api.glUseProgram(mProgram);
    api.glActiveTexture(GL_TEXTURE0);
    api.glBindTexture(GL_TEXTURE_2D, textureId);

    api.glUniformMatrix4fv(api.glGetUniformLocation( mProgram, "u_MVP" ), 1, GL_FALSE,
                            mvp.raw_array());

    mGlRect.Draw();

    api.glViewport(mWidth/2,0, mWidth/2 ,mHeight);
    mGlRect.Draw();

    api.glViewport(0,0, mWidth ,mHeight);
    // return;



    // api.glViewport(0, 0, mWidth, mHeight);
    // api.glEnable(GL_BLEND);
    // api.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // api.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // api.glDisable(GL_DEPTH_TEST);
    // api.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // //api.glClearColor(0.f, 1.0f, .0f, 0.2f);
    // //api.glClear(GL_COLOR_BUFFER_BIT);

    // api.glUseProgram(mProgram);
    // api.glActiveTexture(GL_TEXTURE0);
    // api.glBindTexture(GL_TEXTURE_2D, textureId);


    // Matrix mvp = view_matrix()* Matrix::translation(Float3{0, 2, 0.9});
    // api.glUniformMatrix4fv(api.glGetUniformLocation( mProgram, "u_MVP" ), 1, GL_FALSE,
    //                         mvp.raw_array());
    // mGlRect.Draw();
  }
  size_t mWidth{0};
  size_t mHeight{0};
  GLuint mProgram;
  GlRectangle mGlRect;
  bool   mSetup{false};
};

GlStereoRenderer::GlStereoRenderer(size_t width, size_t height)
:mImpl(std::make_unique<GlStereoRenderer::Impl>(width, height))
{

}

GlStereoRenderer::~GlStereoRenderer() = default;

void GlStereoRenderer::Render(uint32_t textureId)
{
  mImpl->Render(textureId);
}

}  // namespace flutter

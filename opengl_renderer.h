#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include "renderer.h"
#include "opengl_common.h"

namespace OpenGL
{

	// OpenGL renderer implementation.
	class Implementation : public Renderer::Interface
	{

	public:

		Implementation();
		~Implementation();

		// Renderer interface functions.
		virtual bool Initialize();
		virtual void Shutdown();
		virtual void ClearScene();
		virtual void SetUniform(const Renderer::Uniform *uniform, float value);
		virtual void SetUniform(const Renderer::Uniform *uniform, const Vector3 *vector);
		virtual void SetUniform(const Renderer::Uniform *uniform, const Vector4 *vector);
		virtual void SetUniform(const Renderer::Uniform *uniform, const Matrix3x3 *matrix);
		virtual void SetUniform(const Renderer::Uniform *uniform, const Matrix4x4 *matrix);

		// Singleton retriever.
		static Implementation *GetInstance();

	private:

		// Singleton instance.
		static Implementation instance;

	};

}


int opengl_initialize(void);
void opengl_destroy(void);


// Renderer model functions.
int opengl_create_model(
	const void *vertex_data,
	int num_vertices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);
int opengl_create_indexed_model(
	const void *vertex_data,
	int num_vertices,
	const unsigned int *index_data,
	int num_indices,
	renderer_shader_schema_t schema,
	renderer_model_t *out);
void opengl_destroy_model(renderer_model_t *model);
void opengl_draw_model(renderer_model_t model, renderer_shader_schema_t schema);

// Renderer drawing functions.
void opengl_clear_scene(void);

#endif // _OPENGL_RENDERER_H_
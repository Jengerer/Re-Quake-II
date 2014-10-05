#pragma once

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
		virtual void SetProgram(const Renderer::Program *program);
		virtual void UnsetProgram(const Renderer::Program *program);
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
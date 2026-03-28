#pragma once

#include <cstdint>
#include <iostream>

namespace NocEngine
{
	struct GPU_Texture {
		uint32_t generation{};
		virtual ~GPU_Texture() = default;
		virtual void Destroy() = 0;
		virtual bool IsValid() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	struct OpenGL_Texture: public GPU_Texture {
		uint32_t texture_id{};
		~OpenGL_Texture() override {
			Destroy();
		}

		void Destroy() override {
			if (texture_id) {
				glDeleteTextures(1, &texture_id);
			}
			texture_id = 0;
		}

		bool IsValid() const override {
			return texture_id != 0;
		}

		void Bind() const override {
			Bind(0);
		}

		void Bind(GLuint unit = 0) const {
			if (!IsValid() || unit > GL_MAX_TEXTURE_IMAGE_UNITS) return;
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}

		void Unbind() const override {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	};
}
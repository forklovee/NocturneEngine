#pragma once

#include "handle.h"

namespace NocEngine 
{
	// Class representing material data from external text file
	class Material : public IResource
	{
	public:
		Material(const char* file_path)
			: IResource()
		{
		};

		Material(const Material& other)
			: IResource(other)
		{
			AlbedoTexture = other.AlbedoTexture;
			AmbientColor = other.AmbientColor;
			DiffuseColor = other.DiffuseColor;
			SpecularColor = other.SpecularColor;
			Roughness = other.Roughness;
		};

		bool IsLoaded() const override {
			return true;
		}

		void Load() override {
		
		}

		void Unload() override {
		
		}

	public:
		Handle<Texture> AlbedoTexture{};
		glm::vec3 AmbientColor{ 1.f };
		glm::vec3 DiffuseColor{ 1.f };
		glm::vec3 SpecularColor{ 1.f };
		float Roughness{ 1.f };

	};
}
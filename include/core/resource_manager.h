#pragma once

#include <type_traits>
#include <string>
#include "handle.h"
#include "resource_container.h"
#include "texture.h"
#include "meshdata.h"
#include "material.h"

namespace NocEngine
{
	using FilePath = std::string;

	class ResourceManager
	{
	public:
		static ResourceManager& Get() {
			static ResourceManager instance;
			return instance;
		}

		template<ResourceType T>
		Handle<T> Load(const FilePath& file_path, bool autoload_asset = true) {
			return getResourceContainer<T>().Load(file_path, autoload_asset);
		}

		template<ResourceType T>
		T* Get(const Handle<T>& handle) {
			return getResourceContainer<T>().Get(handle);
		}

	private:
		ResourceManager()
			: m_meshContainer(m_resourcePathPool),
			m_texturesContainer(m_resourcePathPool),
			m_materialsContainer(m_resourcePathPool)
		{};

		template<ResourceType T>
		ResourceContainer<T>& getResourceContainer();

	private:
		StringPool m_resourcePathPool{};
		ResourceContainer<MeshData> m_meshContainer;
		ResourceContainer<Texture> m_texturesContainer;
		ResourceContainer<Material> m_materialsContainer;
	};

	template<>
	inline ResourceContainer<MeshData>& ResourceManager::getResourceContainer() {
		return m_meshContainer;
	}

	template<>
	inline ResourceContainer<Texture>& ResourceManager::getResourceContainer() {
		return m_texturesContainer;
	}

	template<>
	inline ResourceContainer<Material>& ResourceManager::getResourceContainer() {
		return m_materialsContainer;
	}
}
#pragma once

#include <cstdint>

namespace NocEngine
{
	class IResource
	{
	public:
		IResource()
			: m_filePath("")
		{};
		IResource(const char* filePath)
			: m_filePath(filePath)
		{};
		virtual ~IResource() = default;

		IResource(const IResource& other)
			:m_filePath(other.m_filePath), m_generation(other.m_generation)
		{
		}

		IResource(IResource&& other) noexcept
			: m_filePath(other.m_filePath), m_generation(other.m_generation)
		{
			other.m_filePath = nullptr;
		};

		IResource& operator=(const IResource& other) = delete;
		IResource& operator=(IResource&& other) noexcept {
			m_filePath = other.m_filePath;
			m_generation = other.m_generation;
			other.m_filePath = nullptr;
			return *this;
		}

		virtual bool IsLoaded() const = 0;
		virtual void Load() = 0;
		virtual void Unload() = 0;

		uint32_t GetGeneration() const {
			return m_generation;
		}

	protected:
		const char* m_filePath;
		uint32_t m_generation{};
	};
}
#if _WIN32
#include "resource.h"

#define NOMINMAX
#include <cstddef>
#include <windows.h>

namespace kanji::system
{
	class ResourceImpl
	{
	public:
		explicit ResourceImpl(int resource_id)
		{
			auto module = GetModuleHandle(nullptr);

			auto resource = FindResource(module, MAKEINTRESOURCE(resource_id), RT_RCDATA);
			if (!resource)
			{
				return;
			}

			resource_handle = LoadResource(module, resource);
			if (!resource_handle)
			{
				return;
			}

			data = LockResource(resource_handle);
			if (!data)
			{
				return;
			}

			size = SizeofResource(module, resource);
			if (size == 0)
			{
				return;
			}
		}

		HGLOBAL resource_handle = nullptr;
		void* data = nullptr;
		std::size_t size = 0;
	};

	Resource::Resource(int resource_id)
	{
		impl = new ResourceImpl(resource_id);
	}

	Resource::~Resource()
	{
		delete impl;
	}

	void* Resource::GetData() const
	{
		return impl->data;
	}

	std::size_t Resource::GetSize() const
	{
		return impl->size;
	}
} // namespace kanji::system
#endif
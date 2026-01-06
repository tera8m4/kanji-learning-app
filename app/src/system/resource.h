#pragma once
#include <cstddef>

namespace kanji::system
{
	class ResourceImpl;

	class Resource
	{
	public:
		explicit Resource(int resource_id);
		~Resource();

		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;

		Resource(Resource&&) = delete;
		Resource& operator=(Resource&&) = delete;

		void* GetData() const;
		std::size_t GetSize() const;

	private:
		ResourceImpl* impl;
	};
} // namespace kanji::system

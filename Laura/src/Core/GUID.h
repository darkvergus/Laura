#pragma once
#include "lrpch.h"

namespace Laura
{
	// Globally Unique IDentifier
	// A wrapper for a uint64_t
	class GUID
	{
	public:
		GUID(); // constructs a random uint64_t GUID
		GUID(uint64_t guid);
		GUID(const GUID& other); // copy constructor
		operator uint64_t() const { return m_GUID; } // uint64_t conversion operator 
		~GUID() = default;
	private:
		uint64_t m_GUID;
	};
}


// Hashing specialization for GUIDs for use in std::unordered_map ...
namespace std
{
	template<>
	struct hash<Laura::GUID>
	{
		std::size_t operator()(const Laura::GUID& guid) const
		{
			return std::hash<uint64_t>()((uint64_t)guid);
		}
	};
}
#pragma once

#include <random>
#include <sstream>

namespace QuasarEngine {
    class UUID
    {
    public:
        UUID();
        UUID(uint32_t uuid);
        UUID(const UUID&) = default;

        static UUID Null() { return UUID(0); }

        operator uint32_t() const { return m_UUID; }

        bool operator==(const UUID& other) const { return m_UUID == other.m_UUID; }
        bool operator!=(const UUID& other) const { return m_UUID != other.m_UUID; }

        std::string ToString() const
        {
			std::stringstream ss;
			ss << m_UUID;
			return ss.str();
		}
    private:
        uint32_t m_UUID;
    };

    struct UUIDMapping
    {
        size_t operator()(const UUID& k)const
        {
            return std::hash<int>()(k);
        }

        bool operator()(const UUID& a, const UUID& b)const
        {
            return a == b;
        }
    };
}

namespace std {
    template <typename T> struct hash;

    template<>
    struct hash<QuasarEngine::UUID>
    {
        std::size_t operator()(const QuasarEngine::UUID& uuid) const
        {
            return (uint32_t)uuid;
        }
    };

}
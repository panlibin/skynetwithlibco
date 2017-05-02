#include "arguments.h"

namespace csn
{
	template<>
	int8_t Arguments::get<int8_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Int8);
			return base.m_value.i8;
		}
		return 0;
	}

	template<>
	int16_t Arguments::get<int16_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Int16);
			return base.m_value.i16;
		}
		return 0;
	}

	template<>
	int32_t Arguments::get<int32_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Int32);
			return base.m_value.i32;
		}
		return 0;
	}

	template<>
	int64_t Arguments::get<int64_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Int64);
			return base.m_value.i64;
		}
		return 0;
	}

	template<>
	uint8_t Arguments::get<uint8_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_UInt8);
			return base.m_value.u8;
		}
		return 0;
	}

	template<>
	uint16_t Arguments::get<uint16_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_UInt16);
			return base.m_value.u16;
		}
		return 0;
	}

	template<>
	uint32_t Arguments::get<uint32_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_UInt32);
			return base.m_value.u32;
		}
		return 0;
	}

	template<>
	uint64_t Arguments::get<uint64_t>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_UInt64);
			return base.m_value.u64;
		}
		return 0;
	}

	template<>
	float Arguments::get<float>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Float32);
			return base.m_value.f32;
		}
		return 0.0f;
	}

	template<>
	double Arguments::get<double>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Float64);
			return base.m_value.f64;
		}
		return 0.0;
	}

	template<>
	bool Arguments::get<bool>(uint32_t nIdx)
	{
		if (nIdx < m_vecArgument.size())
		{
			BaseType& base = m_vecArgument[nIdx];
			assert(base.m_type == eArg_Bool);
			return base.m_value.b;
		}
		return false;
	}
}

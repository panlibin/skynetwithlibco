#pragma once

#include "../common/common.h"
#include <vector>
#include <string>
#include <memory>
#include <assert.h>

namespace csn
{
    class ArgumentStub
    {
    public:
        virtual ~ArgumentStub() {}
    };

    template<typename T>
    class Argument : public ArgumentStub
    {
    public:
        virtual ~Argument() {}

        Argument(T&& arg)
        {
            m_arg = arg;
        }

        T& getValue()
        {
            return m_arg;
        }

    private:
        T m_arg;
    };

    class Arguments
    {
    public:
        Arguments()
        {
            m_vecArgument.reserve(10);
        }

        Arguments(int32_t nSize)
        {
            m_vecArgument.reserve(nSize);
        }

        ~Arguments()
        {

        }
        
        uint64_t size() const
        {
            return m_vecArgument.size();
        }
        
        void clear()
        {
            m_vecArgument.clear();
            m_vecUserData.clear();
        }

        template<typename T>
        int32_t push(T&& arg)
        {
            BaseType base;
            base.m_type = eArg_UserData;
            base.m_value.u64 = m_vecUserData.size();
            m_vecArgument.push_back(base);
            m_vecUserData.push_back(std::make_shared<Argument<T>>(std::forward<T>(arg)));
            return 0;
        }

        template<typename T>
        int32_t push(T& arg)
        {
            BaseType base;
            base.m_type = eArg_UserData;
            base.m_value.u64 = m_vecUserData.size();
            m_vecArgument.push_back(base);
            m_vecUserData.push_back(std::make_shared<Argument<T>>(std::forward<T>(arg)));
            return 0;
        }

        int32_t push(int8_t arg)
        {
            BaseType base;
            base.m_type = eArg_Int8;
            base.m_value.i8 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(int16_t arg)
        {
            BaseType base;
            base.m_type = eArg_Int16;
            base.m_value.i16 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(int32_t arg)
        {
            BaseType base;
            base.m_type = eArg_Int32;
            base.m_value.i32 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }
        
        int32_t push(long arg)
        {
            push(static_cast<long long>(arg));
            return 0;
        }

        int32_t push(long long arg)
        {
            BaseType base;
            base.m_type = eArg_Int64;
            base.m_value.i64 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(uint8_t arg)
        {
            BaseType base;
            base.m_type = eArg_UInt8;
            base.m_value.u8 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(uint16_t arg)
        {
            BaseType base;
            base.m_type = eArg_UInt16;
            base.m_value.u16 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(uint32_t arg)
        {
            BaseType base;
            base.m_type = eArg_UInt32;
            base.m_value.u32 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }
        
        int32_t push(unsigned long arg)
        {
            push(static_cast<unsigned long long>(arg));
            return 0;
        }

        int32_t push(unsigned long long arg)
        {
            BaseType base;
            base.m_type = eArg_UInt64;
            base.m_value.u64 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(float arg)
        {
            BaseType base;
            base.m_type = eArg_Float32;
            base.m_value.f32 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(double arg)
        {
            BaseType base;
            base.m_type = eArg_Float64;
            base.m_value.f64 = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(bool arg)
        {
            BaseType base;
            base.m_type = eArg_Bool;
            base.m_value.b = arg;
            m_vecArgument.push_back(base);
            return 0;
        }

        int32_t push(const char* arg)
        {
            push(std::string(arg));
            return 0;
        }

        template<typename T>
        T get(uint32_t nIdx)
        {
            assert(nIdx < m_vecArgument.size());
            BaseType& base = m_vecArgument[nIdx];
            assert(base.m_type == eArg_UserData && base.m_value.u64 < m_vecUserData.size());
            std::shared_ptr<ArgumentStub> pUd = m_vecUserData[base.m_value.u64];
            Argument<T>* pRes = dynamic_cast<Argument<T>*>(pUd.get());
            return pRes->getValue();
        }

    private:
        enum eArgumentType
        {
            eArg_Undefine = 0x0,
            eArg_Int8 = 0x10,
            eArg_Int16 = 0x11,
            eArg_Int32 = 0x12,
            eArg_Int64 = 0x13,
            eArg_UInt8 = 0x20,
            eArg_UInt16 = 0x21,
            eArg_UInt32 = 0x22,
            eArg_UInt64 = 0x23,
            eArg_Float32 = 0x30,
            eArg_Float64 = 0x31,
            eArg_Bool = 0x40,
            eArg_UserData = 0x50,
        };

        union ArgumentValue
        {
            int8_t i8;
            int16_t i16;
            int32_t i32;
            int64_t i64;
            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64;
            float f32;
            double f64;
            bool b;
        };

        struct BaseType
        {
            eArgumentType m_type;
            ArgumentValue m_value;

            BaseType()
            {
                m_type = eArg_Undefine;
                m_value.u64 = 0;
            }
        };

        std::vector<BaseType> m_vecArgument;
        typedef std::shared_ptr<ArgumentStub> ArgumentPtr;
        std::vector<ArgumentPtr> m_vecUserData;
    };
}

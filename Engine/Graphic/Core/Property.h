// Copyright 2023 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @note Property Class
 * Use a series of Property classes to collect class property info.
 * Property has
 *  owner,
 *  nameCrc,
 *  type,
 *  offset
*/

#pragma once
#include "System.h"
#include "MemManager.h"
#include "Stream/Stream.h"
#include "Type.marc"

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>


namespace NoelleGraphic
{
    class ClassInfo;

    class Property
    {
    public:
        enum // Property type 
        {
            PT_VALUE, // Others
            PT_ENUM,
            PT_DATA,  // Pointer to store data
            PT_ARRAY,
            PT_MAP,
            PT_MAX
        };

        enum  // Flag
        {
            F_SAVE_LOAD = 0X01,
            F_CLONE = 0X02,
            F_COPY = 0X04,
            F_SAVE_LOAD_CLONE = 0X03,
            F_SAVE_LOAD_COPY = 0X05,
            F_REFLECT_NAME = 0X08,
            F_MAX
        };
    public:
        Property()
        {
            m_pOwner = nullptr;
            m_uiFlag = 0;
            m_uiOffset = 0;
        }

        Property(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset)
        : m_pOwner(&pOwner), m_crcName(nameCrc), m_uiFlag(uiFlag), m_uiOffset(uiOffset)
        {
        }

        
        Property(const Property&) = delete;
        Property& operator=(const Property&) = delete;
        Property(Property&&) = default;
        Property& operator=(Property&&) = default;
        virtual ~Property() = default;

        virtual bool Clone(const Property* property)
        {
            if (property->GetType() == GetType() && GetType())
            {
                m_pOwner = property->m_pOwner;
                m_crcName = property->m_crcName;
                m_uiFlag = property->m_uiFlag;
                m_uiOffset = property->m_uiOffset;
                return true;
            }
            return false;
        }

        inline const StringCrc GetName() const
        {
            return m_crcName;
        }

        inline ClassInfo* GetType() const
        {
            return m_pOwner;
        }

        unsigned int GetFlag()
        {
            return m_uiFlag;
        }

        void SetOwner(ClassInfo& pOwner)
        {
            m_pOwner = &pOwner;
        }

        void SetName(const StringCrc nameCrc)
        {
            m_crcName = m_crcName;
        }

        void SetFlag(unsigned int uiFlag)
        {
            m_uiFlag = uiFlag;
        }

        virtual bool Archive(BinaryStream& stream, void* pObject)
        {
            return true;
        }

        virtual void* GetValueAddress(void* pObj) const
        {
            return (void*)(((unsigned char*)pObj) + m_uiOffset); 
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return nullptr;
        }

    protected:
        ClassInfo* m_pOwner;
        StringCrc m_crcName;
        unsigned int m_uiFlag;
        unsigned int m_uiOffset;
    };

    template<typename T>
    class EnumProperty : public Property
    {
    public:
        EnumProperty()
        {
        }

        EnumProperty(ClassInfo& pOwner, const StringCrc& nameCrc, const StringCrc& enumName, unsigned int uiFlag, unsigned int uiOffset)
        : Property(pOwner, nameCrc, uiFlag, uiOffset), m_EnumName(enumName)
        {
        }

        virtual ~EnumProperty()
        {
        }

        EnumProperty(const EnumProperty&) = delete;
        EnumProperty(EnumProperty&&) = default;
        EnumProperty& operator=(const EnumProperty&) = delete;
        EnumProperty& operator=(EnumProperty&&) = default;

        virtual bool Clone(const Property* property)
        {
            EnumProperty* tmp = (EnumProperty*)property;
            if (Property::Clone(tmp))
            {
                m_EnumName = tmp->m_EnumName;
                return true;
            }
            return false;
        }

        virtual bool SetValue(void* pObj, T& pDataSrc) const
        {
            *(T*)(((unsigned char*)pObj) + m_uiOffset) = pDataSrc;
            return true;
        }

        virtual bool GetValue(void* pObj, T& pDataSrc) const
        {
            pDataSrc = *(T*)(((unsigned char*)pObj) + m_uiOffset);
            return true;
        }

        virtual bool GetValue(const void* pObj, T& pDataSrc) const
        {
            pDataSrc = *(T*)(((const unsigned char*)pObj) + m_uiOffset);
            return true;
        }

        virtual T& Value(void *pObj) const
        {
            return *(T*)(((const unsigned char*)pObj) + m_uiOffset);
        }

        virtual bool Archive(BinaryStream& stream, void* pObject) override
        {
            stream.Archive(Value(pObject));
            return true;
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return std::make_unique<EnumProperty<T>>();
        }

    private:
        StringCrc m_EnumName;
    };

    template<typename T, typename NumType>
    class DataProperty: public Property
    {
    public:
        DataProperty()
        {
        }

        DataProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiOffset, bool bDynamicCreate, unsigned int uiDataNum)
        : Property(pOwner, nameCrc, F_SAVE_LOAD_CLONE, uiOffset), m_bDynamicCreate(bDynamicCreate), m_uiDataNum(uiDataNum)
        {
        }

        DataProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiOffset, unsigned int uiNumOffset)
        : Property(pOwner, nameCrc, F_SAVE_LOAD_CLONE, uiOffset), m_uiNumOffset(uiNumOffset), m_bDynamicCreate(true), m_uiDataNum(0)
        {
        }

        virtual ~DataProperty()
        {

        }

        DataProperty(const DataProperty&) = delete;
        DataProperty(DataProperty&&) = default;
        DataProperty& operator=(const DataProperty&) = delete;
        DataProperty& operator=(DataProperty&&) = default;

        virtual bool Clone(const Property* property)
        {
            DataProperty<T, NumType>* tmp = (DataProperty<T, NumType>*)property;
            if (Property::Clone(tmp))
            {
                m_bDynamicCreate = tmp->m_bDynamicCreate;
                m_uiDataNum = tmp->m_uiDataNum;
                m_uiNumOffset = tmp->m_uiNumOffset;
                return true;
            }
            return false;
        }

        virtual bool Archive(BinaryStream& stream, void* pObject) override
        {
            uint32_t archiveType = stream.GetStreamFlag();
            switch (archiveType)
            {
            case BinaryStream::AT_SAVE:
            {
                T* valueAddress = *(T**)GetValueAddress(pObject);  // Indirect addressing

                if (m_uiDataNum > 0)  // static array
                {
                    stream.Write(valueAddress, m_uiDataNum * sizeof(T));
                }
                else  // dynamic array
                {
                    void* numOffset = (void*)(((unsigned char*)pObject) + m_uiNumOffset);
                    NumType uiDataNum = *(NumType*)numOffset;
                    stream.Write(numOffset, sizeof(NumType));
                    stream.Write(valueAddress, uiDataNum * sizeof(T));
                }
            }
            break;
            case BinaryStream::AT_LOAD:
            {
                T** pTmp = (T**)GetValueAddress(pObject);
                if (m_uiDataNum > 0)  // static array
                {
                    if (m_bDynamicCreate)
                    {
                        *pTmp = new T[m_uiDataNum];
                        stream.Read((void*)(*pTmp), m_uiDataNum * sizeof(T));
                    }
                    else
                    {
                        stream.Read((void*)(*pTmp), m_uiDataNum * sizeof(T));
                    }
                }
                else
                {
                    void* numOffset = (void*)(((unsigned char*)pObject) + m_uiNumOffset);
                    stream.Read(numOffset, sizeof(NumType));
                    NumType uiDataNum = *(NumType*)numOffset;
                    if (uiDataNum)
                    {
                        *pTmp = new T[uiDataNum];
                        stream.Read((void*)(*pTmp), uiDataNum * sizeof(T));
                    }
                }
            }
            break;
            case BinaryStream::AT_SIZE:
            {
                if (m_uiDataNum > 0)
                {
                    stream.AddBufferSize(m_uiDataNum * sizeof(T));
                }
                else
                {
                    void* numOffset = (void*)(((unsigned char*)pObject) + m_uiNumOffset);
                    NumType uiDataNum = *(NumType*)numOffset;

                    stream.AddBufferSize(sizeof(NumType));
                    stream.AddBufferSize(uiDataNum * sizeof(T));
                }
            }
            break;
            }

            return true;
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return std::unique_ptr<DataProperty<T, NumType>>();
        }
        
    private:
        bool m_bDynamicCreate;
        unsigned int m_uiDataNum;
        unsigned int m_uiNumOffset;
    };

    template<typename T>
    class RangeProperty : public Property
    {
    public:
        RangeProperty()
        {
        }

        RangeProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, bool bRange = false, T lowValue = T(), T highValue = T(), T step = T())
        : Property(pOwner, nameCrc, F_SAVE_LOAD_CLONE, uiOffset)
        {
            m_LowValue = lowValue;
            m_HighValue = highValue;
            m_Step = step;
            m_bRange = bRange;
        }

        virtual ~RangeProperty()
        {
        }

        RangeProperty(const RangeProperty&) = delete;
        RangeProperty(RangeProperty&&) = default;
        RangeProperty& operator=(const RangeProperty&) = delete;
        RangeProperty& operator=(RangeProperty&&) = default;

        virtual bool Clone(const Property* property)
        {
            RangeProperty<T>* tmp = (RangeProperty<T>*)(property);
            if (Property::Clone(tmp))
            {
                m_LowValue = tmp->m_LowValue;
                m_HighValue = tmp->m_HighValue;
                m_Step = tmp->m_Step;
                m_bRange = tmp->m_bRange;
                tmp = nullptr;
                return true;
            }
            tmp = nullptr;
            return false;
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return std::make_unique<RangeProperty<T>>();
        }

    protected:
        T m_LowValue;
        T m_HighValue;
        T m_Step;
        bool m_bRange;
    };

    template<typename T>
    class ValueProperty : public RangeProperty<T>
    {
    public:
        ValueProperty()
        {
        }

        ValueProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, bool bRange = false, T lowValue = T(), T highValue = T(), T step = T())
        :RangeProperty<T>(pOwner, nameCrc, uiFlag, uiOffset, bRange, lowValue, highValue, step)
        {
        }

        virtual ~ValueProperty()
        {
        }

        ValueProperty(const ValueProperty&) = delete;
        ValueProperty(ValueProperty&&) = default;
        ValueProperty& operator=(const ValueProperty&) = delete;
        ValueProperty& operator=(ValueProperty&&) = default;

        template <typename V = T>
        std::enable_if_t<T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, T& srcValue)
        {
            if (srcValue < this->m_LowValue || srcValue > this->m_HighValue)
            {
                return false;
            }
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = srcValue;
            return true;
        }

        template <typename V = T>
        std::enable_if_t<!T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, T& srcValue)
        {
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = srcValue;
            return true;
        }

        virtual bool GetValue(void* pObj, T& dstValue) const
        {
            dstValue = *(T*)(((unsigned char*)pObj) + this->m_uiOffset);
            return true;
        }

        virtual bool GetValue(const void* pObj, T& dstValue) const
        {
            dstValue = *(const T*)(((const unsigned char*)pObj) + this->m_uiOffset);
            return true;
        }

        virtual T& Value(void *pObj) const
        {
            return *(T*)(((const unsigned char*)pObj) + this->m_uiOffset);
        }

        virtual bool Archive(BinaryStream& stream, void* pObject) override
        {
            stream.Archive(Value(pObject));
            return true;
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return std::make_unique<ValueProperty<T>>();
        }
    };

    template<typename U>
    class ValueProperty<std::unique_ptr<U>> : public RangeProperty<U> {
    public:
        using T = std::unique_ptr<U>;

        ValueProperty() = default;

        ValueProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, bool bRange = false, U lowValue = U(), U highValue = U(), U step = U())
        : RangeProperty<U>(pOwner, nameCrc, uiFlag, uiOffset, bRange, lowValue, highValue, step) 
        {
        }

        virtual ~ValueProperty() = default;

        template <typename V = U>
        std::enable_if_t<T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, T& srcValue) 
        {
            if ((*srcValue) < this->m_LowValue || (*srcValue) > this->m_HighValue)
            {
                return false;
            }
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::move(srcValue);
            return true;
        }

        template <typename V = U>
        std::enable_if_t<!T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, T& srcValue) 
        {
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::move(srcValue);
            return true;
        }

        template <typename V = U>
        std::enable_if_t<T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, U& srcValue)
        {
            if (srcValue < this->m_LowValue || srcValue > this->m_HighValue)
            {
                return false;
            }
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::make_shared<U>(std::move(srcValue));
            return true;
        }

        template <typename V = U>
        std::enable_if_t<!T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, U& srcValue)
        { 
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::make_shared<U>(std::move(srcValue));
            return true;
        }

        virtual bool GetValue(void* pObj, T& dstValue) const
        {
            return false;
        }

        virtual bool GetValue(void* pObj, U* dstValue) const
        {
            dstValue = (*(T*)(((unsigned char*)pObj) + this->m_uiOffset)).get();
            return true;
        }

        virtual T& Value(void *pObj) const
        {
            return *(T*)(((const unsigned char*)pObj) + this->m_uiOffset);
        }

        virtual bool Archive(BinaryStream& stream, void* pObject)  override
        {
            stream.Archive(Value(pObject));
            return true;
        }
    };

    template<typename U>
    class ValueProperty<std::shared_ptr<U>> : public RangeProperty<U> {
    public:
        using T = std::shared_ptr<U>;

        ValueProperty() = default;

        ValueProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, bool bRange = false, U lowValue = U(), U highValue = U(), U step = U())
            : RangeProperty<U>(pOwner, nameCrc, uiFlag, uiOffset, bRange, lowValue, highValue, step) {
        }

        virtual ~ValueProperty() = default;

        template <typename V = U>
        std::enable_if_t<T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, T& srcValue)
        {
            if (*srcValue < this->m_LowValue || *srcValue > this->m_HighValue)
            {
                return false;
            }
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::move(srcValue);
            return true;
        }

        template <typename V = U>
        std::enable_if_t<!T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, T& srcValue)
        {
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::move(srcValue);
            return true;
        }

        template <typename V = U>
        std::enable_if_t<T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, U& srcValue)
        {
            if (srcValue < this->m_LowValue || srcValue > this->m_HighValue)
            {
                return false;
            }
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::make_unique<U>(std::move(srcValue));
            return true;
        }

        template <typename V = U>
        std::enable_if_t<!T_Has_Comparison_Operators<V>::Value, bool> SetValue(void* pObj, U& srcValue)
        {
            *(T*)(((unsigned char*)pObj) + this->m_uiOffset) = std::make_unique<U>(std::move(srcValue));
            return true;
        }

        virtual bool GetValue(void* pObj, T& dstValue) const
        {
            return false;
        }

        virtual bool GetValue(void* pObj, U* dstValue) const
        {
            dstValue = (*(T*)(((unsigned char*)pObj) + this->m_uiOffset)).get();
            return true;
        }

        virtual T& Value(void *pObj) const
        {
            return *(T*)(((const unsigned char*)pObj) + this->m_uiOffset);
        }

        virtual bool Archive(BinaryStream& stream, void* pObject)  override
        {
            stream.Archive(Value(pObject));
            return true;
        }
    };

    template<typename ArrayType, typename T>
    class ArrayProperty : public RangeProperty<T>
    {
    public:
        ArrayProperty()
        {
        }

        ArrayProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, T lowValue = T(), T highValue = T(), T step = T(), bool bRange = false)
        : RangeProperty<T>(pOwner, nameCrc, uiFlag, uiOffset, lowValue, highValue, step, bRange)
        {

        }

        virtual ~ArrayProperty()
        {
        }

        ArrayProperty(const ArrayProperty&) = delete;
        ArrayProperty(ArrayProperty&&) = default;
        ArrayProperty& operator=(const ArrayProperty&) = delete;
        ArrayProperty& operator=(ArrayProperty&&) = default;

        inline ArrayType& GetContainer(void* pObj) const
        {
            return *(ArrayType*)(((const unsigned char*)pObj) + this->m_uiOffset);
        }

        inline bool AddElement(void* pObj, unsigned int uiIndex, T& pDataSrc)
        {
            ArrayType& arr = GetContainer(pObj);
            arr.insert(arr.begin() + uiIndex);
            return true;
        } 

        inline bool Erase(void* pObj, unsigned int uiIndex)
        {
            ArrayType& arr = GetContainer(pObj);
            arr.erase(arr.begin() + uiIndex);
            return true;
        }

        virtual bool SetValue(void* pObj, unsigned int uiIndex, T& pDataSrc)
        {
            if (T_Has_Comparison_Operators<T>::Value)
            {
                if (pDataSrc < this->m_LowValue || pDataSrc > this->m_HighValue)
                {
                    return false;
                }
            }
            GetContainer(pObj)[uiIndex] = pDataSrc;
            return true;
        }

        virtual bool GetValue(void* pObj, unsigned int uiIndex, T& pDataDest) const
		{
			pDataDest = (GetContainer(pObj)[uiIndex]);
			return true;
		}

        virtual bool Archive(BinaryStream& stream, void* pObject) override
        {
            stream.Archive(GetContainer(pObject));
            return true;
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return std::make_unique<ArrayProperty<ArrayType, T>>();
        }
    };

    template<typename MapType, typename KeyType, typename ValueType>
    class MapProperty : public RangeProperty<ValueType>
    {
    public:
        MapProperty()
        {
        }

        MapProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, ValueType lowValue = ValueType(), ValueType highValue = ValueType(), ValueType step = ValueType(), bool bRange = false)
        : RangeProperty<ValueType>(pOwner, nameCrc, uiFlag, uiOffset, lowValue, highValue, step, bRange)
        {
        }

        virtual ~MapProperty()
        {

        }

        MapProperty(const MapProperty&) = delete;
        MapProperty(MapProperty&&) = default;
        MapProperty& operator=(const MapProperty&) = delete;
        MapProperty& operator=(MapProperty&&) = default;

        inline MapType& GetContainer(void* pObj) const
        {
            return *(MapType*)(((const unsigned char*)pObj) + this->m_uiOffset);
        }

        inline bool AddElement(void* pObj, unsigned int uiIndex, std::pair<KeyType, ValueType>& pDataSrc)
        {
            MapType& container = GetContainer(pObj);
            container.insert(container.begin() + uiIndex, pDataSrc);
            return true;
        }

        inline bool Erase(void* pObj, unsigned int uiIndex)
        {
            MapType& container = GetContainer(pObj);
            container.erase(container.begin() + uiIndex);
            return true;
        }

        virtual bool SetValue(void* pObj, KeyType& key, ValueType& value)
        {
            if (T_Has_Comparison_Operators<ValueType>::Value)
            {
                if (value < this->m_LowValue || value > this->m_HighValue)
                    return false;
            }
            MapType& container = GetContainer(pObj);
            container[key] = value;
            return true;
        }

        virtual bool GetValue(void* pObj, KeyType& key, ValueType& value)
        {
            MapType& container = GetContainer(pObj);
            if (container.count(key) != 0)
            {
                value = container[key];
                return true;
            }
            else
            {
                return false;
            }
        }

        virtual bool Archive(BinaryStream& stream, void* pObject) override
        {
            stream.Archive(GetContainer(pObject));
            return true;
        }

        virtual std::unique_ptr<Property> GetInstance()
        {
            return std::make_unique<MapProperty<MapType, KeyType, ValueType>>();
        }
    };

    template<typename T, typename NumType>
    class DataPropertyCreator
    {
    public:
        std::unique_ptr<DataProperty<T, NumType>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiOffset, bool bDynamicCreate, unsigned int uiDataNum)
        {
            return std::make_unique<DataProperty<T, NumType>>(pOwner, nameCrc, uiOffset, bDynamicCreate, uiDataNum);
        }

        std::unique_ptr<DataProperty<T, NumType>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiOffset, unsigned int uiNumOffset)
        {
            return std::make_unique<DataProperty<T, NumType>>(pOwner, nameCrc, uiOffset, uiNumOffset);
        }
    };

    template<typename T>
    class AutoPropertyCreator
    {
    public:
        std::unique_ptr<ValueProperty<T>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset)
        {
            return std::make_unique<ValueProperty<T>>(pOwner, nameCrc, uiFlag, uiOffset);
        }

        std::unique_ptr<ValueProperty<T>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, T lowValue, T highValue, T step)
        {
            return std::make_unique<ValueProperty<T>>(pOwner, nameCrc, uiFlag, uiOffset, lowValue, highValue, step);
        }
    };

    template<typename T, typename Alloc>
    class AutoPropertyCreator<std::vector<T, Alloc>>
    {
    public:
        std::unique_ptr<ArrayProperty<std::vector<T, Alloc>, T>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset)
        {
            return std::make_unique<ArrayProperty<std::vector<T, Alloc>, T>>(pOwner, nameCrc, uiFlag, uiOffset);
        }

        std::unique_ptr<ArrayProperty<std::vector<T, Alloc>, T>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, T lowValue, T highValue, T step)
        {
            return std::make_unique<ArrayProperty<std::vector<T, Alloc>, T>>(pOwner, nameCrc, uiFlag, uiOffset, lowValue, highValue, step);
        }
    };

    template<typename KeyType, typename ValueType, typename Alloc>
    class AutoPropertyCreator<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>>
    {
    public:
        std::unique_ptr<MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset)
        {
            return std::make_unique<MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>>(pOwner, nameCrc, uiFlag, uiOffset);
        }

        std::unique_ptr<MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>> CreateProperty(ClassInfo& pOwner, const StringCrc& nameCrc, unsigned int uiFlag, unsigned int uiOffset, ValueType lowValue, ValueType highValue, ValueType step)
        {
            return std::make_unique<MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>>(pOwner, nameCrc, uiFlag, uiOffset, lowValue, highValue, step);
        }
    };

    class PropertyCreator
    {
    public:
		template<class ValueType>
		static AutoPropertyCreator<ValueType>& GetAutoPropertyCreator(ValueType& valueTypeDummyRef)
		{
			static AutoPropertyCreator<ValueType> apc;
			return apc;
		}
		template<class ValueType,class NumType>
		static DataPropertyCreator<ValueType,NumType>& GetAutoPropertyCreator(ValueType*& valueTypeDummyRef, NumType& valueNumTypeDummyRef)
		{
			static DataPropertyCreator<ValueType,NumType> apc;
			return apc;
		}
		template<class ValueType>
		static std::unique_ptr<EnumProperty<ValueType>> CreateEnumProperty(ValueType& valueTypeDummyRef, const StringCrc& Name, const StringCrc& EnumName, ClassInfo& pOwner, unsigned int uiFlag, unsigned int uiOffset)
		{
			return std::make_unique<EnumProperty<ValueType>>(pOwner, Name, EnumName, uiFlag, uiOffset);
		}
    };
}
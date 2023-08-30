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
 *  name,
 *  type,
 *  offset
*/

#pragma once
#include "System.h"

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
        enum // Property type 
        {
            PT_VALUE,
            PT_ENUM,
            PT_DATA,
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
            F_SAVE_LOAD_COPY = 0X03,
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

        Property(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset)
        : m_pOwner(&pOwner), m_Name(name), m_uiFlag(uiFlag), m_uiOffset(uiOffset)
        {
        }

        
        Property(const Property&) = delete;
        Property& operator=(const Property&) = delete;
        Property(Property&&) = default;
        Property& operator=(Property&&) = default;
        virtual ~Property() = default;

        virtual bool Clone(const Property* property)
        {
            m_pOwner = property->m_pOwner;
            m_Name = property->m_Name;
            m_uiFlag = property->m_uiFlag;
            m_uiOffset = property->m_uiOffset;
            return true;
        }

        virtual void Serialize()
        {
        }

        std::string GetName() const
        {
            return m_Name;
        }

        unsigned int GetFlag()
        {
            return m_uiFlag;
        }

        void SetOwner(ClassInfo& pOwner)
        {
            m_pOwner = &pOwner;
        }

        void SetName(const std::string name)
        {
            m_Name = m_Name;
        }

        void SetFlag(unsigned int uiFlag)
        {
            m_uiFlag = uiFlag;
        }

        virtual void* GetValueAddress(void* pObj) const
        {
            return (void*)(((unsigned char*)pObj) + m_uiOffset); 
        }

        virtual Property* GetInstance()
        {
            return nullptr;
        }

    protected:
        ClassInfo* m_pOwner;
        std::string m_Name;
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

        EnumProperty(ClassInfo& pOwner, const std::string& name, const std::string& enumName, unsigned int uiFlag, unsigned int uiOffset)
        : Property(pOwner, name, uiFlag, uiOffset), m_EnumName(enumName)
        {
        }

        virtual ~EnumProperty()
        {
        }

        EnumProperty(const EnumProperty&) = delete;
        EnumProperty(EnumProperty&&) = default;
        EnumProperty& operator=(const EnumProperty&) = delete;
        EnumProperty& operator=(EnumProperty&&) = default;

        virtual void Clone(const Property* property)
        {
            EnumProperty& tmp = static_cast<EnumProperty&>(property);
            Property::Clone(tmp);
            m_EnumName = tmp.m_EnumName;
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

        virtual Property* GetInstance()
        {
            return new EnumProperty<T>();
        }

    private:
        std::string m_EnumName; //Q: There is already m_Name, why define m_EnumName
    };

    template<typename T, typename NumType>
    class DataProperty: public Property
    {
    public:
        DataProperty()
        {
        }

        DataProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiOffset, bool bDynamicCreate, unsigned int uiDataNum)
        : Property(pOwner, name, F_SAVE_LOAD_CLONE, uiOffset), m_bDynamicCreate(bDynamicCreate), m_uiDataNum(uiDataNum)
        {
        }

        DataProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiNumOffset)
        : Property(pOwner, name, F_SAVE_LOAD_CLONE, uiOffset), m_uiNumOffset(uiNumOffset)
        {
        }

        virtual ~DataProperty()
        {

        }

        DataProperty(const DataProperty&) = delete;
        DataProperty(DataProperty&&) = default;
        DataProperty& operator=(const DataProperty&) = delete;
        DataProperty& operator=(DataProperty&&) = default;

        virtual void Clone(const Property* property)
        {
            DataProperty<T, NumType>& tmp = static_cast<DataProperty<T, NumType>&>(property);
            Property::Clone(tmp);
            m_bDynamicCreate = tmp.m_bDynamicCreate;
            m_uiDataNum = tmp.m_uiDataNum;
            m_uiNumOffset = tmp.m_uiNumOffset;
        }

        virtual Property* GetInstance()
        {
            return new DataProperty<T, NumType>();
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

        RangeProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, bool bRange = false, T lowValue = T(), T highValue = T(), T step = T())
        : Property(pOwner, name, F_SAVE_LOAD_CLONE, uiOffset)
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

        virtual void Clone(const Property* property)
        {
            RangeProperty<T>& tmp = static_cast<RangeProperty<T>&>(property);
            Property::Clone(tmp);
            m_LowValue = tmp.m_LowValue;
            m_HighValue = tmp.m_HighValue;
            m_Step = tmp.m_Step;
            m_bRange = tmp.m_bRange;
        }

        virtual Property* GetInstance()
        {
            return new RangeProperty<T>();
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

        ValueProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, bool bRange = false, T lowValue = T(), T highValue = T(), T step = T())
        :RangeProperty<T>(pOwner, name, uiFlag, uiOffset, bRange, lowValue, highValue, step)
        {
        }

        virtual ~ValueProperty()
        {
        }

        ValueProperty(const ValueProperty&) = delete;
        ValueProperty(ValueProperty&&) = default;
        ValueProperty& operator=(const ValueProperty&) = delete;
        ValueProperty& operator=(ValueProperty&&) = default;

        virtual bool SetValue(void* pObj, T& srcValue)
        {
            if (srcValue < this->m_LowValue || srcValue > this->m_HighValue)
            {
                return false;
            }
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

        virtual Property* GetInstance()
        {
            return new ValueProperty<T>();
        }
    };

    template<typename ArrayType, typename T>
    class ArrayProperty : public RangeProperty<T>
    {
    public:
        ArrayProperty()
        {
        }

        ArrayProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, T lowValue = T(), T highValue = T(), T step = T(), bool bRange = false)
        : RangeProperty<T>(pOwner, name, uiFlag, uiOffset, lowValue, highValue, step, bRange)
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
            if (pDataSrc < this->m_LowValue || pDataSrc > this->m_HighValue)
            {
                return false;
            }
            GetContainer(pObj)[uiIndex] = pDataSrc;
            return true;
        }

        virtual bool GetValue(void* pObj, unsigned int uiIndex, T& pDataDest) const
		{
			pDataDest = (GetContainer(pObj)[uiIndex]);
			return true;
		}

        virtual Property* GetInstance()
        {
            return new ArrayProperty<ArrayType, T>();
        }
    };

    template<typename MapType, typename KeyType, typename ValueType>
    class MapProperty : public RangeProperty<ValueType>
    {
    public:
        MapProperty()
        {
        }

        MapProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, ValueType lowValue = ValueType(), ValueType highValue = ValueType(), ValueType step = ValueType(), bool bRange = false)
        : RangeProperty<MapType>(pOwner, name, uiFlag, uiOffset, lowValue, highValue, step, bRange)
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
            if (value < this->m_LowValue || value > this->m_HighValue)
                return false;
            
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

        virtual Property* GetInstance()
        {
            return new MapProperty<MapType, KeyType, ValueType>();
        }
    };

    template<typename T, typename NumType>
    class DataPropertyCreator
    {
    public:
        DataProperty<T, NumType>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiOffset, bool bDynamicCreate, unsigned int uiDataNum)
        {
            return new DataProperty<T, NumType>(pOwner, name, uiOffset, bDynamicCreate, uiDataNum);
        }

        DataProperty<T, NumType>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiNumOffset)
        {
            return new DataProperty<T, NumType>(pOwner, name, uiOffset, uiNumOffset);
        }
    };

    template<typename T>
    class AutoPropertyCreator
    {
    public:
        ValueProperty<T>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset)
        {
            return new ValueProperty<T>(pOwner, name, uiFlag, uiOffset);
        }

        ValueProperty<T>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, T lowValue, T highValue, T step)
        {
            return new ValueProperty<T>(pOwner, name, uiFlag, uiOffset, lowValue, highValue, step);
        }
    };

    template<typename T, typename Alloc>
    class AutoPropertyCreator<std::vector<T, Alloc>>
    {
    public:
        ArrayProperty<std::vector<T, Alloc>, T>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset)
        {
            return new ArrayProperty<std::vector<T, Alloc>, T>(pOwner, name, uiFlag, uiOffset);
        }

        ArrayProperty<std::vector<T, Alloc>, T>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, T lowValue, T highValue, T step)
        {
            return new ArrayProperty<std::vector<T, Alloc>, T>(pOwner, name, uiFlag, uiOffset, lowValue, highValue, step);
        }
    };

    template<typename KeyType, typename ValueType, typename Alloc>
    class AutoPropertyCreator<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>>
    {
    public:
        MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset)
        {
            return new MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>(pOwner, name, uiFlag, uiOffset);
        }

        MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>* CreateProperty(ClassInfo& pOwner, const std::string& name, unsigned int uiFlag, unsigned int uiOffset, ValueType lowValue, ValueType highValue, ValueType step)
        {
            return new MapProperty<std::map<KeyType, ValueType, std::less<KeyType>, Alloc>, KeyType, ValueType>(pOwner, name, uiFlag, uiOffset, lowValue, highValue, step);
        }
    };

}
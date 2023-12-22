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

#pragma once
#include "Graphic.h"
#include "Core/Type.marc"
#include "Core/Object.h"
#include "System.h"

#include <vector>
#include <map>

#ifdef DEBUG
#include <spdlog/spdlog.h>
#include "spdlog/fmt/bin_to_hex.h"
#endif // DEBUG



namespace NoelleGraphic
{
    class Object;

    class GRAPHIC_API BinaryStream final
    {
    public:
        enum // Archive Type
        {
            AT_REGISTER,
            AT_SIZE,
            AT_SAVE,
            AT_LOAD,
            AT_LINK,
            AT_POSTLOAD
        };

        BinaryStream();
        BinaryStream(uint32_t uiFlag);
        ~BinaryStream();
        
        bool ArchiveAll(Object* object);
        bool Save(std::string path);
        bool Load(std::string path);
        bool LoadFromBuffer(unsigned char* pBuffer, uint32_t uiSize);

        void AddBufferSize(uint32_t uiSize)
        {
            m_uiArchivePropertySize += uiSize;
        }

        template <typename T>
        void Archive(T& io) 
        {
            switch (m_uiStreamFlag)
            {
                case AT_REGISTER:
                {
                    if (TIsNoellePointerType<T>::Value)
                    {
                        Object*& obj = *static_cast<Object**>(static_cast<void*>(&io));
                        ArchiveAll(obj);
                    }
                    else if (TIsNoelleUniquePointerType<T>::Value)
                    {
                        std::unique_ptr<Object>& obj = *static_cast<std::unique_ptr<Object>*>(static_cast<void*>(&io));
                        ArchiveAll(obj.get());
                    }
                    else if (TIsNoelleSharedPointerType<T>::Value)
                    {
                        std::shared_ptr<Object>& obj = *static_cast<std::shared_ptr<Object>*>(static_cast<void*>(&io));
                        ArchiveAll(obj.get());
                    }
                    else if (TIsNoelleType<T>::Value)
                    {
                        Object* obj = static_cast<Object*>(static_cast<void*>(&io));
                        ArchiveAll(obj);
                    }
                }
                break;
            case AT_SAVE:
                {
                    if (TIsNoellePointerType<T>::Value)
                    {
                        Object*& obj = *static_cast<Object**>(static_cast<void*>(&io));
                        WriteObjectTableID(obj);
                    }
                    else if (TIsNoelleUniquePointerType<T>::Value)
                    {
                        std::unique_ptr<Object>& obj = *static_cast<std::unique_ptr<Object>*>(static_cast<void*>(&io));
                        WriteObjectTableID(obj.get());
                    }
                    else if (TIsNoelleSharedPointerType<T>::Value)
                    {
                        std::shared_ptr<Object>& obj = *static_cast<std::shared_ptr<Object>*>(static_cast<void*>(&io));
                        WriteObjectTableID(obj.get());
                    }
                    else if (TIsNoelleType<T>::Value)
                    {
                        Object* obj = static_cast<Object*>(static_cast<void*>(&io));
                        WriteObjectTableID(obj);
                    }
                    else if (TIsStringType<T>::Value)
                    {
                        std::string str = *static_cast<std::string*>(static_cast<void*>(&io));
                        uint32_t strLen = sizeof(str.data());
                        Write(&strLen, sizeof(uint32_t));
                        char* tmp = str.data();
                        Write(static_cast<void*>(&tmp), strLen);
                    }
                    else
                    {
                        Write(static_cast<void*>(&io), sizeof(T));
                    }
                }
                break;
            case AT_LOAD:
                {
                    if (TIsNoellePointerType<T>::Value)
                    {
                        Object*& obj = *static_cast<Object**>(static_cast<void*>(&io));
                        ReadObjectTableID(obj);
                    }
                    else if (TIsNoelleUniquePointerType<T>::Value)
                    {
                        std::unique_ptr<Object>& obj = *static_cast<std::unique_ptr<Object>*>(static_cast<void*>(&io));
                        Object* tmp = nullptr;
                        ReadObjectTableID(tmp);
                        obj.reset(tmp);
                    }
                    else if (TIsNoelleSharedPointerType<T>::Value)
                    {
                        std::shared_ptr<Object>& obj = *static_cast<std::shared_ptr<Object>*>(static_cast<void*>(&io));
                        Object* tmp = nullptr;
                        ReadObjectTableID(tmp);
                        obj.reset(tmp);
                    }
                    else if (TIsNoelleType<T>::Value)
                    {
                        Object* obj = static_cast<Object*>(static_cast<void*>(&io));
                        Object* deserializedObj = nullptr;
                        ReadObjectTableID(deserializedObj);
                        *obj = *deserializedObj;
                    }
                    else if (TIsStringType<T>::Value)
                    {
                        std::string& str = *static_cast<std::string*>(static_cast<void*>(&io));
                        uint32_t strLen = 0;
                        Read(&strLen, sizeof(uint32_t));
                        str.resize(strLen);
                        Read(static_cast<void*>(&str[0]), strLen);
                    }
                    else
                    {
                        Read((void*)&io, sizeof(T));
                    }
                }
                break;
            case AT_SIZE:
                {
                    if (TIsNoellePointerType<T>::Value || TIsNoelleUniquePointerType<T>::Value || TIsNoelleSharedPointerType<T>::Value)
                    {
                        m_uiArchivePropertySize += sizeof(uint32_t);
                    }
                    else if (TIsStringType<T>::Value)
                    {
                        std::string str = *static_cast<std::string*>(static_cast<void*>(&io));
                        m_uiArchivePropertySize += sizeof(str.data());
                    }
                    else
                    {
                        m_uiArchivePropertySize += sizeof(T);
                    }
                }
                break;
            }
        }

        template <typename T, typename Alloc>
        void Archive(std::vector<T, Alloc>& io) 
        {
            switch (m_uiStreamFlag)
            {
                case AT_SAVE:
                {
                    uint32_t uiSize = io.size();
                    Archive(uiSize);

                    if (TIsNoNeedLoop<T>::Value)
                    {
                        void* dataPtr = io.data();
                        Write(dataPtr, io.size() * sizeof(T));
                    }
                    else
                    {
                        for (size_t i = 0; i < io.size(); ++i)
                        {
                            Archive(io[i]);
                        }
                    }
                }
                break;
            case AT_REGISTER:
                {
                    if (!TIsNeedGC<T>::Value)
                        return;

                    for (T elem : io)
                    {
                        Archive(elem);
                    }
                }
                break;
            case AT_SIZE:
                {
                    m_uiArchivePropertySize += io.size() * sizeof(T);
                }
                break;
            case AT_LOAD:
                {
                    uint32_t uiSize = 0;
                    Archive(uiSize);
                    io.resize(uiSize);
                    if (TIsNoNeedLoop<T>::Value)
                    {
                        Read(io.data(), io.size() * sizeof(T));
                    }
                    else
                    {
                        for (size_t i = 0; i < io.size(); ++i)
                        {
                            Archive(io[i]);
                        }
                    }
                }
                break;
            }
        }

        template <typename KeyType, typename ValueType, typename Alloc>
        void Archive(std::map<KeyType, ValueType, std::less<KeyType>>& io) 
        {
            switch (m_uiStreamFlag)
            {
                case AT_SAVE:
                    {
                        uint32_t uiSize = 0;
                        Archive(uiSize);

                        if (TIsNoNeedLoop<KeyType>::Value && TIsNoNeedLoop<ValueType>::Value)
                        {
                            uint32_t dataSize = uiSize * (sizeof(KeyType) + sizeof(ValueType));
                            Write(io.data(), dataSize);
                        }
                        else
                        {
                            for (const auto& pair : io)
                            {
                                Archive(pair.first);
                                Archive(pair.second);
                            }
                        }
                    }
                    break;
                case AT_REGISTER:
                    {
                        if (!TIsNeedGC<KeyType>::Value || !TIsNeedGC<ValueType>::Value)
                            return;

                        for (const auto& pair : io)
                        {
                            if (TIsNeedGC<KeyType>::Value)
                                Archive(pair.first);
                            if (TIsNeedGC<ValueType>::Value)
                                Archive(pair.second);
                        }
                    }
                    break;
                case AT_SIZE:
                    {
                        m_uiArchivePropertySize += io.size() * (sizeof(KeyType) + sizeof(ValueType));
                    }
                    break;
                case AT_LOAD:
                    {
                        uint32_t uiSize = 0;
                        Archive(uiSize);

                        if (TIsNoNeedLoop<KeyType>::Value && TIsNoNeedLoop<ValueType>::Value)
                        {
                            void* dataPtr = io.data();
                            size_t dataSize = io.size() * (sizeof(KeyType) + sizeof(ValueType));
                            Read(dataPtr, dataSize);
                        }
                        else
                        {
                            for (const auto& pair : io)
                            {
                                Archive(pair.first);
                                Archive(pair.second);
                            }
                        }
                    }
                    break;
            }
        }

        bool Read(void* pvBuffer, unsigned int uiSize)
        {
            if (!pvBuffer || !m_pcBuffer)
                return false;

            if ((unsigned int)(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
                return false;

            if (Noelle::NoelMemCpy(pvBuffer, m_pcCurBufPtr, uiSize))
            {
#ifdef DEBUG
                spdlog::info("Read");
                char* pByteData = new char[uiSize];
                memcpy(pByteData, pvBuffer, uiSize);
                std::array<char, 80> buf = { 0 };
                for (int i = 0; i < uiSize; ++i)
                    buf[i] = pByteData[i];

                spdlog::info(spdlog::to_hex(buf));
                spdlog::info(uiSize);
                delete[] pByteData;
#endif // DEBUG
                m_pcCurBufPtr += uiSize;
                return true;
            }

            return false;
        }

        virtual uint32_t GetStreamFlag()
        {
            return m_uiStreamFlag;
        }

        virtual void SetStreamFlag(uint32_t uiFlag)
        {
            m_uiStreamFlag = uiFlag;
        }

        bool Write(const void* pvBuffer, unsigned int uiSize)
        {
            if (!pvBuffer || !m_pcBuffer)
                return false;

            if ((unsigned int)(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
                return false;

            if (Noelle::NoelMemCpy(m_pcCurBufPtr, pvBuffer, uiSize))
            {
#ifdef DEBUG
                spdlog::info("Write");
                char* pByteData = new char[uiSize];
                memcpy(pByteData, pvBuffer, uiSize);
                std::array<char, 80> buf = {0};
                for (int i = 0; i < uiSize; ++i)
                    buf[i] = pByteData[i];

                spdlog::info(spdlog::to_hex(buf));
                spdlog::info(uiSize);
                delete[] pByteData;
#endif // DEBUG
                m_pcCurBufPtr += uiSize;
                return true;
            }
            
            return false;
        }

        bool RegisterObject(Object* object);

        bool WriteObjectTableID(Object* const& object);

        bool ReadObjectTableID(Object*& object);

        Object* GetObjectByType(const ClassInfo& pType);
    
    protected:
        uint32_t m_uiArchivePropertySize;
        uint32_t m_uiFlag;
        uint32_t m_uiStreamFlag;
        static uint32_t ms_uiVersion;

    private:
         struct PropertyTable
         {
             uint32_t m_uiPropertyOffset;
             uint32_t m_uiPropertyName;
             uint32_t m_uiSize;
         };

         struct ObjectTable
         {
             uint32_t m_typeID;
             uint32_t m_uiTableID;  // Need to save object table id since Objects/Pointers/Smart Pointers can be properties
             uint32_t m_uiPropertySize;
             uint32_t m_uiPropertyNum;
             uint32_t m_uiPropertyTableOffset;
             uint32_t m_uiPropertyTableSize;
             std::vector<PropertyTable> m_propertyTableArray;
         };

         unsigned char* m_pcCurBufPtr;
         unsigned char* m_pcBuffer;
         unsigned int m_uiBufferSize;
         std::vector<Object*> m_ObjectArray;
         std::unordered_map<Object*, uint32_t> m_Object2GUID;
         std::unordered_map<uint32_t, Object*> m_GUID2Object;
    };
}
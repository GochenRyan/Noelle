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

#include "Stream.h"
#include "Core/ClassInfo.h"
#include "Core/Context.h"
#include <fstream>

using namespace NoelleGraphic;

uint32_t NoelleGraphic::BinaryStream::ms_uiVersion = 0;


BinaryStream::BinaryStream()
{
    m_pcCurBufPtr = nullptr;
    m_pcBuffer = nullptr;
    m_uiBufferSize = 0;
    m_ObjectArray.clear();
    m_Object2GUID.clear();
    m_GUID2Object.clear();
}

BinaryStream::BinaryStream(uint32_t uiFlag)
{
    m_uiFlag = uiFlag;
    m_pcCurBufPtr = nullptr;
    m_pcBuffer = nullptr;
    m_uiBufferSize = 0;
    m_ObjectArray.clear();
    m_Object2GUID.clear();
    m_GUID2Object.clear();
}

NoelleGraphic::BinaryStream::~BinaryStream()
{
    m_pcCurBufPtr = nullptr;
    m_pcBuffer = nullptr;
    m_ObjectArray.clear();
    m_Object2GUID.clear();
    m_GUID2Object.clear();
}

bool BinaryStream::ArchiveAll(Object *object)
{
    switch (m_uiStreamFlag)
    {
    case AT_REGISTER:
    {
        if (RegisterObject(object))
        {
            ClassInfo& type = object->GetType();
            uint32_t propertyNum = type.GetPropertyNum();
            for (size_t i = 0; i < propertyNum; ++i)
            {
                Property* property = type.GetProperty(i);
                property->Archive(*this, object);
            }
        }
    }
    break;
    }
    return true;
}

/*
* Format:
*   Version : uint32_t
*   ObjectTable Size : uint32_t
*   Loop(ObjectTable Size){
*       m_typeID : uint32_t
*       m_uiTableID : uint32_t
*       m_uiPropertySize : uint32_t
*       m_uiPropertyNum : uint32_t
*       m_uiPropertyTableOffset : uint32_t
*       m_uiPropertyTableSize : uint32_t
*   }
*   Loop(ObjectTable Size){
*   Loop(m_uiPropertyNum){
*       m_uiPropertyOffset : uint32_t
*       m_uiPropertyName : uint32_t(StringCrc)
*       m_uiSize : uint32_t
*   }
*   }
*   Archive Property
*/

bool BinaryStream::Save(std::string path)
{
    m_uiBufferSize = 0;
    m_uiBufferSize += sizeof(uint32_t);
    m_uiBufferSize += sizeof(uint32_t);

    /* Generate object tables and property tables */
    std::vector<ObjectTable> objectTableArray(m_ObjectArray.size());
    for (int i = 0; i < m_ObjectArray.size(); ++i)
    {
        uint32_t uiTypeID; 
        if (!Context::GetTypeIDByClassInfo(&m_ObjectArray[i]->GetType(), uiTypeID))
            continue;
        
        objectTableArray[i].m_typeID = uiTypeID;
        objectTableArray[i].m_uiTableID = m_uiBufferSize;

        m_Object2GUID[m_ObjectArray[i]] = objectTableArray[i].m_uiTableID;
        m_uiBufferSize += 6 * sizeof(uint32_t);
    }
    
    for (int i = 0; i < objectTableArray.size(); ++i)
    {
        int archivePropertyCnt = 0;
        uint32_t uiPropertyNum = m_ObjectArray[i]->GetType().GetPropertyNum();
        for (int j = 0; j < uiPropertyNum; ++j)
        {
            if (m_ObjectArray[i]->GetType().GetProperty(j)->GetFlag() & Property::F_SAVE_LOAD != 0)
            {
                archivePropertyCnt++;
            }
        }
        objectTableArray[i].m_propertyTableArray.resize(archivePropertyCnt);
    }

    m_uiStreamFlag = AT_SIZE;
    for (int i = 0; i < objectTableArray.size(); ++i)
    {
        objectTableArray[i].m_uiPropertyTableOffset = m_uiBufferSize;
        objectTableArray[i].m_uiPropertyNum = 0;
        uint32_t uiPropertyNum = m_ObjectArray[i]->GetType().GetPropertyNum();
        uint32_t uiTotalPropertySize = 0;
        int archivePropertyIndex = 0;
        for (int j = 0; j < uiPropertyNum; ++j)
        {
            if (m_ObjectArray[i]->GetType().GetProperty(j)->GetFlag() & Property::F_SAVE_LOAD != 0)
            {
                m_uiArchivePropertySize = 0;
                objectTableArray[i].m_uiPropertyNum++;
                objectTableArray[i].m_propertyTableArray[archivePropertyIndex].m_uiPropertyName = m_ObjectArray[i]->GetType().GetProperty(j)->GetName().Value();
                m_ObjectArray[i]->GetType().GetProperty(j)->Archive(*this, m_ObjectArray[i]);
                objectTableArray[i].m_propertyTableArray[archivePropertyIndex].m_uiSize = m_uiArchivePropertySize;
                m_uiBufferSize += 3 * sizeof(uint32_t);

                uiTotalPropertySize += m_uiArchivePropertySize;
                archivePropertyIndex++;
            }
        }

        objectTableArray[i].m_uiPropertySize = uiTotalPropertySize;
        objectTableArray[i].m_uiPropertyTableSize = m_uiBufferSize - objectTableArray[i].m_uiPropertyTableOffset;
    }

    for (int i = 0; i < objectTableArray.size(); ++i)
    {
        uint32_t uiPropertyNum = m_ObjectArray[i]->GetType().GetPropertyNum();
        for (int j = 0; j < objectTableArray[i].m_uiPropertyNum; ++j)
        {
            objectTableArray[i].m_propertyTableArray[j].m_uiPropertyOffset = m_uiBufferSize;
            m_uiBufferSize += objectTableArray[i].m_propertyTableArray[j].m_uiSize;
        }
    }

    if (m_pcBuffer)
        delete[] m_pcBuffer;
    m_pcBuffer = new unsigned char[m_uiBufferSize];
    if (!m_pcBuffer)
        return false;

    m_pcCurBufPtr = m_pcBuffer;

    /* Write version */
    Write(&ms_uiVersion, sizeof(uint32_t));

    /* Write obejct num */
    uint32_t uiObjectNum = objectTableArray.size();
    Write(&uiObjectNum, sizeof(uint32_t));

    /* Write object tables */
    for (int i = 0; i < objectTableArray.size(); ++i)
    {
        Write(&objectTableArray[i].m_typeID, sizeof(uint32_t));
        Write(&objectTableArray[i].m_uiTableID, sizeof(uint32_t));
        Write(&objectTableArray[i].m_uiPropertySize, sizeof(uint32_t));
        Write(&objectTableArray[i].m_uiPropertyNum, sizeof(uint32_t));
        Write(&objectTableArray[i].m_uiPropertyTableOffset, sizeof(uint32_t));
        Write(&objectTableArray[i].m_uiPropertyTableSize, sizeof(uint32_t));
    }

    /* Write property tables */
    for (int i = 0; i < objectTableArray.size(); ++i)
    {
        for (int j = 0; j < objectTableArray[i].m_propertyTableArray.size(); ++j)
        {
            Write(&objectTableArray[i].m_propertyTableArray[j].m_uiPropertyOffset, sizeof(uint32_t));
            Write(&objectTableArray[i].m_propertyTableArray[j].m_uiPropertyName, sizeof(uint32_t));
            Write(&objectTableArray[i].m_propertyTableArray[j].m_uiSize, sizeof(uint32_t));
        }
    }

    /* Save properties */
    m_uiStreamFlag = AT_SAVE;
    for (int i = 0; i < m_ObjectArray.size(); ++i)
    {
        uint32_t uiPropertyNum = m_ObjectArray[i]->GetType().GetPropertyNum();
        for (int j = 0; j < uiPropertyNum; ++j)
        {
            if (m_ObjectArray[i]->GetType().GetProperty(j)->GetFlag() & Property::F_SAVE_LOAD != 0)
            {
                m_ObjectArray[i]->GetType().GetProperty(j)->Archive(*this, m_ObjectArray[i]);
            }
        }
    }

    /* Write to the file */
    std::ofstream outputFile(path, std::ios::binary);
    if (!outputFile.is_open())
    {
        return false;
    }
    outputFile.write(reinterpret_cast<const char*>(m_pcBuffer), m_uiBufferSize);
    outputFile.close();

    delete[] m_pcBuffer;
    m_uiBufferSize = 0;

    return true;
}

bool BinaryStream::Load(std::string path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        return false;
    }

    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    char* pBuffer = new char[fileSize];
    file.read(pBuffer, fileSize);

    if (!file) {
        delete[] pBuffer;
        return false;
    }

    if (!LoadFromBuffer(static_cast<unsigned char*>(static_cast<void*>(pBuffer)), fileSize))
    {
        delete[] pBuffer;
        return false;
    }

    delete[] pBuffer;
    file.close();
    return true;
}

bool BinaryStream::LoadFromBuffer(unsigned char* pBuffer, uint32_t uiSize)
{
    if (pBuffer == nullptr || uiSize == 0)
        return false;

    m_ObjectArray.clear();
    m_GUID2Object.clear();

    m_pcBuffer = pBuffer;
    m_pcCurBufPtr = pBuffer;
    m_uiBufferSize = uiSize;

    Read(&ms_uiVersion, sizeof(uint32_t));

    uint32_t uiObjectNum;
    Read(&uiObjectNum, sizeof(uint32_t));

    std::vector<ObjectTable> objectTableArray(uiObjectNum);

    for (int i = 0; i < uiObjectNum; ++i)
    {
        Read(&objectTableArray[i].m_typeID, sizeof(uint32_t));
        Read(&objectTableArray[i].m_uiTableID, sizeof(uint32_t));
        Read(&objectTableArray[i].m_uiPropertySize, sizeof(uint32_t));
        Read(&objectTableArray[i].m_uiPropertyNum, sizeof(uint32_t));
        Read(&objectTableArray[i].m_uiPropertyTableOffset, sizeof(uint32_t));
        Read(&objectTableArray[i].m_uiPropertyTableSize, sizeof(uint32_t));
    }

    for (int i = 0; i < objectTableArray.size(); ++i)
    {
        std::vector<PropertyTable> propertyTableArray(objectTableArray[i].m_uiPropertyNum);

        for (int j = 0; j < propertyTableArray.size(); ++j)
        {
            Read(&propertyTableArray[j].m_uiPropertyOffset, sizeof(uint32_t));
            Read(&propertyTableArray[j].m_uiPropertyName, sizeof(uint32_t));
            Read(&propertyTableArray[j].m_uiSize, sizeof(uint32_t));
        }
        objectTableArray[i].m_propertyTableArray = propertyTableArray;
    }

    for (int i = 0; i < uiObjectNum; ++i)
    {
        ClassInfo* pType = nullptr;
        if (Context::GetClassInfoByTypeID(objectTableArray[i].m_typeID, pType))
        {
            Object* pObject = Object::GetInstance(*pType);
            if (pObject != nullptr)
            {
                m_GUID2Object[objectTableArray[i].m_uiTableID] = pObject;
                RegisterObject(pObject);
            }   
        }
    }
    
    m_uiStreamFlag = AT_LOAD;
    for (int i = 0; i < m_ObjectArray.size(); ++i)
    {
        uint32_t uiPropertyNum = m_ObjectArray[i]->GetType().GetPropertyNum();
        for (int j = 0; j < uiPropertyNum; ++j)
        {
            if (m_ObjectArray[i]->GetType().GetProperty(j)->GetFlag() & Property::F_SAVE_LOAD != 0)
            {
                m_ObjectArray[i]->GetType().GetProperty(j)->Archive(*this, m_ObjectArray[i]);
            }
        }
    }
}

bool BinaryStream::RegisterObject(Object* object)
{
    assert(object != nullptr);

    if (object == nullptr)
        return false;

    for (Object* obj : m_ObjectArray)
    {
        if (obj == object)
            return false;
    }

    m_ObjectArray.push_back(object);
    return true;
}


bool BinaryStream::WriteObjectTableID(Object* const &object)
{
    if (m_Object2GUID.contains(object))
    {
        uint32_t uiGUID = m_Object2GUID[object];
        return Write(&uiGUID, sizeof(uint32_t));
    } 
    
    return false;
}

bool BinaryStream::ReadObjectTableID(Object*& object)
{
    uint32_t uiGUID = 0;
    if (Read(&uiGUID, sizeof(uint32_t)))
    {
        if (m_GUID2Object.contains(uiGUID))
        {
            object = m_GUID2Object[uiGUID];
            return true;
        }
    }
    return false;
}

Object* NoelleGraphic::BinaryStream::GetObjectByType(const ClassInfo& pType)
{
    //TODO: same type objects
    for(Object* obj : m_ObjectArray)
    {
        if (obj->GetType().IsSameType(pType))
            return obj;
    }
    return nullptr;
}

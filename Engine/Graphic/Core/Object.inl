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

#include "Object.h"

inline bool Object::IsSameType(const ClassInfo& Type) const
{
    return GetType().IsSameType(Type);
}

inline bool Object::IsDerived(const ClassInfo& Type) const
{
    return GetType().IsDerived(Type);
}

inline bool Object::IsSameType(const Object* pObject) const
{
    return pObject && GetType().IsSameType(pObject->GetType());
}

inline bool Object::IsDerived(const Object* pObject) const
{
    return pObject && GetType().IsDerived(pObject->GetType());
}

inline void Object::SetFlag(unsigned int uiFlag)
{
    m_uiFlag |= uiFlag;
}

inline void Object::ClearFlag(unsigned int uiFlag)
{
    m_uiFlag &= ~uiFlag;
}

inline bool Object::IsHasFlag(unsigned int uiFlag)
{
    return (m_uiFlag & uiFlag) != 0;
}


template<class T>
T* StaticCast(Object* pObject)
{
    return (T*)pObject;
}

template<class T>
const T* StaticCast(const Object* pObject)
{
    return (const T*)pObject;
}

template<class T>
T* DynamicCast(Object* pObject)
{
    return pObject && pObject.IsDerived(T::ms_Type) ? (T*)pObject: nullptr;
}

template<class T>
const T* DynamicCast(const Object* pObject)
{
    return pObject && pObject.IsDerived(T::ms_Type) ? (const T*)pObject: nullptr;
}

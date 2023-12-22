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
#include "StringCrc.h"
#include "Context.h"
#include "ClassInfo.h"
#include "ObjectDefines.h"

#include "Property.marc"

using namespace NoelleGraphic;

IMPLEMENT_CLASSINFO_NOPARENT_NOCREATEFUNC(Object)

std::unordered_map<uint32_t, CreateObjectFunc> Object::ms_ClassFactory;

IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(Object)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
ADD_PROPERTY_ROOT_BEGIN(Object)
REGISTER_PROPERTY(m_uiFlag, Flag, Property::F_CLONE)
ADD_PROPERTY_END
IMPLEMENT_REGISTER_TYPE_ID(1, Object)

Object::Object()
{
    GetFastObjManager().AddObject(this);
}

Object::~Object()
{
    GetFastObjManager().DeleteObject(this);
}

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


Object* Object::GetInstance(const ClassInfo& Type)
{
    uint32_t nameValue = Type.GetName().Value();
    if (ms_ClassFactory.contains(nameValue))
    {
        Object* pObject = ms_ClassFactory[nameValue]();
        return pObject;
    }
    return nullptr;
}

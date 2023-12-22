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

#include "ClassInfo.h"

inline StringCrc ClassInfo::GetName() const
{
    return m_crcName;
}

inline bool ClassInfo::IsSameType(const ClassInfo& Type) const
{
	return (&Type == this);
}

inline bool ClassInfo::IsDerived(const ClassInfo& Type) const
{
    const ClassInfo* temp = this;
    while(!temp->IsSameType(Type))
    {
        if (temp->m_pBase)
        {
            temp = temp->m_pBase;
        }
        else
        {
            return false;
        }
    }
    return true;
}
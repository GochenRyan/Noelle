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
#include <memory>

namespace NoelleUtils
{

    /// @brief See https://stackoverflow.com/questions/36120424/alternatives-of-static-pointer-cast-for-unique-ptr
    /// @tparam TO 
    /// @tparam FROM 
    /// @param old 
    /// @return 
    template<typename TO, typename FROM>
    std::unique_ptr<TO> StaticUniquePointerCast(std::unique_ptr<FROM>&& old) 
    {
        return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
    }
}
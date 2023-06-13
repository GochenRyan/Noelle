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

#define NOMINMAX
#include <MemManager.h>
#undef NOMINMAX

#include "Core/ClassInfo.h"
#include "StringCrc.h"
#include "Core/Object.h"
#include "Core/ClassInfo.marc"


#include <iostream>

using namespace NoelleGraphic;

class A: public Object
{
public:
    A(){}
    virtual ~A(){}
    DECLARE_CLASSINFO
};

class B: public A
{
public:
    B(){}
    virtual ~B(){}
    DECLARE_CLASSINFO
};

IMPLEMENT_CLASSINFO_NOCREATEFUNC(A, Object)
IMPLEMENT_CLASSINFO_NOCREATEFUNC(B, A)

int main()
{
    A* a = new A();
    B* b = new B();

    bool bDerived = b->IsDerived(a);
    std::cin.get();
	return 0;
}
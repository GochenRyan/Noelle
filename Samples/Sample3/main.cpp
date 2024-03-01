// Copyright 2024 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <MemManager.h>


#include "Core/ClassInfo.h"
#include "StringCrc.h"
#include "Core/Object.h"
#include "Core/Context.h"

#include "Core/ClassInfo.marc"
#include "Core/Initial.marc"


#include <iostream>

using namespace NoelleGraphic;

class A: public Object
{
public:
    A(){}
    virtual ~A(){}
    DECLARE_CLASSINFO
    DECLARE_INITIAL_NO_CLASS_FACTORY
};

class B: public A
{
public:
    B(){}
    virtual ~B(){}
    DECLARE_CLASSINFO
    DECLARE_INITIAL_NO_CLASS_FACTORY
};

IMPLEMENT_CLASSINFO_NOCREATEFUNC(A, Object)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(A)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
ADD_PROPERTY_BEGIN(A, Object)
ADD_PROPERTY_END

IMPLEMENT_CLASSINFO_NOCREATEFUNC(B, A)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(B)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
ADD_PROPERTY_BEGIN(B, A)
ADD_PROPERTY_END


int main()
{
    A* a = new A();
    B* b = new B();

    bool bDerived = b->IsDerived(a);

    delete a;
    delete b;
	return 0;
}
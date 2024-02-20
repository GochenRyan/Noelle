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

#include "MemManager.h"
using namespace Noelle;

class DefualtSample
{

};

class ConstructSample
{
public:
	ConstructSample() {}
	~ConstructSample() {}
};

class ArgsSample
{
public:
	ArgsSample(int args1) : m_args1(args1) {};

private:
	int m_args1;
};

class VirtualSample
{
public:
	virtual void func() {};
};

struct MyStruct
{
	int a;
	float b;
};

void func1()
{
	StackMemAlloc<MyStruct> ms(1);
	ms.GetPtr()->a = 3;
	ms.GetPtr()->b = 4.0;
}

void func2()
{
	func1();
	StackMemAlloc<MyStruct> ms(1);
	ms.GetPtr()->a = 1;
	ms.GetPtr()->b = 2.0;
}

int main()
{
	//StackMemAlloc<DefualtSample> sample(2);
	//StackMemAlloc<ConstructSample> sample(2);
	//StackMemAlloc<VirtualSample> vsample(2);
	//StackMemAlloc<ArgsSample> sample(2);
	func2();
	return 0;
}
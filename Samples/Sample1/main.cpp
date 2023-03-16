// Copyright 2022 Gaocheng Ran <rangaocheng@outlook.com>

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
#include <vector>
#include <memory>

void Fun()
{
	char * k = NOELLE_NEW char[10];
}
void main()
{
	int * a = NOELLE_NEW int;
	*a = 5;
	delete a;
	Fun();

	std::vector<int> vec;
	vec.push_back(1);

	std::unique_ptr<int> sp0;
	std::unique_ptr<int> sp1(NOELLE_NEW int(5));

	sp0 = move(sp1);
}
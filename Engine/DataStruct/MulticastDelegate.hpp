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
#pragma once

#include "Delegate.hpp"

#include <list>

template<typename T>
class MulticastDelegate
{
private:
	static_assert("MulticastDelegate class should accept more than one parameter. Void should also pass through.");
};

template<typename RetVal, typename... Args>
class MulticastDelegate<RetVal(Args...)>
{
public:
	MulticastDelegate() = default;
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
	MulticastDelegate(MulticastDelegate&&) = default;
	MulticastDelegate& operator=(MulticastDelegate&&) = default;
	~MulticastDelegate() = default;

	template<RetVal(*Function)(Args...)>
	void Bind()
	{
		Delegate<RetVal(Args...)> delegate;
		delegate.Bind<Function>();
		m_delegates.emplace_back(std::move(delegate));
	}

	template<typename C, RetVal(C::*Function)(Args...)>
	void Bind(C* pInstance)
	{
		Delegate<RetVal(Args...)> delegate;
		delegate.Bind<C, Function>(pInstance);
		m_delegates.emplace_back(std::move(delegate));
	}

	template<typename C, RetVal(C::*Function)(Args...) const>
	void Bind(const C* pInstance)
	{
		Delegate<RetVal(Args...)> delegate;
		delegate.Bind<C, Function>(pInstance);
		m_delegates.emplace_back(std::move(delegate));
	}

	void Invoke(Args... args) const
	{
		if (m_delegates.empty())
		{
			// assert("MulticastDelegate doesn't bind any function calls.");
			return;
		}

		for(const auto& delegate : m_delegates)
		{
			delegate.Invoke(args...);
		}
	}

private:
	std::list<Delegate<RetVal(Args...)>> m_delegates;
};
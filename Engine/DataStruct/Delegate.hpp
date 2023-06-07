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

#include <assert.h>
#include <functional>

template<typename T>
class Delegate
{
private:
	static_assert("Delegate class should accept more than one parameter. Void should also pass through.");
};

template<typename RetVal, typename... Args>
class Delegate<RetVal(Args...)>
{
private:
	// The ProxyFunction is a function pointer which keeps a same signature
	// for different kinds of function calls.
	using ProxyFuncPtr = RetVal(*)(void*, Args...);

	// Basic functions
	template<RetVal(*Function)(Args...)>
	static RetVal FunctionProxy(void*, Args... args)
	{
		return Function(std::forward<Args>(args)...);
	}

	// Class's non-const methods
	template<typename C, RetVal(C::*Function)(Args...)>
	static RetVal MethodProxy(void* pInstance, Args... args)
	{
		return (static_cast<C*>(pInstance)->*Function)(std::forward<Args>(args)...);
	}

	// Class's const methods
	template<typename C, RetVal(C::*Function)(Args...) const>
	static RetVal ConstMethodProxy(void* pInstance, Args... args)
	{
		return (static_cast<const C*>(pInstance)->*Function)(std::forward<Args>(args)...);
	}

public:
	Delegate() = default;
	Delegate(const Delegate&) = delete;
	Delegate& operator=(const Delegate&) = delete;
	Delegate(Delegate&&) = default;
	Delegate& operator=(Delegate&&) = default;
	~Delegate() = default;

	template<RetVal(*Function)(Args...)>
	void Bind()
	{
		m_pInstance = nullptr;
		m_pProxyFunc = &FunctionProxy<Function>;
	}

	template<typename C, RetVal(C::*Function)(Args...)>
	void Bind(C* pInstance)
	{
		m_pInstance = pInstance;
		m_pProxyFunc = &MethodProxy<C, Function>;
	}

	template<typename C, RetVal(C::*Function)(Args...) const>
	void Bind(const C* pInstance)
	{
		m_pInstance = const_cast<C*>(pInstance);
		m_pProxyFunc = &ConstMethodProxy<C, Function>;
	}

	RetVal Invoke(Args... args) const
	{
		assert(m_pProxyFunc != nullptr && "Cannot invoke unbound Delegate. Call Bind() first.");
		return m_pProxyFunc(m_pInstance, std::forward<Args>(args)...);
	}

private:
	void* m_pInstance = nullptr;
	ProxyFuncPtr m_pProxyFunc = nullptr;
};
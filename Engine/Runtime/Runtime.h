#pragma once

#ifdef RUNTIME_EXPORTS
	#define RUNTIME_API __declspec(dllexport) 
#else 
	#define RUNTIME_API __declspec(dllimport) 
#endif
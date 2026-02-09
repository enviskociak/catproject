#ifndef CHOOKS_HPP_GUARD
#define CHOOKS_HPP_GUARD
#pragma once

#include <Windows.h>
#include <vector>

static __forceinline void AddPageGuardProtect(void* addr) {
	DWORD old;
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);
	VirtualQuery(addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VirtualProtect(addr, sysInfo.dwPageSize, mbi.Protect | PAGE_GUARD, &old);
}

static __forceinline void RemovePageGuardProtect(void* addr) {
	DWORD old;
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);
	VirtualQuery(addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VirtualProtect(addr, sysInfo.dwPageSize, mbi.Protect & ~PAGE_GUARD, &old);
}

typedef struct CIntelligentHookedFunction {
	explicit CIntelligentHookedFunction(void* from, void* to, void** original) : _from(from), _to(to), _original(original) {}

	void* _from, * _to, ** _original;
} CIntelligentHookedFunction;

typedef struct CHooks {
	static void Init();
	static std::vector<CIntelligentHookedFunction> GetHookedFunctions() {
		return hookedFunctions;
	}
	static void CreateHook(_In_ void* _Function, _In_ void* _Hooked, _In_ void** _Original) {
		AddPageGuardProtect(_Function);
		hookedFunctions.push_back(CIntelligentHookedFunction{ _Function, _Hooked, _Original });
	}

	static std::vector<CIntelligentHookedFunction> hookedFunctions; //would cause nullptr exceptions without {} initializer
} Hooks;

long __stdcall UnhandledExceptionHandler(EXCEPTION_POINTERS* ep) {
	if (ep->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION) {
#ifdef _WIN64
		auto ip = ep->ContextRecord->Rip;
#else
		auto ip = ep->ContextRecord->Eip;
#endif

		for (auto func : CHooks::GetHookedFunctions()) {
			if (ip == (unsigned long long) func._from) {
#ifdef _WIN64
				* func._original = (void*)(ep->ContextRecord->Rip);
				ep->ContextRecord->Rip = (unsigned long long) func._to;
#else
				* func._original = (void*)(ep->ContextRecord->Eip);
				ep->ContextRecord->Eip = (unsigned long long) func._to;
#endif
				break;
			}
		}

		ep->ContextRecord->EFlags |= 0x100; //Single step flag

		return EXCEPTION_CONTINUE_EXECUTION;
	}
	if (ep->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP) {
#ifdef _WIN64
		auto ip = ep->ContextRecord->Rip;
#else
		auto ip = ep->ContextRecord->Eip;
#endif

		for (auto func : CHooks::GetHookedFunctions()) {
			if (ip == (unsigned long long) func._from) {
				AddPageGuardProtect(func._from);
				break;
			}
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void CHooks::Init() {
	AddVectoredExceptionHandler(1, &UnhandledExceptionHandler);
	hookedFunctions = {};
}

#endif //CHOOKS_HPP_GUARD
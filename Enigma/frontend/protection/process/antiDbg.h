#pragma once
#include <windows.h>
//#include <winternl.h>
#include <thread>
#include <iostream>
#include "obfuscator.hpp"
#include "anti_vm.h"

typedef NTSTATUS(__stdcall* _NtQueryInformationProcess)(_In_ HANDLE, _In_  unsigned int, _Out_ PVOID, _In_ ULONG, _Out_ PULONG);
typedef NTSTATUS(__stdcall* _NtSetInformationThread)(_In_ HANDLE, _In_ THREAD_INFORMATION_CLASS, _In_ PVOID, _In_ ULONG);
typedef NTSTATUS(WINAPI* lpQueryInfo)(HANDLE, LONG, PVOID, ULONG, PULONG);

bool rape = false;

inline void crash(const char* text) {
	std::cout << _("error : ") << text << std::endl;
	LI_FN(Sleep).get()(3000);
	*(uintptr_t*)(0) = 1;
}

inline DWORD anti_suspend() //aka anti pause
{
	static DWORD TimeTest1 = 0, TimeTest2 = 0;
	TimeTest1 = TimeTest2;
	TimeTest2 = LI_FN(GetTickCount).forwarded_safe_cached()();
	if (TimeTest1 != 0)
		if (TimeTest2 - TimeTest1 > 6000) {
			crash(_("Time"));
			*(uintptr_t*)(0) = 1;
		}
	return 0;
}

inline bool hide_thread(HANDLE thread)
{
	typedef NTSTATUS(NTAPI* pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
	NTSTATUS Status;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)LI_FN(GetProcAddress).forwarded_safe_cached()((LI_FN(GetModuleHandleA).forwarded_safe_cached())(_("ntdll.dll")), _("NtSetInformationThread"));

	if (NtSIT == NULL) return false;
	if (thread == NULL)
		Status = NtSIT(LI_FN(GetCurrentThread).forwarded_safe_cached(), 0x11, 0, 0);
	else
		Status = NtSIT(thread, 0x11, 0, 0);

	if (Status != 0x00000000)
		return false;
	else
		return true;
}
inline int hide_loader_thread()
{
	unsigned long thread_hide_from_debugger = 0x11;

	const auto ntdll = LI_FN(LoadLibraryA).forwarded_safe_cached()(_("ntdll.dll"));

	if (ntdll == INVALID_HANDLE_VALUE || ntdll == NULL) { return false; }

	_NtQueryInformationProcess NtQueryInformationProcess = NULL;
	NtQueryInformationProcess = (_NtQueryInformationProcess)LI_FN(GetProcAddress).forwarded_safe_cached()(ntdll, _("NtQueryInformationProcess"));

	if (NtQueryInformationProcess == NULL) { return false; }

	(_NtSetInformationThread)(LI_FN(GetCurrentThread).forwarded_safe_cached(), thread_hide_from_debugger, 0, 0, 0);

	return true;
}
inline int thread_context()
{
	int found = false;
	CONTEXT ctx = { 0 };
	void* h_thread = LI_FN(GetCurrentThread).forwarded_safe_cached();

	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (LI_FN(GetThreadContext).forwarded_safe_cached()(h_thread, &ctx))
	{
		if ((ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00) || (ctx.Dr7 != 0x00))
		{
			found = true;
		}
	}

	return found;
}
inline int ollydbg_exploit() {
	__try {
		LI_FN(OutputDebugStringA).forwarded_safe_cached()(_("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"));
	}
	__except (EXCEPTION_EXECUTE_HANDLER) { ; }

	return 0;
}
inline int last_error()
{
	LI_FN(SetLastError).forwarded_safe_cached()(0);
	const auto last_error = LI_FN(GetLastError).forwarded_safe_cached()();

	return last_error != 0;
}
inline int remote_is_present()
{
	int debugger_present = false;

	LI_FN(CheckRemoteDebuggerPresent).forwarded_safe_cached()(LI_FN(GetCurrentProcess).forwarded_safe_cached()(), &debugger_present);

	return debugger_present;
}
inline bool check12()
{
	UCHAR* pMem = NULL;
	SYSTEM_INFO SystemInfo = { 0 };
	DWORD OldProtect = 0;
	PVOID pAllocation = NULL;

	LI_FN(GetSystemInfo).forwarded_safe_cached()(&SystemInfo);

	pAllocation = LI_FN(VirtualAlloc).forwarded_safe_cached()(NULL, SystemInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pAllocation == NULL)
		return FALSE;

	RtlFillMemory(pAllocation, 1, 0xC3);

	if (LI_FN(VirtualProtect).forwarded_safe_cached()(pAllocation, SystemInfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect) == 0)
		return FALSE;

	__try
	{
		((void(*)())pAllocation)();
	}
	__except (GetExceptionCode() == STATUS_GUARD_PAGE_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		LI_FN(VirtualFree).forwarded_safe_cached()(pAllocation, NULL, MEM_RELEASE);
		return FALSE;
	}

	LI_FN(VirtualFree).forwarded_safe_cached()(pAllocation, NULL, MEM_RELEASE);
	return TRUE;
}
inline bool check2()
{
	PCONTEXT ctx = PCONTEXT(LI_FN(VirtualAlloc).forwarded_safe_cached()(NULL, sizeof(ctx), MEM_COMMIT, PAGE_READWRITE));
	RtlSecureZeroMemory(ctx, sizeof(CONTEXT));

	ctx->ContextFlags = CONTEXT_DEBUG_REGISTERS;

	if (LI_FN(GetThreadContext).forwarded_safe_cached()(LI_FN(GetCurrentThread).forwarded_safe_cached()(), ctx) == 0)
		return -1;


	if (ctx->Dr0 != 0 || ctx->Dr1 != 0 || ctx->Dr2 != 0 || ctx->Dr3 != 0)
		return TRUE;
	else
		return FALSE;
}

//inline int null_size()
//{
//	const auto peb = (PPEB)__readgsqword(0x60);
//
//	const auto in_load_order_module_list = (PLIST_ENTRY)peb->Ldr->Reserved2[1];
//	const auto table_entry = CONTAINING_RECORD(in_load_order_module_list, LDR_DATA_TABLE_ENTRY, Reserved1[0]);
//	const auto p_size_of_image = (PULONG)&table_entry->Reserved3[1];
//	*p_size_of_image = (ULONG)((INT_PTR)table_entry->DllBase + 0x100000);
//
//	return 0;
//}
inline bool system_check()
{
	typedef NTSTATUS(__stdcall* td_NtQuerySystemInformation)(
		ULONG           SystemInformationClass,
		PVOID           SystemInformation,
		ULONG           SystemInformationLength,
		PULONG          ReturnLength
		);

	struct SYSTEM_CODEINTEGRITY_INFORMATION {
		ULONG Length;
		ULONG CodeIntegrityOptions;
	};

	static td_NtQuerySystemInformation NtQuerySystemInformation = (td_NtQuerySystemInformation)LI_FN(GetProcAddress).forwarded_safe_cached()(LI_FN(GetModuleHandleA).forwarded_safe_cached()(_("ntdll.dll")), _("NtQuerySystemInformation"));

	SYSTEM_CODEINTEGRITY_INFORMATION Integrity = { sizeof(SYSTEM_CODEINTEGRITY_INFORMATION), 0 };
	NTSTATUS status = NtQuerySystemInformation(103, &Integrity, sizeof(Integrity), NULL);

	return (status && (Integrity.CodeIntegrityOptions & 1));
}
inline void window_check()
{
	if (LI_FN(FindWindowA).forwarded_safe_cached()(("Window"), NULL))
	{
		//std::cout << "Window\n";
		BlackList();
	}
	/*if (LI_FN(FindWindowA).forwarded_safe_cached()(("Qt5QWindowIcon"), NULL))
	{
		std::cout << "Qt5QWindowIcon\n";
		BlackList();
	}*/
	if (LI_FN(FindWindowA).forwarded_safe_cached()(_("PROCEXPL"), NULL))
	{
		//std::cout << "PROCEXPL\n";
		BlackList();
	}
	if (LI_FN(FindWindowA).forwarded_safe_cached()(_("dbgviewClass"), NULL))
	{
		//std::cout << "dbgviewClass\n";
		BlackList();
	}
	if (LI_FN(FindWindowA).forwarded_safe_cached()(_("XTPMainFrame"), NULL))
	{
		//std::cout << "XTPMainFrame\n";
		BlackList();
	}
	if (LI_FN(FindWindowA).forwarded_safe_cached()(_("WdcWindow"), _("Resource Monitor")))
	{
		//std::cout << "WdcWindow\n";
		BlackList();
	}
}
void antidump()
{
	DWORD Protect = 0;
	char* pBaseAddr = (char*)GetModuleHandle(NULL);
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &Protect);
	ZeroMemory(pBaseAddr, 4096);
}
inline bool close_handle()
{
	__try {
		LI_FN(CloseHandle).forwarded_safe_cached()((HANDLE)0x13333337);
	}
	__except (STATUS_INVALID_HANDLE) {
		return TRUE;
	}
}
inline bool thread_hide_debugger()
{
	typedef NTSTATUS(WINAPI* pNtSetInformationThread)(IN HANDLE, IN UINT, IN PVOID, IN ULONG);

	const int ThreadHideFromDebugger = 0x11;
	pNtSetInformationThread NtSetInformationThread = NULL;

	NTSTATUS Status;
	BOOL IsBeingDebug = FALSE;

	HMODULE hNtDll = LI_FN(LoadLibraryA).forwarded_safe_cached()(_("ntdll.dll"));
	NtSetInformationThread = (pNtSetInformationThread)LI_FN(GetProcAddress).forwarded_safe_cached()(hNtDll, _("NtSetInformationThread"));
	Status = NtSetInformationThread(LI_FN(GetCurrentThread).forwarded_safe_cached()(), ThreadHideFromDebugger, NULL, 0);

	if (Status)
		*(uintptr_t*)(0) = 1;

	return IsBeingDebug;
}

inline void hardware_register()
{
	BOOL found = FALSE;
	CONTEXT ctx = { 0 };
	HANDLE hThread = LI_FN(GetCurrentThread).forwarded_safe_cached()();

	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (LI_FN(GetThreadContext).forwarded_safe_cached()(hThread, &ctx))
	{
		if ((ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00) || (ctx.Dr7 != 0x00))
		{
			found = TRUE;
		}
	}

	if (found)
	{
		crash(_("rgst"));
	}
}

bool anti_injection_vm()
{
	MEMORY_BASIC_INFORMATION mbi;
	DWORD oldProtect;

	HMODULE moduleHandle = GetModuleHandle(NULL);
	LPVOID moduleBaseAddress = moduleHandle;

	if (VirtualQuery(moduleBaseAddress, &mbi, sizeof(mbi)) == 0)
	{
		return true;
	}

	if (VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READONLY, &oldProtect) == 0)
	{
		return true;
	}

	return false;
}

inline int hardware_breakpoints()
{
	unsigned int NumBps = 0;

	CONTEXT ctx;
	RtlSecureZeroMemory(&ctx, sizeof(CONTEXT));

	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	HANDLE hThread = LI_FN(GetCurrentThread).forwarded_safe_cached()();

	if (LI_FN(GetThreadContext).forwarded_safe_cached()(hThread, &ctx) == 0)
		crash(_("brkp"));

	if (ctx.Dr0 != 0)
		++NumBps;
	if (ctx.Dr1 != 0)
		++NumBps;
	if (ctx.Dr2 != 0)
		++NumBps;
	if (ctx.Dr3 != 0)
		++NumBps;

	return NumBps;
}

void security_loop()
{
	hide_thread(LI_FN(GetCurrentThread).forwarded_safe_cached()());
	thread_hide_debugger();
	hide_loader_thread();

	while (true)
	{
		anti_suspend();
		check_processes();
		obfuscator();
		window_check();

		ollydbg_exploit();
		hardware_breakpoints();
		hardware_register();
		vmware_check();
		virtual_box_drivers();
		virtual_box_registry();

		if (check12())
		{
			//std::cout << "check12\n";
			BlackList();
		}
		if (check2())
		{
			//std::cout << "check2\n";
			BlackList();
		}
		if (last_error())
		{
			//std::cout << "last_error\n";
			BlackList();
		}
		if (close_handle())
		{
			//std::cout << "close_handle\n";
			BlackList();
		}

		if (thread_context())
		{
			//std::cout << "thread_context\n";
			BlackList();
		}

		if (anti_injection_vm())
		{
			//std::cout << "anti_injection_vm\n";
			BlackList();
		}

		if (remote_is_present())
		{
			//std::cout << "remote_is_present\n";
			BlackList();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
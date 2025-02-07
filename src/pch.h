// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include "../Include/GLFW/glfw3.h"
#include "../Include/ImGui/imconfig.h"
#include "../Include/ImGui/imgui.h"
#include "../Include/ImGui/backends/imgui_impl_opengl3.h";
#include "../Include/ImGui/backends/imgui_impl_win32.h";

#endif //PCH_H

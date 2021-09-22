#pragma once 

#include <imgui/imgui.h>

namespace RealEngine {

    class LogWindow
    {
    public:

        ~LogWindow() {};
        
        void Clear();
        void AddLog(std::string fmt, ...);
        void OnImGuiRender();

        static LogWindow& Get() { return *s_Instance; }
    private:
        LogWindow();
    public:
        static LogWindow* s_Instance;

        ImGuiTextBuffer m_Buffer;
        ImGuiTextFilter m_Filter;
        ImVector<int> m_LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool s_AutoScroll;  // Keep scrolling if already at the bottom.
    };

}
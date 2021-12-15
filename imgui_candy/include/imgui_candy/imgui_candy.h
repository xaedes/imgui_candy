#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

template<class U>
struct ImGuiDataTypeOfPtr_
{
    template <class T> ImGuiDataType operator()( T*   val );
    template <> ImGuiDataType operator()( int8_t*   val ) { return ImGuiDataType_S8;     }
    template <> ImGuiDataType operator()( uint8_t*  val ) { return ImGuiDataType_U8;     }
    template <> ImGuiDataType operator()( int16_t*  val ) { return ImGuiDataType_S16;    }
    template <> ImGuiDataType operator()( uint16_t* val ) { return ImGuiDataType_U16;    }
    template <> ImGuiDataType operator()( int32_t*  val ) { return ImGuiDataType_S32;    }
    template <> ImGuiDataType operator()( uint32_t* val ) { return ImGuiDataType_U32;    }
    template <> ImGuiDataType operator()( int64_t*  val ) { return ImGuiDataType_S64;    }
    template <> ImGuiDataType operator()( uint64_t* val ) { return ImGuiDataType_U64;    }
    template <> ImGuiDataType operator()( float*    val ) { return ImGuiDataType_Float;  }
    template <> ImGuiDataType operator()( double*   val ) { return ImGuiDataType_Double; }
};

template<typename T>
bool ImGuiSliderScalar(const char* label, T* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGui::SliderScalar(label, ImGuiDataTypeOfPtr_<T>()(p_data), p_data, p_min, p_max, format, flags);
}
template<typename T>
bool ImGuiSliderScalar(const char* label, T* p_data, T p_min, T p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScalar(label, p_data, &p_min, &p_max, format, flags);
}

template<typename T>
bool ImGuiSliderScalarN(const char* label, T* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGui::SliderScalarN(label, ImGuiDataTypeOfPtr_<T>()(p_data), p_data, components, p_min, p_max, format, flags);
}
template<typename T>
bool ImGuiSliderScalarN(const char* label, T* p_data, int components, T p_min, T p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScalarN(label, p_data, components, &p_min, &p_max, format, flags);
}

template<typename T, typename F>
bool ImGuiSliderScaledScalar(const char* label, T* p_data, F scale, const void* p_scaled_min, const void* p_scaled_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    F scaled_data = (*p_data) / (scale);
    bool changed = ImGui::SliderScalar(label, ImGuiDataTypeOfPtr_<F>()(&scaled_data), &scaled_data, p_scaled_min, p_scaled_max, format, flags);
    if (changed)
    {
        *p_data = static_cast<T>(scaled_data * scale);
    }
    return changed;
}
template<typename T, typename F>
bool ImGuiSliderScaledScalar(const char* label, T* p_data, F scale, F p_scaled_min, F p_scaled_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScaledScalar(label, p_data, scale, &p_scaled_min, &p_scaled_max, format, flags);
}

template<typename T, typename F>
bool ImGuiSliderScaledScalarN(const char* label, T* p_data, int components, const F scale, const void* p_scaled_min, const void* p_scaled_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    static thread_local std::vector<F> scaled_data;
    scaled_data.clear();
    for (int i=0; i<components; ++i)
    {
        scaled_data.push_back(p_data[i] / scale);
    }
    F* p_scaled = &scaled_data[0];

    bool changed = ImGui::SliderScalarN(label, ImGuiDataTypeOfPtr_<F>()(p_scaled), p_scaled, components, p_scaled_min, p_scaled_max, format, flags);
    if (changed)
    {
        for (int i=0; i<components; ++i)
        {
            p_data[i] = static_cast<T>(scaled_data[i] * scale);
        }
    }
}
template<typename T, typename F>
bool ImGuiSliderScaledScalarN(const char* label, T* p_data, int components, F scale, T p_min, T p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScaledScalarN(label, p_data, components, scale, &p_min, &p_max, format, flags);
}

template<typename T, typename F>
bool ImGuiSliderScaledScalarN(const char* label, T* p_data, int components, const F* scale, const void* p_scaled_min, const void* p_scaled_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    static thread_local std::vector<F> scaled_data;
    scaled_data.clear();
    for (int i=0; i<components; ++i)
    {
        scaled_data.push_back(p_data[i] / scale[i]);
    }
    F* p_scaled = &scaled_data[0];

    bool changed = ImGui::SliderScalarN(label, ImGuiDataTypeOfPtr_<F>()(p_scaled), p_scaled, components, p_scaled_min, p_scaled_max, format, flags);
    if (changed)
    {
        for (int i=0; i<components; ++i)
        {
            p_data[i] = static_cast<T>(scaled_data[i] * scale[i]);
        }
    }
    return changed;
}
template<typename T, typename F>
bool ImGuiSliderScaledScalarN(const char* label, T* p_data, int components, F* scale, T p_min, T p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScaledScalarN(label, p_data, components, scale, &p_min, &p_max, format, flags);
}

inline bool ImGuiButton(const char* label, bool enabled = true, const ImVec2& size = ImVec2(0, 0))
{
    // https://github.com/ocornut/imgui/issues/1889#issuecomment-398681105
    if (!enabled)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    bool result = ImGui::Button(label, size);
    if (!enabled)
    {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    return result;
}

template <class...Args>
void ImGuiText(const std::string& str, Args&&... args)
{
    ImGui::Text(str.c_str(), std::forward<Args>(args)...);
}

namespace ImGuiCandy {

template <class T>
std::string append_id(const std::string& label, const T* ptr)
{
    static std::stringstream ss;
    ss.str("");
    // const void* ptr = &value;
    ss << label << "##";
    ss << "0x" << std::uppercase << std::setfill('0') << std::setw(sizeof(ptr)*2) << std::hex << ptr;
    return ss.str();
}

}

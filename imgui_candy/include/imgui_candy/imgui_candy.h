#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct ImGuiDataTypeOfPtr
{
    static ImGuiDataType Call( int8_t*   val ) { return ImGuiDataType_S8;     }
    static ImGuiDataType Call( uint8_t*  val ) { return ImGuiDataType_U8;     }
    static ImGuiDataType Call( int16_t*  val ) { return ImGuiDataType_S16;    }
    static ImGuiDataType Call( uint16_t* val ) { return ImGuiDataType_U16;    }
    static ImGuiDataType Call( int32_t*  val ) { return ImGuiDataType_S32;    }
    static ImGuiDataType Call( uint32_t* val ) { return ImGuiDataType_U32;    }
    static ImGuiDataType Call( int64_t*  val ) { return ImGuiDataType_S64;    }
    static ImGuiDataType Call( uint64_t* val ) { return ImGuiDataType_U64;    }
    static ImGuiDataType Call( float*    val ) { return ImGuiDataType_Float;  }
    static ImGuiDataType Call( double*   val ) { return ImGuiDataType_Double; }
    
    template<
        class T,
        std::enable_if_t<std::is_same<T,long long>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int32_t>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int64_t>::value, bool> = true
    >
    static ImGuiDataType Call( T* val)  { return (sizeof(T) == 32) ? ImGuiDataType_S32 : ImGuiDataType_S64; }
    template<
        class T,
        std::enable_if_t<std::is_same<T,long>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int32_t>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int64_t>::value, bool> = true
    >
    static ImGuiDataType Call( T* val)  { return (sizeof(T) == 32) ? ImGuiDataType_S32 : ImGuiDataType_S64; }
    template<
        class T,
        std::enable_if_t<std::is_same<T,unsigned long long>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int32_t>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int64_t>::value, bool> = true
    >
    static ImGuiDataType Call( T* val)  { return (sizeof(T) == 32) ? ImGuiDataType_S32 : ImGuiDataType_S64; }
    template<
        class T,
        std::enable_if_t<std::is_same<T,unsigned long>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int32_t>::value, bool> = true,
        std::enable_if_t<!std::is_same<T,int64_t>::value, bool> = true
    >
    static ImGuiDataType Call( T* val)  { return (sizeof(T) == 32) ? ImGuiDataType_S32 : ImGuiDataType_S64; }
};


template<typename T>
bool ImGuiSliderScalar(const char* label, T* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGui::SliderScalar(label, ImGuiDataTypeOfPtr::Call(p_data), p_data, p_min, p_max, format, flags);
}
template<typename T>
bool ImGuiSliderScalar(const char* label, T* p_data, T p_min, T p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScalar(label, p_data, &p_min, &p_max, format, flags);
}

template<typename T>
bool ImGuiSliderScalarN(const char* label, T* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGui::SliderScalarN(label, ImGuiDataTypeOfPtr::Call(p_data), p_data, components, p_min, p_max, format, flags);
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
    bool changed = ImGui::SliderScalar(label, ImGuiDataTypeOfPtr::Call(&scaled_data), &scaled_data, p_scaled_min, p_scaled_max, format, flags);
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

    bool changed = ImGui::SliderScalarN(label, ImGuiDataTypeOfPtr::Call(p_scaled), p_scaled, components, p_scaled_min, p_scaled_max, format, flags);
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

    bool changed = ImGui::SliderScalarN(label, ImGuiDataTypeOfPtr::Call(p_scaled), p_scaled, components, p_scaled_min, p_scaled_max, format, flags);
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

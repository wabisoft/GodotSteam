#pragma once

#include <algorithm>
#include <string_view>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/defs.hpp>


_FORCE_INLINE_
std::ostream& operator<<(std::ostream& o, const godot::String& in)
{
    o << in.ptr();
    return o;
}
namespace wabisoft
{
    namespace utils
    {

        template<typename T, typename Iterator, typename Predicate>
        T* ptrfind_if(Iterator begin, Iterator end, Predicate predicate)
        {
            auto it = std::find_if(container.begin(), container.end(), predicate);
            if(it == end)
            {
                return nullptr;
            }
            return &*it;
        }

        _FORCE_INLINE_ 
        uint64_t getTicksMS()
        {
            return godot::Time::get_singleton()->get_ticks_msec();
        }

        _FORCE_INLINE_ 
        uint64_t getProcessFrames()
        {
            return godot::Engine::get_singleton()->get_process_frames();
        }

        _FORCE_INLINE_
        godot::String getDateTimeStringFromSystem(bool utc = false, bool use_space = false)
        {
            return godot::Time::get_singleton()->get_datetime_string_from_system(utc, use_space);
        }

        namespace variants
        {
            _FORCE_INLINE_
            godot::Variant toVariant(const std::string& s)
            {
                return godot::Variant(s.c_str());
            }
        }
    }
}
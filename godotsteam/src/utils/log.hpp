#pragma once

#include "utils.hpp"
#include <iostream>

namespace wabisoft
{
    namespace log
    {
        enum class Level 
        {
            CRITICAL = 0,
            ERROR,
            WARN,
            INFO,
            DEBUG
        };

        struct CallContext
        {
            std::string_view function_;
            std::string_view file_;
            uint32_t line_;
        };

        
        std::string logHeading(CallContext, Level level);
        template<typename T>
        void log_internal(CallContext cc, Level level, const T& logged)  // TODO: (owen) support formatting and variadic args
        {
            godot::Variant v1 = utils::variants::toVariant(logHeading(cc, level));
            godot::Variant v2 = utils::variants::toVariant(logged);
            godot::UtilityFunctions::print(v1, v2);
        }

    }
}
#define CALL_CONTEXT wabisoft::log::CallContext{std::string_view(__func__), std::string_view(__FILE__), __LINE__}
#define log(level, ...) log_internal(CALL_CONTEXT, wabisoft::log::Level::level, __VA_ARGS__)
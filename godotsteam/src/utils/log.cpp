#include "log.hpp"
#include "utils.hpp"
#include <sstream>


constexpr size_t kMaxFileNameSize = 35;

namespace
{
    constexpr const char* levelStrs[] =
    {
        "CRIT",
        "ERR",
        "WARN",
        "INFO",
        "DEBUG"
    };
}

namespace wabisoft
{
    namespace log
    {
        std::string logHeading(CallContext cc, Level level)
        {
            // TODO: (owen) use fmt
            std::stringstream ss;
            ss << "[" << wabisoft::utils::getDateTimeStringFromSystem(true, false).utf8().get_data() << "]"
                << "[" << wabisoft::utils::getProcessFrames() << "]"
                << "[" << levelStrs[static_cast<size_t>(level)] << "]";
            if(cc.file_.size() > kMaxFileNameSize)
            {
                ss << cc.file_.substr(cc.file_.size() - kMaxFileNameSize);
            }
            else
            {

                ss << cc.file_;
            }
            ss << "(" << cc.line_  << "): ";
            return ss.str();
        }
    }
}
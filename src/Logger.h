//
// Created by samox on 1/8/21.
//

#ifndef PACMINDER_LOGGER_H
#define PACMINDER_LOGGER_H

#include <boost/move/utility_core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using severity_level = boost::log::trivial::severity_level;

class Logger {
    src::severity_logger<severity_level> lg;

public:

    Logger() {
//        logging::add_file_log(
//                keywords::file_name = "/var/log/pacminder.log",
//                keywords::rotation_size = 10 * 1024 * 1024,
//                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
//                keywords::format = "[%TimeStamp%] [%Severity%] %Message%"
//        );
        logging::add_console_log(std::cout, keywords::format = "[%TimeStamp%] [%Severity%] %Message%");
        logging::add_common_attributes();
    }

    void info(const char *s) {
        logging::record rec = lg.open_record(keywords::severity = severity_level::info);
        if (!rec) throw std::runtime_error("Could not open record for logging.");
        logging::record_ostream stream(rec);
        stream << s;
        stream.flush();
        lg.push_record(boost::move(rec));
    }

    void info(std::string s) {
        this->info(s.c_str());
    }
};

#endif //PACMINDER_LOGGER_H

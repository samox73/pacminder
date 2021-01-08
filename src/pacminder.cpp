#include <pwd.h>
#include <unistd.h>

#define BOOST_NO_CXX11_SCOPED_ENUMS

#include <boost/filesystem.hpp>

#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include <ctime>
#include <iomanip>
#include <iostream>
#include <libconfig.h++>
#include <regex>
#include "ParameterParser.h"
#include "Logger.h"

namespace fs = boost::filesystem;
namespace lc = libconfig;

int main(int argc, char *argv[]) {
    ParameterParser parser{argc, argv};
    bool verbose = parser.isVerbose();
    Logger logger;

    // --------------- CONFIG STUFF ---------------
    const char *h;
    // get home directory of user
    if ((h = getenv("HOME")) == nullptr) {
        h = getpwuid(getuid())->pw_dir;
    }
    std::string homedir{h};
    std::string cfgDir{homedir + "/.config/pacminder"};
    std::string cfgFile{cfgDir + "/config"};

    if (verbose) {
        logger.info("Home directory: '" + homedir + "'");
        logger.info("Using config file: '" + cfgFile + "'");
    }

    // create directories and config file if not already present
    fs::create_directories(cfgDir);
    if (!fs::exists(cfgFile)) {
        std::ofstream cfgOS{cfgFile};
        cfgOS << "";
        cfgOS.close();
    }

    // create config object
    lc::Config cfg;
    cfg.setOptions(lc::Config::OptionFsync |
                   lc::Config::OptionSemicolonSeparators |
                   lc::Config::OptionColonAssignmentForGroups |
                   lc::Config::OptionOpenBraceOnSeparateLine);

    // read file from the config directory
    try {
        cfg.readFile(cfgFile.c_str());
    } catch (const lc::FileIOException &fioex) {
        std::cerr << "I/O error while reading file." << std::endl;
        return (EXIT_FAILURE);
    } catch (const lc::ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return (EXIT_FAILURE);
    }

    // get root Setting
    lc::Setting &root = cfg.getRoot();

    // add config stuff if not present
    if (!root.exists("general")) root.add("general", lc::Setting::TypeGroup);
    lc::Setting &general = root["general"];
    if (!general.exists("max-days"))
        general.add("max-days", lc::Setting::TypeInt) = 1;
    if (!general.exists("max-hours"))
        general.add("max-hours", lc::Setting::TypeInt) = 1;
    if (!general.exists("update-message"))
        general.add("update-message", lc::Setting::TypeString) =
                "Update pacman you little shit!";

    // write config file back to filesystem
    try {
        cfg.writeFile(cfgFile.c_str());
    } catch (const lc::FileIOException &fioex) {
        std::cerr << "I/O error while writing file: " << cfgFile << std::endl;
        return (EXIT_FAILURE);
    }

    // -========= PACMAN LOG STUFF =========-
    // open pacman log
    // TODO: field in config file
    std::ifstream pacmanLog{"/var/log/pacman.log"};
    std::regex pattern("starting full system upgrade");
    std::string time_line{""};
    if (pacmanLog.good()) {
        std::string line;
        while (getline(pacmanLog, line)) {
            // get last line which matches the pattern
            if (std::regex_search(line, pattern)) {
                time_line = line;
            }
        }
    }
    std::regex time_regex(R"(\[(\d*-\d*-\d*T\d*:\d*:\d*).\d*\].*)");
    std::smatch match;
    // write match into #match
    std::regex_match(time_line, match, time_regex);

    // get first group of regex match, [0] is whole match, [1] the first subgroup
    std::stringstream ss(match[1]);
    std::tm tm{};
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    auto lastUpdate = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto time = std::chrono::system_clock::to_time_t(lastUpdate);

    // get difference since last update
    auto now = std::chrono::system_clock::now();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdate)
            .count();
    auto days = secs / (60 * 60 * 24);
    auto hours = (secs / (60 * 60)) % 24;
    auto minutes = (secs / (60)) % 60;
    int maxDays;
    int maxHrs;

    parser.displayInfo(days, hours, minutes);

    // read values from config
    std::string updateMessage;
    general.lookupValue("max-days", maxDays);
    general.lookupValue("max-hours", maxHrs);
    general.lookupValue("update-message", updateMessage);

    // print message if last update is too long back
    if (days > maxDays || (days == maxDays && hours > maxHrs)) {
        std::cout << updateMessage << std::endl;
        std::cout << "Last update was on:\n    "
                  << std::put_time(std::localtime(&time), "%c %Z") << std::endl;
        std::cout << "Which was " << days << " days, " << hours << " hours and "
                  << minutes << " minutes ago." << std::endl;
        std::cout << "(Max time without update is " << maxDays << " days and "
                  << maxHrs << " hours.)\n"
                  << std::endl;
    }

    return (EXIT_SUCCESS);
}

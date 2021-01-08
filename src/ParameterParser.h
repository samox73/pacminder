//
// Created by samox on 1/8/21.
//

#ifndef PACMINDER_PARAMETERPARSER_H
#define PACMINDER_PARAMETERPARSER_H

#include <boost/program_options.hpp>
#include "Codes.h"

namespace po = boost::program_options;

class ParameterParser {
    po::variables_map variablesMap;
    po::options_description description{"Allowed options"};
public:
    void init() {
        description.add_options()
                (CODES::HELP.c_str(), "print this help message")
                (CODES::INFO.c_str(), "print info about last pacman update")
                (CODES::VERBOSE.c_str(), "")
                ;
    }

    void displayInfo(size_t days, size_t hours, size_t minutes) {
        if(variablesMap.count(CODES::INFO_SHORT) || variablesMap.count(CODES::INFO_LONG)) {
            if(variablesMap.count(CODES::VERBOSE_SHORT) || variablesMap.count(CODES::VERBOSE_LONG)) {
                std::cout << "Time since last update: " << days << " days, " << hours
                          << " hours and " << minutes << " minutes" << std::endl;
            } else {
                std::cout << "Time since last update: " << days << " days, " << hours
                          << " hours and " << minutes << " minutes" << std::endl;
            }
        }
    }

    ParameterParser(int argc, char **argv) {
        this->init();
        po::store(po::parse_command_line(argc, argv, description), variablesMap);
        po::notify(variablesMap);

        if(variablesMap.count(CODES::HELP_SHORT) || variablesMap.count(CODES::HELP_LONG)) {
            std::cout << description << "\n";
        }
    }

    bool isVerbose() {
        return variablesMap.count(CODES::VERBOSE_SHORT) >= 1 || variablesMap.count(CODES::VERBOSE_LONG) >= 1;
    }
};

#endif //PACMINDER_PARAMETERPARSER_H

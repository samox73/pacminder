//
// Created by samox on 1/8/21.
//

#ifndef PACMINDER_CODES_H
#define PACMINDER_CODES_H

struct CODES {
    static const std::string HELP;
    static const std::string HELP_SHORT;
    static const std::string HELP_LONG;
    static const std::string INFO;
    static const std::string INFO_SHORT;
    static const std::string INFO_LONG;
    static const std::string VERBOSE;
    static const std::string VERBOSE_SHORT;
    static const std::string VERBOSE_LONG;
};

const std::string CODES::HELP_SHORT = "h";
const std::string CODES::HELP_LONG = "help";
const std::string CODES::HELP = HELP_LONG + ',' + HELP_SHORT;
const std::string CODES::INFO_SHORT = "i";
const std::string CODES::INFO_LONG = "info";
const std::string CODES::INFO = INFO_LONG + ',' + INFO_SHORT;
const std::string CODES::VERBOSE_SHORT = "v";
const std::string CODES::VERBOSE_LONG = "verbose";
const std::string CODES::VERBOSE = VERBOSE_LONG + ',' + VERBOSE_SHORT;

#endif //PACMINDER_CODES_H

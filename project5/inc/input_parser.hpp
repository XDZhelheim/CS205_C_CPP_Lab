#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <algorithm>
#include <string>
#include <vector>

// This class is from
// https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
// to parse commandline args.
// Author: iain

class InputParser {
   private:
    std::vector<std::string> tokens;

   public:
    InputParser(int& argc, char const* argv[]) {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }

    /// @author iain
    const std::string& getCmdOption(const std::string& option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    /// @author iain
    bool cmdOptionExists(const std::string& option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
    }
};

#endif

#pragma once

#include <string>
#include <cmath>

using std::min;
using std::string;

namespace helpers
{
    enum color
    {
        red = 31,
        green = 32,
        blue = 34,
        yellow = 33,
        magenta = 35,
        cyan = 36,
        white = 37,
        black = 30,
        red_bg = 41,
        green_bg = 42,
        blue_bg = 44,
        yellow_bg = 43,
        magenta_bg = 45,
        cyan_bg = 46,
        white_bg = 47,
        black_bg = 40,
        bold = 1,

        reset = 0
    };

    string colorize(string text, color c);
    string stripEndline(string text);
    color nearestColorFromHexCode(string hexcode);
    string strip(string text);
    string randomHexColor();

    class Logger
    {
    public:
        enum verbosity
        {
            silent = -1,
            normal = 0,
            verbose = 1
        };

    private:
        inline static verbosity verbosity_level;

    public:
        static void ln(string text, verbosity v = verbose, color c = reset, color bg = reset, bool bold = false);
        static void log(string text, verbosity v = verbose, color c = reset, color bg = reset, bool bold = false);

        static void setVerbosity(verbosity v)
        {
            verbosity_level = v;
        }
        // static verbosity getVerbosity()
        // {
        //     return verbosity_level;
        // }
    };

}
#pragma once

#include <string>
#include <cmath>

using std::string;
using std::min;

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
}
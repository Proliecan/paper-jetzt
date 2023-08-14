#include "helpers.hpp"

using std::min;
using std::string;

namespace helpers
{
    string colorize(string text, color color)
    {
        return "\033[" + std::to_string(color) + "m" + text + "\033[0m";
    }

    string stripEndline(string text)
    {
        return text.substr(0, text.length() - 1);
    }

    color nearestColorFromHexCode(string hexcode)
    {

        // strip # from hexcode if necessary
        if (hexcode[0] == '#')
        {
            hexcode = hexcode.substr(1, hexcode.length() - 1);
        }

        // convert hexcode to rgb
        int r, g, b;
        sscanf(hexcode.c_str(), "%02x%02x%02x", &r, &g, &b);

        // calculate distance to each color
        int red_dist = pow(r - 255, 2) + pow(g, 2) + pow(b, 2);
        int green_dist = pow(r, 2) + pow(g - 255, 2) + pow(b, 2);
        int blue_dist = pow(r, 2) + pow(g, 2) + pow(b - 255, 2);
        int yellow_dist = pow(r - 255, 2) + pow(g - 255, 2) + pow(b, 2);
        int magenta_dist = pow(r - 255, 2) + pow(g, 2) + pow(b - 255, 2);
        int cyan_dist = pow(r, 2) + pow(g - 255, 2) + pow(b - 255, 2);
        int white_dist = pow(r - 255, 2) + pow(g - 255, 2) + pow(b - 255, 2);
        int black_dist = pow(r, 2) + pow(g, 2) + pow(b, 2);

        // find minimum distance
        int min_dist = min(min(min(min(min(min(min(red_dist, green_dist), blue_dist), yellow_dist), magenta_dist), cyan_dist), white_dist), black_dist);

        // return color
        if (min_dist == red_dist)
        {
            return red;
        }
        else if (min_dist == green_dist)
        {
            return green;
        }
        else if (min_dist == blue_dist)
        {
            return blue;
        }
        else if (min_dist == yellow_dist)
        {
            return yellow;
        }
        else if (min_dist == magenta_dist)
        {
            return magenta;
        }
        else if (min_dist == cyan_dist)
        {
            return cyan;
        }
        else if (min_dist == white_dist)
        {
            return white;
        }
        else if (min_dist == black_dist)
        {
            return black;
        }

        return reset;
    }
}
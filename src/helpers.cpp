#include "helpers.hpp"

using std::min;
using std::string;

#include <iostream>
#include <cassert>

namespace helpers
{
    string colorize(string text, color color)
    {
        return "\033[" + std::to_string(color) + "m" + text + "\033[0m";
    }

    string stripEndline(string text)
    {
        if (text[text.length() - 1] == '\n')
        {
            return text.substr(0, text.length() - 1);
        }
        return text;
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

    string strip(string text)
    {
        string stripped = "";

        for (unsigned int i = 0; i < text.length(); i++)
        {
            if (text[i] != ' ' && text[i] != '\n' && text[i] != '\t')
            {
                stripped += text[i];
            }
        }

        return stripped;
    }

    string randomHexColor()
    {
        string hexcode = "#";
        for (int i = 0; i < 6; i++)
        {
            int rand_num = rand() % 16;
            if (rand_num < 10)
            {
                hexcode += std::to_string(rand_num);
            }
            else
            {
                switch (rand_num)
                {
                case 10:
                    hexcode += "a";
                    break;
                case 11:
                    hexcode += "b";
                    break;
                case 12:
                    hexcode += "c";
                    break;
                case 13:
                    hexcode += "d";
                    break;
                case 14:
                    hexcode += "e";
                    break;
                case 15:
                    hexcode += "f";
                    break;
                }
            }
        }
        return hexcode;
    }

    void Logger::log(string text, verbosity v, color c, color bg, bool bold)
    {
        assert(v != silent && "Cannot log with verbosity level silent");

        // if (v > Logger::getVerbosity())
        if (v > verbosity_level)
        {
            return;
        }

        // using colorize function
        if (bold)
        {
            text = colorize(text, helpers::bold);
        }
        if (c != reset)
        {
            text = colorize(text, c);
        }
        if (bg != reset)
        {
            text = colorize(text, bg);
        }

        std::cout << text;

        std::cout.flush();
    }
    void Logger::ln(string text, verbosity v, color c, color bg, bool bold)
    {
        log(text + "\n", v, c, bg, bold);
    }
}
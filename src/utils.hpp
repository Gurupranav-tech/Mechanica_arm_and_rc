#pragma once

enum class Mode
{
    RC,
    LINE
};

int modeToNum(Mode mode)
{
    if (mode == Mode::LINE)
        return 1;
    else if (mode == Mode::RC)
        return 0;
    else
        return 0;
}
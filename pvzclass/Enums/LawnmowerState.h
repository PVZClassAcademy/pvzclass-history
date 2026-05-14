#pragma once

namespace LawnMowerState
{
    enum LawnMowerState
    {
        RollingIn,
        Ready,
        Triggered,
        Squished,
    };

    extern const char* ToString(LawnMowerState lawnmowert);
}

#pragma once

namespace LawnMowerType
{
    enum LawnMowerType
    {
        LawnCleaner,
        PoolCleaner,
        RoofCleaner,
        Trickedout,
    };

    extern const char* ToString(LawnMowerType lawnmowert);
}

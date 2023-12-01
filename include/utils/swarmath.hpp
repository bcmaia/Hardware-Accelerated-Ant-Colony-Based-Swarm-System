#pragma once

#include <cmath>
#include "sinCosLookup.h"

namespace swarmath {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float INV_PI = 1.0f / PI;
    constexpr float DOUBLE_PI = 6.28318530717958647692f;
    constexpr float HALF_PI = 1.57079632679489661923f;

    class Trig {
        public:
        // NOTE: change those functions according to your needs.
        // use aproximations, original functions or lookup tables

        static float lookup_sin (const float x) {
            int angle = (int)((x / M_PI) * 1800);

            if (angle < 0) angle += 3600;
            else if (angle >= 3600) angle -= 3600;

            return angle;
        }

        /**
         * @brief 
         * 
         * @param x angle in rads
         * @return float 
         */
        static float sin(const float x) {
            return sinf(x);
        }

        static float cos(const float x) {
            return cosf(x);
        }

        static float tan(const float x) {
            return tanf(x);
        }
    };
} // namespace swarmath
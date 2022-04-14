#pragma once

#include <ostream>

namespace orpg {

    struct blocked {
        friend auto &operator<<(std::ostream &os, blocked const &obj) {
            return os << "blocked";
        }
    };
}

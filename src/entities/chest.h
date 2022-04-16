#pragma once

namespace orpg {

    struct chest {
        friend auto &operator<<(std::ostream &os, chest const &obj) {
            return os << "chest";
        }
    };
}

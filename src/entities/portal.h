#pragma once

#include "../point.h"
#include "../tags.h"

namespace orpg {

    //! \todo remove
    struct chest {
        friend auto &operator<<(std::ostream &os, chest const &obj) {
            return os << "var1";
        }
    };

    //! \todo remove
    struct var2 {
        friend auto &operator<<(std::ostream &os, var2 const &obj) {
            return os << "var2";
        }
    };

    struct portal {
        friend auto &operator<<(std::ostream &os, portal const &obj) {
            return os << "portal";
        }

        //! \brief target map
        map_tags_t target{};
        //! \brief target player position in world coordinates
        point position{};
    };
}

#pragma  once

#include <cassert>

#include "sdl_canvas.h"
#include "view.h"

// actions
using view_t = orpg::view<orpg::sdl_canvas>;

const auto update_view = [](view_t& v) {
   v.update();
};

// events
#include "controller/events/time_tick.h"
#include "controller/events/quit.h"
#include "controller/events/key_pressed.h"
#include "controller/events/mouse_motion.h"
#include "controller/events/mouse_button_down.h"
#include "controller/events/finger_down.h"
// guards
#include "controller/guards/is_key.h"
#include "controller/guards/is_mobile.h"

namespace orpg {

   struct controller
   {
      auto operator()() const noexcept
      {
         using namespace sml;

         // states
         auto idle = state<class idle>;
         auto UI = state<class UI>;
         auto update_state = state<class update_class>;

         auto render = (update_view);

         auto update = []() {
            // todo handle logic
            // todo handle animations
         };

         auto empty_func = []() {};

         // src_state + event [ guard ] / action = dst_state
         return make_transition_table(
            (*idle) / render = UI
            // +-----------------------------------------------------------------------------------------------------------------+
            , UI + event<key_pressed>[is_key(SDLK_SPACE)] / [] { std::cout << "space pressed" << std::endl; }
#ifndef NDEBUG 
            , UI + event<key_pressed>[is_key(SDLK_ESCAPE)] = X
#endif
            , UI + event<mouse_motion> / empty_func

            , UI + event<finger_down>[is_mobile] / empty_func
            , UI + event<mouse_button_down>[!is_mobile] / empty_func

            // +-----------------------------------------------------------------------------------------------------------------+
            // time_tick event is called from the mainloop
            , *update_state + event<time_tick> / update
         );
      }
   };
}


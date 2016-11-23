#include <iostream>

// boost experimental
#include <boost/sml.hpp>
#include <boost/sml/utility/dispatch_table.hpp>
namespace sml = boost::sml;
//! \todo use boost:di as well

// orpg
#include "config.h"
#include "sdl_canvas.h"
#include "model.h"
#include "view.h"
#include "controller.h"

int main(int argc, char *argv[])
{
   try
   {
      using namespace orpg;

      config cfg{ "orpg", 800, 600 };
      sdl_canvas canvas{ cfg };
      view_t view{ canvas };

      // State machine constructor provides required dependencies for actions and guards.
      sml::sm<controller> sm{ view };
      do 
      {
         auto dispatch_event = sml::utility::make_dispatch_table<SDL_Event, SDL_QUIT, SDL_MOUSEWHEEL>(sm);
         sm.process_event(time_tick{});

         SDL_Event event;
         while (SDL_PollEvent(&event)) {
            dispatch_event(event, event.type);
         }
      } while (!sm.is(sml::X));

//      game{}.run();
   } 
   catch (std::exception &ex) {
      std::cout << ex.what() << std::endl;
   }
   return 0;
}

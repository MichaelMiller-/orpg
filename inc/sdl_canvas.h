#pragma once

#include <exception>
#include <memory>

#include <SDL.h>

namespace orpg {

class sdl_canvas
{
   std::shared_ptr<SDL_Window> window_;
   std::shared_ptr<SDL_Renderer> renderer_;

public:
   explicit sdl_canvas(config c) 
   {
      if (SDL_Init(SDL_INIT_VIDEO) != 0)
         throw std::runtime_error(std::string{"Unable to initialize SDL: "} + SDL_GetError());

      window_ = std::shared_ptr<SDL_Window>(
         SDL_CreateWindow(
            c.window_title.c_str(), 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 
            c.window_width, 
            c.window_height,
            SDL_WINDOW_SHOWN),
         SDL_DestroyWindow);
      
      renderer_ = std::shared_ptr<SDL_Renderer>(
        SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
   }

   ~sdl_canvas() noexcept 
   {
      SDL_Quit();
   }

   void clear() const noexcept
   {
      SDL_RenderClear(renderer_.get());
   }

   void draw() const noexcept
   {
      SDL_RenderClear(renderer_.get());

      SDL_RenderPresent(renderer_.get());      
   }
};

}

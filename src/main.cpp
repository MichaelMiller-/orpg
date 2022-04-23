#include "components/collision.h"
#include "components/drawable.h"
#include "components/layers.h"
#include "components/movement_speed.h"
#include "components/position.h"
#include "components/triggerable.h"
#include "components/velocity.h"
#include "config.h"
#include "data.h"
#include "entities.h"
#include "point.h"
#include "rect.h"
#include "tags.h"
#include "tile_map.h"
#include "settings.h"
#include "components/player.h"

#define ORPG_IRGNORE_WARNING(e) \

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wenum-compare"
#pragma clang diagnostic ignored "-Wnarrowing"
#pragma clang diagnostic ignored "-Wuseless-cast"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Walloc-size-larger-than="
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wenum-compare"
#pragma GCC diagnostic ignored "-Wnarrowing"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Walloc-size-larger-than="

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#pragma GCC diagnostic pop

#include <spdlog/spdlog.h>

#include <cstdarg>
#include <ranges>
#include <variant>

#include <entt/entt.hpp>

auto operator+(Vector2 const& lhs, Vector2 const& rhs) noexcept { return Vector2{lhs.x + rhs.x, lhs.y + rhs.y}; }

auto operator+(Vector2 const& lhs, orpg::point const& rhs) noexcept
{
   return Vector2{lhs.x + static_cast<decltype(Vector2::x)>(rhs.x), lhs.y + static_cast<decltype(Vector2::y)>(rhs.y)};
}

template <typename T>
auto operator/(Vector2 const& lhs, T const& rhs)
{
   // \todo if (rhs == 0)
   return Vector2{lhs.x / rhs, lhs.y / rhs};
}

template <class... Ts>
struct overloaded : Ts...
{
   using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

using namespace entt::literals;

namespace orpg
{
   static constexpr std::uint16_t SQUARE_SIZE = 32;

   struct rpc_2d_camera
   {
      rect viewport{};
      //      extents view{};
      point position{};
      //! \todo
      float fog_of_war{};
   };

   struct active
   {
   };
   struct npc
   {
      std::string phrase{};
   };

   template <typename Enum>
   constexpr auto index_of(Enum e) -> std::underlying_type_t<Enum>
   {
      return static_cast<typename std::underlying_type<Enum>::type>(e);
   }

   class application
   {
      using trigger_portal_t = entt::sigh<void(portal)>;

      struct debug_settings
      {
         bool draw_ground{true};
         bool draw_entities{true};
         bool draw_overlays{true};
         bool draw_weather{true};
      };
      // window
      debug_settings debug_cfg{};
      settings cfg{};

      point screen_offset{0, 0};
      point map_offset{0, 0};

      bool pause{false};

      std::size_t frame_counter{0};

      std::variant<village1_house1_t, village1_house2_t, village1_house3_t, world1_t, test_world_t> maps = world1_t{};

      entt::registry registry{};
      // entt::dispatcher dispatcher{};

      auto make_camera()
      {
         auto entity = registry.create();
         registry.emplace<rpc_2d_camera>(entity, cfg.window / SQUARE_SIZE, point{});
      }

      void make_blocked_tile(int x, int y)
      {
         auto entity = registry.create();
         registry.emplace<drawable>(entity, BLACK);
         registry.emplace<blocked>(entity);
         registry.emplace<position>(entity, x, y);
         registry.emplace<layer2>(entity);
         registry.emplace<collision>(entity);
      }

      auto make_npc(int x, int y)
      {
         auto entity = registry.create();
         registry.emplace<drawable>(entity, GRAY);
         registry.emplace<position>(entity, x, y);
         registry.emplace<active>(entity);
         registry.emplace<npc>(entity, "npc: say's WTF !!!");
         registry.emplace<triggerable>(entity);
         registry.emplace<layer2>(entity);
         registry.emplace<collision>(entity);
      }

      template <typename Map>
      auto make_portal(int x, int y, Map map, point p)
      {
         auto entity = registry.create();
         registry.emplace<drawable>(entity, PINK);
         registry.emplace<position>(entity, x, y);
         registry.emplace<portal>(entity, std::move(map), std::move(p));
         registry.emplace<triggerable>(entity);
         registry.emplace<layer2>(entity);
         registry.emplace<collision>(entity);
      }

      auto make_player(point pos)
      {
         auto entity = registry.create();
         registry.emplace<drawable>(entity, SKYBLUE);
         registry.emplace<position>(entity, pos.x, pos.y);
         registry.emplace<velocity>(entity, 0, 0);
         registry.emplace<active>(entity);
         registry.emplace<movement_speed>(entity, 1.f);
         registry.emplace<player>(entity);
         registry.emplace<collision>(entity);

         // registry.assign<Health>(id, 3);
         // registry.assign<Active>(id);
         // auto &rect = registry.assign<RectCollider>(id);
         // registry.assign<NullVelocityCollision>(id);
         // registry.assign<CollisionLayer>(id, LayersID::PLAYER);
         // registry.assign<ParticleData>(id);
         // registry.assign<Dash>(id);
         // rect.rect.w = sprite.rect.w * sprite.scale.x();
         // rect.rect.h = sprite.rect.h * sprite.scale.y();
      }

      template <typename T>
      [[nodiscard]] auto single_entity() const
      {
         auto view_all = registry.view<T>();
         // expects([=](){ return view_all.begin() != view_all.end(); }, "Expects at least one item");
         return std::get<T&>(view_all.get(*view_all.begin()));
      }

      template <typename T>
      [[nodiscard]] auto& single_entity()
      {
         auto view_all = registry.view<T>();
         // expects([=](){ return view_all.begin() != view_all.end(); }, "Expects at least one item");
         return std::get<T&>(view_all.get(*view_all.begin()));
      }

   public:
      application(settings defaults) : cfg{std::move(defaults)}
      {
         SetTraceLogCallback([](auto log_level, auto text, auto args) {
            // boilerplate to copy the arguments into a buffer
            char buffer[512] = {0};
            vsnprintf(buffer, 512, text, args);

            switch (log_level) {
            case TraceLogLevel::LOG_DEBUG:
            case TraceLogLevel::LOG_TRACE:
               spdlog::debug("{}", buffer);
               break;
            case TraceLogLevel::LOG_INFO:
               spdlog::info("{}", buffer);
               break;
            case TraceLogLevel::LOG_WARNING:
               spdlog::warn("{}", buffer);
               break;
            case TraceLogLevel::LOG_ERROR:
            case TraceLogLevel::LOG_FATAL:
               spdlog::error("{}", buffer);
               break;
            default:
               spdlog::info("+++ level {} = {}", log_level, buffer);
               break;
            }
         });
         InitWindow(cfg.window.width(), cfg.window.height(), cfg.window_title.c_str());
      }

      ~application() { destroy(); }

      void destroy() {}

      void init()
      {
         spdlog::info("Initialize application ...");

         frame_counter = 0;
         pause = false;

         screen_offset.x = cfg.window.width() % SQUARE_SIZE;
         screen_offset.y = cfg.window.height() % SQUARE_SIZE;
         spdlog::debug("screen offset: ({},{})", screen_offset.x, screen_offset.y);

         make_camera();
         make_player({2, 2});

         std::visit([this](auto map) { return load_map(map); }, maps);

         spdlog::info("connect signals");
         {
            auto& signal = registry.set<trigger_portal_t>();
            entt::sink sink{signal};
            sink.connect<&application::handle_portal>(*this);
         }
      }

      void update()
      {
         // input
         update_input();
         // dispatcher.update(); // emits all the events queued so far at once
         // *** fixed update
         update_view(); // if window is resized
         update_camera();
         collision_detection();
         // health_update();
         // *** variable-timed update:
         // update_animation(dt);
         // update_movement(dt);
         // enemy_walking(dt);
         player_movement(0.f);
         // player_attack(dt);
         // update_particles(dt);
         //
         draw();
         // reset
         registry.view<player, velocity>().each([](auto, velocity& vel) {
            vel.dx = 0;
            vel.dy = 0;
         });
      }

   private:
      template <typename Layer>
      void clear()
      {
         const auto entities = registry.view<Layer>();
         registry.destroy(entities.begin(), entities.end());
      }

      void handle_portal(portal const& p)
      {
         spdlog::debug("portal to '{}' triggered ... ", to_string(p.target));
         // select target (new) map
         std::visit(
            [this, &p](auto e) {
               const auto map = tile_map_by_tag_t<decltype(e), decltype(maps)>{};
               load_map(map);
               // set player position
               registry.view<player, position>().each([&p](auto, position& pos) { pos = p.position; });
            },
            p.target);
      }

      template <typename Map>
      void load_map(Map map)
      {
         clear<layer2>();

         // set tiles
         maps = map;

         // load entities
         for (auto y = 0; y < Map::rows_v; ++y) {
            for (auto x = 0; x < Map::cols_v; ++x) {
               if (std::get_if<blocked>(&map.entities[y][x])) {
                  make_blocked_tile(x, y);
               }
               if (const auto port = std::get_if<portal>(&map.entities[y][x]); port) {
                  make_portal(x, y, port->target, port->position);
               }
            }
         }

         //
         map_offset = {0, 0};
         const auto ext = rect{{0, 0}, extent(map)};
         const auto active_camera = single_entity<rpc_2d_camera const>();

         if (active_camera.viewport.is_inside(ext)) {
            map_offset = ext.centered(active_camera.viewport.extent).top_left_position;
         }
      }

      // const auto is_in_viewport = [this](auto ent) { return view.is_inside(registry.get<position>(ent)); };

      void update_view() {}

      void collision_detection()
      {
         registry.view<player, position, velocity>().each([this](auto, position const& pos, velocity& vel) {
            const auto map_size = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};
            const auto inside_map = map_size.is_inside(pos + vel);
            const auto all_entities = registry.view<blocked, position>(); //! \todo: std::views::filter(is_in_viewport);
            const auto entity_collision =
               std::count_if(all_entities.begin(), all_entities.end(),
                             [this, p = pos + vel](auto e) { return registry.get<position>(e) == p; });

            if (inside_map == false || entity_collision > 0)
               vel = {0, 0};
         });
      }

      void player_movement(const float) // dt
      {
         registry.view<player, position, velocity>().each([](auto, position& pos, velocity const& vel) {
            pos.x += vel.dx;
            pos.y += vel.dy;
         });
      }

      void update_camera()
      {
         auto& active_camera = single_entity<rpc_2d_camera>();
         const auto map_size = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};

         if (active_camera.viewport.is_inside(map_size)) {
            active_camera.position = {0, 0};
            return;
         }

         const auto seq = registry.view<player, position>();
         const auto pos = seq.get<position>(*seq.begin());
         const auto h2 = active_camera.viewport.height() / 2;
         const auto w2 = active_camera.viewport.width() / 2;
         const auto mx = std::max(pos.x - w2, 0);
         const auto my = std::max(pos.y - h2, 0);

         active_camera.position.x = std::min(mx, map_size.width() - active_camera.viewport.width());
         active_camera.position.y = std::min(my, map_size.height() - active_camera.viewport.height());
      }

      auto move_player(auto dx, auto dy)
      {
         registry.view<player, velocity>().each([dx, dy](auto, velocity& vel) {
            vel.dx = dx;
            vel.dy = dy;
         });
      }

      void update_input()
      {
         if (IsKeyPressed(KEY_F12)) {
            spdlog::info("Take screenshot: {}", "screenshot.png");
            TakeScreenshot("screenshot");
            // dispatcher.enqueue<event::take_screenshot>();
         }
         if (IsKeyPressed(KEY_F10)) {
            cfg.fullscreen = !cfg.fullscreen;
            spdlog::info("Toggle fullscreen: {}", cfg.fullscreen);
            //! \todo
            // dispatcher.enqueue<event::game_pause>();
         }
         if (IsKeyPressed('P')) {
            pause = !pause;
            // dispatcher.enqueue<event::game_pause>();
         }
         if (!pause) {
            if (IsKeyPressed(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
               move_player(1, 0);
            }
            if (IsKeyPressed(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
               move_player(-1, 0);
            }
            if (IsKeyPressed(KEY_UP) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
               move_player(0, -1);
            }
            if (IsKeyPressed(KEY_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
               move_player(0, 1);
            }

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {

               registry.view<player, position>().each([this](auto, position const& player_pos) {
                  const auto portals =
                     registry.view<position, portal>(); //! \todo: | ranges::view::filter(is_in_viewport);

                  portals.each([this, player_pos](auto, position const& pos, auto const& port) {
                     if (player_pos == pos) {
                        registry.ctx<trigger_portal_t>().publish(port);
                     }
                  });
               });
            }

            frame_counter++;
         }
      }

      void draw_grid() const noexcept
      {
         const auto half_offset = screen_offset / 2;
         const auto ho =
            Vector2{static_cast<decltype(Vector2::x)>(half_offset.x), static_cast<decltype(Vector2::y)>(half_offset.y)};

         for (auto i = 0; i < cfg.window.width() / SQUARE_SIZE + 1; ++i) {
            DrawLineV({SQUARE_SIZE * static_cast<float>(i) + ho.x, ho.y},
                      {SQUARE_SIZE * static_cast<float>(i) + ho.x, cfg.window.height() - ho.y}, LIGHTGRAY);
         }
         for (auto i = 0; i < cfg.window.height() / SQUARE_SIZE + 1; i++) {
            DrawLineV({ho.x, SQUARE_SIZE * static_cast<float>(i) + ho.y},
                      {cfg.window.width() - ho.x, SQUARE_SIZE * static_cast<float>(i) + ho.y}, LIGHTGRAY);
         }
      }

      template <typename Function>
      void draw_tile(int x, int y, Function f) const noexcept
      {
         const auto cam = single_entity<rpc_2d_camera const>();
         const auto dt = point{x, y} + cam.position;
         DrawRectangleV(
            Vector2{static_cast<decltype(Vector2::x)>((x + map_offset.x) * SQUARE_SIZE),
                    static_cast<decltype(Vector2::y)>((y + map_offset.y) * SQUARE_SIZE)},
            Vector2{static_cast<decltype(Vector2::x)>(SQUARE_SIZE), static_cast<decltype(Vector2::y)>(SQUARE_SIZE)},
            std::invoke(f, dt.y, dt.x));
      }

      template <typename Map, typename Function>
      void draw_tiles(Function f) const noexcept
      {
         const auto cam = single_entity<rpc_2d_camera const>();

         for (auto y = 0; y < std::min(cam.viewport.height(), Map::rows_v); ++y) {
            for (auto x = 0; x < std::min(cam.viewport.width(), Map::cols_v); ++x) {
               draw_tile(x, y, std::move(f));
            }
         }
      }

      template <typename Map>
      void draw_map(Map map) const noexcept
      {
         const auto func2 = [&map](auto row, auto col) {
            const auto index = map.tiles[row][col];
            switch (index) {
            case 0:
               return Color{0, 70, 0, 255};
            case 1:
               return Color{255, 0, 0, 255};
            case 2:
               return Color{0, 255, 0, 255};
            case 3:
               return Color{0, 0, 255, 255};
            case 4:
               return Color{0, 150, 150, 255};
            case 5:
               return Color{150, 0, 20, 255};
            case 6:
               return Color{150, 0, 120, 255};
            case 7:
               return Color{0, 200, 120, 205};
            case 100:
               return Color{150, 10, 20, 205};
            }
            return Color{30, 30, 55, 255};
         };

         draw_tiles<Map>(func2);
      }

      void draw_ground_layer() const noexcept
      {
         std::visit([this](auto map) { draw_map(map); }, maps);
      }

      void draw_entities_layer() const noexcept
      {
         const auto cam = single_entity<rpc_2d_camera const>();

         registry.view<position const, drawable const>().each([cam, this](auto, auto const& pos, auto const& dr) {
            const auto pp = pos - cam.position;
            if (cam.viewport.is_inside(pp)) {
               draw_tile(pp.x, pp.y, [&dr](auto, auto) { return dr.sprite; });
            }
         });
      }
      void draw_overlay_layer() const noexcept
      {
         // gates, etc ...
      }
      void draw_weather_layer() const noexcept {}

      [[nodiscard]] constexpr auto half_window_width() const noexcept { return cfg.window.width() / 2; }

      [[nodiscard]] constexpr auto half_window_height() const noexcept { return cfg.window.height() / 2; }

      void draw_pause_overlay() const noexcept
      {
         if (pause) {
            DrawText("GAME PAUSED", half_window_width() - MeasureText("GAME PAUSED", 40) / 2, half_window_height() - 40,
                     40, GRAY);
         }
      }

      void draw_gui() const noexcept {
#if 0
         int width, height, roundness, lineThick, segments;
         // Draw GUI controls
         //------------------------------------------------------------------------------
         width = (int)GuiSliderBar((Rectangle){ 640, 40, 105, 20 }, "Width", NULL, (float)width, 0, (float)GetScreenWidth() - 300);
         height = (int)GuiSliderBar((Rectangle){ 640, 70, 105, 20 }, "Height", NULL, (float)height, 0, (float)GetScreenHeight() - 50);
         roundness = GuiSliderBar((Rectangle){ 640, 140, 105, 20 }, "Roundness", NULL, roundness, 0.0f, 1.0f);
         lineThick = (int)GuiSliderBar((Rectangle){ 640, 170, 105, 20 }, "Thickness", NULL, (float)lineThick, 0, 20);
         segments = (int)GuiSliderBar((Rectangle){ 640, 240, 105, 20}, "Segments", NULL, (float)segments, 0, 60);
         debug_cfg.draw_ground = GuiCheckBox((Rectangle){ 640, 320, 20, 20 }, "draw ground", debug_cfg.draw_ground);
         debug_cfg.draw_entities = GuiCheckBox((Rectangle){ 640, 350, 20, 20 }, "draw entities", debug_cfg.draw_entities);
         debug_cfg.draw_overlays = GuiCheckBox((Rectangle){ 640, 380, 20, 20 }, "draw entities", debug_cfg.draw_overlays);
         debug_cfg.draw_weather = GuiCheckBox((Rectangle){ 640, 410, 20, 20 }, "draw entities", debug_cfg.draw_weather);
         //------------------------------------------------------------------------------
         // GuiWindowBox((Rectangle){ 50, 200, 300, 200 }, "#198# PORTABLE WINDOW");
         DrawText(TextFormat("MODE: %s", (segments >= 4)? "MANUAL" : "AUTO"), 640, 280, 10, (segments >= 4)? MAROON : DARKGRAY);
#endif
      }

      void draw_game_debug_information() const
      {
         const auto left = cfg.window.width() - 150;

         registry.view<player, position>().each([this, left](auto, position const& pos) {
            DrawText(fmt::format("player pos: ({},{})", pos.x, pos.y).c_str(), left, cfg.window.height() - 40, 10,
                     MAROON);
         });
         registry.view<rpc_2d_camera const>().each([this, left](auto, auto const& cam) {
            DrawText(fmt::format("Camera pos: ({},{})", cam.position.x, cam.position.y).c_str(), left,
                     cfg.window.height() - 20, 10, MAROON);
         });
      }

      static void draw_gamepad_debug_information() noexcept
      {
         struct gamepad
         {
            std::size_t id{};
            std::string name{};
            std::vector<double> axis_movement{};
         };

         std::vector<gamepad> gamepads{};

         for (auto i : {0u, 1u, 2u, 3u}) {
            if (IsGamepadAvailable(static_cast<int>(i))) {
               gamepad gp{};
               gp.id = i;
               gp.name = GetGamepadName(static_cast<int>(gp.id));
               gp.axis_movement.resize(static_cast<std::size_t>(GetGamepadAxisCount(static_cast<int>(gp.id))));
               for (decltype(gp.axis_movement.size()) k = 0; k < gp.axis_movement.size(); ++k) {
                  gp.axis_movement[k] = GetGamepadAxisMovement(static_cast<int>(i), static_cast<int>(k));
               }
               gamepads.push_back(gp);
            }
         }

         if (std::empty(gamepads)) {
            DrawText("NO GAMEPADS DETECTED", 10, 10, 10, GRAY);
            return;
         }

         int it_y = 10;
         if (GetGamepadButtonPressed() != -1) {
            DrawText(fmt::format("Detected button: {}", GetGamepadButtonPressed()).c_str(), 10, it_y, 10, RED);
         } else {
            DrawText("Detected button: None", 10, it_y, 10, GRAY);
         }

         //! \todo sec21::for_each_indexed()
         for (decltype(gamepads.size()) i = 0; i < gamepads.size(); ++i) {
            it_y += 20;
            const auto e = gamepads[i];
            DrawText(fmt::format("GP: {}", e.name).c_str(), 10, it_y, 10, BLACK);
            it_y += 20;
            DrawText(fmt::format("Detected axis count: {}", e.axis_movement.size()).c_str(), 10, it_y, 10, MAROON);
            it_y += 20;
            for (decltype(e.axis_movement.size()) k = 0; k < e.axis_movement.size(); ++k) {
               DrawText(TextFormat("AXIS %i: %.02f", i, e.axis_movement[k]), 20, it_y, 10, DARKGRAY);
               it_y += 20;
            }
         }
      }

      void draw() noexcept
      {
         BeginDrawing();
         ClearBackground(RAYWHITE);

         draw_grid();
         draw_ground_layer();
         draw_entities_layer();
         draw_overlay_layer();
         draw_weather_layer();
         draw_pause_overlay();
         draw_gui();
         draw_game_debug_information();
         draw_gamepad_debug_information();

         EndDrawing();
      }
   };
} // namespace orpg

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
   try {
      // #ifndef NDEBUG
      spdlog::set_level(spdlog::level::debug);
      // #endif
      spdlog::info("Starting application: {} {}.{}", EXECUTABLE_NAME, VERSION_MAJOR, VERSION_MINOR);
      spdlog::debug("enTT version: {}.{}", ENTT_VERSION_MAJOR, ENTT_VERSION_MINOR);

      auto app = orpg::application{{.window_title = "orpg"}};
      app.init();

#if defined(PLATFORM_WEB)
      emscripten_set_main_loop([=]() { app.update(); }, 60, 1);
#else
      SetTargetFPS(60);

      while (!WindowShouldClose()) // Detect window close button or ESC key
      {
         app.update();
      }
#endif
      app.destroy();
      CloseWindow();

   } catch (std::exception& ex) {
      spdlog::critical("Exception: {}", ex.what());
      return -1;
   }
   return 0;
}

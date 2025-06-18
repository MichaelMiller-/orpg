#include "components/collision.h"
#include "components/drawable.h"
#include "components/layers.h"
#include "components/movement_speed.h"
#include "components/npc.h"
#include "components/player.h"
#include "components/triggerable.h"
#include "components/velocity.h"
#include "config.h"
#include "configuration.h"
#include "data.h"
#include "entities.h"
#include "point.h"
#include "rect.h"
#include "tags.h"
#include "tile_map.h"

#include <raylib.h>

#ifdef ORPG_WEB_BUILD
#include <emscripten/emscripten.h>
#endif

#include <cstdarg>
#include <format>
#include <variant>

#include <entt/entt.hpp>

constexpr auto operator+(Vector2 const& lhs, Vector2 const& rhs) noexcept
{
   return Vector2{lhs.x + rhs.x, lhs.y + rhs.y};
}

constexpr auto operator+(Vector2 const& lhs, orpg::point const& rhs) noexcept
{
   return Vector2{lhs.x + static_cast<decltype(Vector2::x)>(rhs.x), lhs.y + static_cast<decltype(Vector2::y)>(rhs.y)};
}

template <typename T>
constexpr auto operator/(Vector2 const& lhs, T const& rhs)
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

   template <typename Enum>
   constexpr auto index_of(Enum e) -> std::underlying_type_t<Enum>
   {
      return static_cast<typename std::underlying_type<Enum>::type>(e);
   }

   class application final
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
      configuration_t config{};

      point screen_offset{0, 0};
      point map_offset{0, 0};

      bool pause{false};

      std::size_t frame_counter{0};

      std::variant<village1_house1_t, village1_house2_t, village1_house3_t, world1_t, test_world_t> maps = world1_t{};

      entt::registry registry{};
      // entt::dispatcher dispatcher{};

      auto make_camera()
      {
         auto const entity = registry.create();
         registry.emplace<rpc_2d_camera>(entity, get_window_rect() / SQUARE_SIZE, point{});
      }

      void make_blocked_tile(int x, int y)
      {
         auto const entity = registry.create();
         registry.emplace<drawable>(entity, BLACK);
         registry.emplace<blocked>(entity);
         registry.emplace<position>(entity, x, y);
         registry.emplace<layer2>(entity);
         registry.emplace<collision>(entity);
      }

      auto make_npc(int x, int y)
      {
         auto const entity = registry.create();
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
         auto const entity = registry.create();
         registry.emplace<drawable>(entity, PINK);
         registry.emplace<position>(entity, x, y);
         registry.emplace<portal>(entity, std::move(map), std::move(p));
         registry.emplace<triggerable>(entity);
         registry.emplace<layer2>(entity);
         registry.emplace<collision>(entity);
      }

      auto make_player(point pos)
      {
         auto const entity = registry.create();
         registry.emplace<drawable>(entity, SKYBLUE);
         registry.emplace<position>(entity, pos.x, pos.y);
         registry.emplace<velocity>(entity, 0, 0);
         registry.emplace<active>(entity);
         registry.emplace<movement_speed>(entity, 1.f);
         registry.emplace<player>(entity);
         registry.emplace<collision>(entity);
      }

      //! \todo deducing this
      template <typename T>
      [[nodiscard]] auto single_entity() const -> decltype(auto)
      {
         auto view_all = registry.view<T>();
         // sec21::expects([=]() { return view_all.begin() != view_all.end(); }, "Expects at least one item");
         return std::get<T&>(view_all.get(*view_all.begin()));
      }

      //! \todo deducing this
      template <typename T>
      [[nodiscard]] auto single_entity() -> decltype(auto)
      {
         auto view_all = registry.view<T>();
         // sec21::expects([=]() { return view_all.begin() != view_all.end(); }, "Expects at least one item");
         return std::get<T&>(view_all.get(*view_all.begin()));
      }

   public:
      application()
      {
         SetTraceLogCallback([](auto log_level, auto text, auto args) {
            // boilerplate to copy the arguments into a buffer
            char buffer[512] = {0};
            vsnprintf(buffer, 512, text, args);
            std::puts(buffer);
#if 0
            switch (log_level) {
            case TraceLogLevel::LOG_DEBUG:
            case TraceLogLevel::LOG_TRACE:
               std::puts(buffer);
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
#endif
         });
         InitWindow(1024, 768, "orpg");
      }

      ~application() = default;

      void init()
      {
         std::puts("Initialize application ...");

         frame_counter = 0;
         pause = false;

         screen_offset.x = GetScreenWidth() % SQUARE_SIZE;
         screen_offset.y = GetScreenHeight() % SQUARE_SIZE;
         std::puts(std::format("screen offset: ({},{})", screen_offset.x, screen_offset.y).c_str());

         make_camera();
         make_player({2, 2});

         std::visit([this](auto map) { return load_map(map); }, maps);

         std::puts("connect signals");
         {
            // store signal in the registry
            auto& signal = registry.ctx().emplace<trigger_portal_t>();
            entt::sink sink{signal};
            sink.connect<&application::handle_portal>(*this);
         }
      }

      void update()
      {
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
      void clear() noexcept
      {
         auto entities = registry.view<Layer>();
         registry.destroy(entities.begin(), entities.end());
      }

      void handle_portal(portal const& p) noexcept
      {
         std::puts(std::format("portal to '{}' triggered ... ", to_string(p.target)).c_str());
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
      void load_map(Map map) noexcept
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

      void update_view() noexcept {}

      void collision_detection() noexcept
      {
         registry.view<player, position, velocity>().each([this](auto, position const& pos, velocity& vel) {
            auto const map_size = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};
            auto const inside_map = map_size.is_inside(pos + vel);
            auto const all_entities = registry.view<blocked, position>(); //! \todo: std::views::filter(is_in_viewport);
            auto const entity_collision =
               std::count_if(all_entities.begin(), all_entities.end(),
                             [this, p = pos + vel](auto e) { return registry.get<position>(e) == p; });

            if (inside_map == false || entity_collision > 0)
               vel = {0, 0};
         });
      }

      void player_movement(float) noexcept // dt
      {
         registry.view<player, position, velocity>().each([](auto, position& pos, velocity const& vel) {
            pos.x += vel.dx;
            pos.y += vel.dy;
         });
      }

      void update_camera() noexcept
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

      auto move_player(auto dx, auto dy) noexcept
      {
         registry.view<player, velocity>().each([dx, dy](auto, velocity& vel) {
            vel.dx = dx;
            vel.dy = dy;
         });
      }

      void update_input() noexcept
      {
         if (IsKeyPressed(KEY_F12)) {
            std::puts("Save screenshot into file: screenshot.png");
            TakeScreenshot("screenshot");
            // dispatcher.enqueue<event::take_screenshot>();
         }
         if (IsKeyPressed(KEY_F10)) {
            ToggleFullscreen();
         }
         if (IsKeyPressed('P')) {
            pause = !pause;
            // dispatcher.enqueue<event::game_pause>();
         }
         if (!pause) {
            if (IsKeyPressed(KEY_RIGHT) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
               move_player(1, 0);
            }
            if (IsKeyPressed(KEY_LEFT) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
               move_player(-1, 0);
            }
            if (IsKeyPressed(KEY_UP) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
               move_player(0, -1);
            }
            if (IsKeyPressed(KEY_DOWN) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
               move_player(0, 1);
            }

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {

               registry.view<player, position>().each([this](auto, position const& player_pos) {
                  auto portals = registry.view<position, portal>(); //! \todo: | ranges::view::filter(is_in_viewport);

                  portals.each([this, player_pos](auto, position const& pos, auto const& port) {
                     if (player_pos == pos) {
                        registry.ctx().get<trigger_portal_t>().publish(port);
                     }
                  });
               });
            }

            frame_counter++;
         }
      }

      void draw_grid() const noexcept
      {
         auto const half_offset = screen_offset / 2;
         auto const ho =
            Vector2{static_cast<decltype(Vector2::x)>(half_offset.x), static_cast<decltype(Vector2::y)>(half_offset.y)};

         for (auto i = 0; i < GetScreenWidth() / SQUARE_SIZE + 1; ++i) {
            DrawLineV({SQUARE_SIZE * static_cast<float>(i) + ho.x, ho.y},
                      {SQUARE_SIZE * static_cast<float>(i) + ho.x, GetScreenHeight() - ho.y}, LIGHTGRAY);
         }
         for (auto i = 0; i < GetScreenHeight() / SQUARE_SIZE + 1; i++) {
            DrawLineV({ho.x, SQUARE_SIZE * static_cast<float>(i) + ho.y},
                      {GetScreenWidth() - ho.x, SQUARE_SIZE * static_cast<float>(i) + ho.y}, LIGHTGRAY);
         }
      }

      template <typename Function>
      void draw_tile(int x, int y, Function f) const noexcept
      {
         auto const cam = single_entity<rpc_2d_camera const>();
         auto const dt = point{x, y} + cam.position;
         DrawRectangleV(
            Vector2{static_cast<decltype(Vector2::x)>((x + map_offset.x) * SQUARE_SIZE),
                    static_cast<decltype(Vector2::y)>((y + map_offset.y) * SQUARE_SIZE)},
            Vector2{static_cast<decltype(Vector2::x)>(SQUARE_SIZE), static_cast<decltype(Vector2::y)>(SQUARE_SIZE)},
            std::invoke(f, dt.y, dt.x));
      }

      template <typename Map, typename Function>
      void draw_tiles(Function f) const noexcept
      {
         auto const cam = single_entity<rpc_2d_camera const>();

         for (auto y = 0; y < std::min(cam.viewport.height(), Map::rows_v); ++y) {
            for (auto x = 0; x < std::min(cam.viewport.width(), Map::cols_v); ++x) {
               draw_tile(x, y, std::move(f));
            }
         }
      }

      template <typename Map>
      void draw_map(Map map) const noexcept
      {
         auto func2 = [&map](auto row, auto col) {
            switch (auto index = map.tiles[row][col]) {
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

      [[nodiscard]] auto get_window_rect() const -> rect
      {
         return rect{.top_left_position = {.x = 0, .y = 0},
                     .extent = {.width = static_cast<extents::value_t>(GetScreenWidth()),
                                .height = static_cast<extents::value_t>(GetScreenHeight())}};
      }

      void draw_pause_overlay() const
      {
         if (pause) {
            auto const rc = get_window_rect() / 2;
            DrawText("GAME PAUSED", rc.width() - MeasureText("GAME PAUSED", 40) / 2, rc.height() - 40, 40, GRAY);
         }
      }

      struct room
      {
         bool light_on{false};
         std::function<void(bool)> light_on_changed{};
      };

      struct button
      {
         std::function<void()> on_clicked{};
         rect pos_and_extents{};
         std::string_view text{};

         button(rect rc, std::string_view caption) : pos_and_extents{rc}, text{caption} {}

         button(rect rc, std::string_view caption, std::function<void()> click)
             : pos_and_extents{rc}, text{caption}, on_clicked{click}
         {
         }
#if 0
         static auto to_rect(rect rc) -> Rectangle
         {
            return {rc.top_left_position.x, rc.top_left_position.y, rc.width(), rc.height()};
         }
#endif
         void draw()
         {
#if TODO
            if (GuiButton(to_rect(pos_and_extents), text.data()) && on_clicked) {
               std::invoke(on_clicked);
            }
#endif
         }
      };

      struct ui_component
      {
         //         button btn{};
         ui_component(std::shared_ptr<room> r, int* parent = nullptr)
         {
            // btn.on_clicked = ([]{
            // r->light_on = !r->light_on;
            // r->light_on_changed(light_on);
            // });
            auto update = [=] {
               // btn.set_text(r->light_on ? "Turn off!" : "Turn on!");
            };
            // room_ = ->light_on_changed = update;
            update();
         }
      };

      void draw_game_debug_information() const
      {
         auto left = GetScreenWidth() - 150;

         registry.view<player, position>().each([left](auto, position const& pos) {
            DrawText(std::format("player pos: ({},{})", pos.x, pos.y).c_str(), left, GetScreenHeight() - 40, 10,
                     MAROON);
         });
         registry.view<rpc_2d_camera const>().each([left](auto, auto const& cam) {
            DrawText(std::format("Camera pos: ({},{})", cam.position.x, cam.position.y).c_str(), left,
                     GetScreenHeight() - 20, 10, MAROON);
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
            DrawText(std::format("Detected button: {}", GetGamepadButtonPressed()).c_str(), 10, it_y, 10, RED);
         } else {
            DrawText("Detected button: None", 10, it_y, 10, GRAY);
         }

         for (auto&& [index, pad] : std::views::enumerate(gamepads)) {
            it_y += 20;
            DrawText(std::format("GP: {}", pad.name).c_str(), 10, it_y, 10, BLACK);
            it_y += 20;
            DrawText(std::format("Detected axis count: {}", pad.axis_movement.size()).c_str(), 10, it_y, 10, MAROON);
            it_y += 20;
            for (double k : pad.axis_movement) {
               DrawText(TextFormat("AXIS %i: %.02f", index, k), 20, it_y, 10, DARKGRAY);
               it_y += 20;
            }
         }
      }

      void draw() const noexcept
      {
         BeginDrawing();
         ClearBackground(RAYWHITE);

         draw_grid();
         draw_ground_layer();
         draw_entities_layer();
         draw_overlay_layer();
         draw_weather_layer();
         draw_pause_overlay();
         // draw_gui();
         draw_game_debug_information();
         draw_gamepad_debug_information();

         EndDrawing();
      }
   };
} // namespace orpg

std::unique_ptr<orpg::application> app{nullptr};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
   try {
      std::puts(std::format("Starting application: {} {}.{}", ORPG_BINARY_NAME, VERSION_MAJOR, VERSION_MINOR).c_str());
      std::puts(std::format("enTT version: {}.{}", ENTT_VERSION_MAJOR, ENTT_VERSION_MINOR).c_str());

      app = std::make_unique<orpg::application>();
      app->init();

#ifdef ORPG_WEB_BUILD
      emscripten_set_main_loop([]() { app->update(); }, 60, 1);
#else
      SetTargetFPS(60);

      while (!WindowShouldClose()) // Detect window close button or ESC key
      {
         app->update();
      }
#endif
      CloseWindow();

   } catch (std::exception& ex) {
      std::puts(ex.what());
      return -1;
   }
   return 0;
}

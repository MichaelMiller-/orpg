#include "config.h"
#include "data.h"
#include "entities.h"
#include "point.h"
#include "rect.h"
#include "tags.h"
#include "tile_map.h"

#include <raylib.h>

#include <spdlog/spdlog.h>

#include <cstdarg>
#include <variant>

auto operator+(Vector2 const &lhs, Vector2 const &rhs) noexcept { return Vector2{lhs.x + rhs.x, lhs.y + rhs.y}; }

auto operator+(Vector2 const &lhs, orpg::point const &rhs) noexcept {
    return Vector2{lhs.x + static_cast<decltype(Vector2::x)>(rhs.x), lhs.y + static_cast<decltype(Vector2::y)>(rhs.y)};
}

template<typename T>
auto operator/(Vector2 const &lhs, T const &rhs) {
    // \todo if (rhs == 0)
    return Vector2{lhs.x / rhs, lhs.y / rhs};
}

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace orpg {
    static constexpr std::uint16_t SQUARE_SIZE = 32;

    struct Food {
        point position{0, 0};
        Vector2 size{SQUARE_SIZE, SQUARE_SIZE};
        Color color = SKYBLUE;
    };

    struct settings {
        std::string window_title{};
        rect window{{0,   0},
                    {640, 480}};
    };

    struct loading {
    };
    struct main_menu {
    };
    struct play {
    };
    struct game_over {
    };
    using gamestate_t = std::variant<loading, main_menu, play, game_over>;

    class application {
        // window
        settings cfg{};
        point screen_offset{0, 0};
        point camera{0, 0};
        rect view{}; // 0,0 { 24,
        bool pause{false};
        bool gameOver{false};
        bool fullscreen{false};
        std::size_t framesCounter{0};
        Food player{};
        point map_offset{0, 0};

        std::variant<village1_house1_t, village1_house2_t, village1_house3_t, world1_t, test_world_t> maps = village1_house1_t{};

    public:
        application(settings defaults) : cfg{std::move(defaults)}, view{defaults.window / SQUARE_SIZE} {
            spdlog::debug("initial view: ({},{}) [{}x{}]", view.top_left_position.x, view.top_left_position.y,
                          view.extent.width, view.extent.height);

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

        void init() {
            spdlog::info("Initialize application ...");

            framesCounter = 0;
            gameOver = false;
            pause = false;

            screen_offset.x = cfg.window.width() % SQUARE_SIZE;
            screen_offset.y = cfg.window.height() % SQUARE_SIZE;
            spdlog::debug("window offset: ({},{})", screen_offset.x, screen_offset.y);

            const auto ext = std::visit([](auto map) { return extent(map); }, maps);
            spdlog::debug("extents initial map: {}x{}", ext.width, ext.height);

            // center first map
            map_offset = {0, 0}; // view.center() - std::visit([](auto map) { return center(map); }, maps);
            // map_offset.x = (cfg.window.width() - (ext.width * SQUARE_SIZE)) / 2;
            // map_offset.y = (cfg.window.height() - (ext.height * SQUARE_SIZE)) / 2;
            spdlog::debug("map offset: ({},{})", map_offset.x, map_offset.y);

            player.position = {2, 2}; // view.center(); // world position
            // camera = player.position;
        }

        void update() {
            logic();
            draw();
        }

    private:
        [[nodiscard]] auto half_window_width() const noexcept { return cfg.window.width() / 2; }

        [[nodiscard]] auto half_window_height() const noexcept { return cfg.window.height() / 2; }

        auto screen_to_map_pos(point p) -> point { return p - map_offset; }

        auto move_player(int dx, int dy) {
            const auto delta = player.position + decltype(player.position){dx, dy};
            spdlog::debug("movement delta ({},{}) ", delta.x, delta.y);

            const auto move_allowed = [this](point pos) {
                return true;
                return std::visit(
                        [p = pos](auto map) { return std::get_if<blocked>(&map.entities[p.y][p.x]) == nullptr; },
                        maps);
            };

            // movement is blocked by an entity
            if (move_allowed(delta) == false) {
                spdlog::debug("player movement to ({},{}) is blocked", delta.x, delta.y);
                return;
            }

            const auto map_bounding_rect = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};
            // spdlog::debug("map_bounding_rect: {} ", to_string(map_bounding_rect));

            const auto map_border = [this, map_bounding_rect]() {
                if (view.is_inside(map_bounding_rect)) {
                    return map_bounding_rect;
                }
                return rect{map_bounding_rect.top_left_position,
                            {static_cast<decltype(extents::width)>(map_bounding_rect.extent.width - 1),
                             static_cast<decltype(extents::height)>(map_bounding_rect.extent.height - 1)}};
            };

            if (map_border().is_inside(delta)) {
                player.position = delta;
            } else {
                spdlog::debug("player movement to ({},{}) is blocked", delta.x, delta.y);
            }

            if (view.is_inside(map_bounding_rect)) {
                // map is smaller than viewport -> no camera movement
                return;
            }

            // halbes view rechteck als schwelle zur kamera bewegung
            const auto h2 = view.height() / 2;
            const auto w2 = view.width() / 2; // 10
            const auto mx = std::max(delta.x - w2, 0);
            const auto my = std::max(delta.y - h2, 0);

            camera.x = std::min(mx, map_bounding_rect.width() - view.width());
            camera.y = std::min(my, map_bounding_rect.height() - view.height());
        }

        void logic() {
            if (IsKeyPressed(KEY_F12)) {
                spdlog::info("Take screenshot: {}", "screenshot.png");
                TakeScreenshot("screenshot");
            }
            if (IsKeyPressed(KEY_F10)) {
                fullscreen = !fullscreen;
                spdlog::info("Toggle fullscreen: {}", fullscreen);
                //! \todo
            }
            if (!gameOver) {
                if (IsKeyPressed('P')) {
                    pause = !pause;
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

                    const auto is_under_portal = [this](point pos) {
                        return std::visit(
                                [p = pos](auto map) { return std::get_if<portal>(&map.entities[p.y][p.x]) != nullptr; },
                                maps);
                    };

                    const auto trigger_portal = [this](point pos) {
                        return std::visit(
                                [this, p = pos](auto map) {
                                    if (const auto value = std::get_if<portal>(&map.entities[p.y][p.x]); value) {

                                        spdlog::debug("portal to '{}' triggered ... ", to_string(value->target));
                                        spdlog::debug("view center: {}", to_string(view.center()));
                                        spdlog::debug("value->position: {}", to_string(value->position));

                                        // select new map
                                        std::visit(
                                                [this](auto e) { maps = tile_map_by_tag_t<decltype(e), decltype(maps)>{}; },
                                                value->target);

                                        //! \todo  position map

                                        const auto map_size = rect{{0, 0},
                                                                   std::visit([](auto m) { return extent(m); }, maps)};
                                        spdlog::debug("map_size: {}", to_string(map_size));

                                        if (view.is_inside(map_size)) {
                                            spdlog::debug("new map is smaller than view, therefore center it");
                                            map_offset = view.center() - map_size.center();
                                            // player.position = map_offset + value->position;
                                        } else if (view.height() < map_size.height() &&
                                                   view.width() > map_size.width()) {
                                            spdlog::debug(
                                                    "new map is higher than view, therefore center around player");
                                            map_offset = view.center() - value->position + point{0, 1};
                                            // player.position = view.center(); // map_offset + value->position - point{0,1};
                                        } else {
                                            spdlog::debug("new map is GREATER than view,!!!");
                                            map_offset =
                                                    view.center() - value->position +
                                                    point{1, 0}; // point{6,-9}; // - value->position;
                                            // player.position = view.center();
                                        }
                                        spdlog::debug("new offset: {}", to_string(map_offset));
                                    }
                                    if (const auto value = std::get_if<chest>(&map.entities[p.y][p.x]); value) {
                                        spdlog::debug("chest triggered at ({},{}) ", p.y, p.x);
                                    }
                                },
                                maps);
                    };

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                        IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                        trigger_portal(screen_to_map_pos(player.position));
                    }

                    framesCounter++;
                }
            } else {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                    IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                    init();
                    gameOver = false;
                }
            }
        }

        template<typename T>
        [[nodiscard]] auto world_to_screen(T v) const noexcept -> Vector2 {
            return {static_cast<decltype(Vector2::x)>(v.x * SQUARE_SIZE),
                    static_cast<decltype(Vector2::y)>(v.y * SQUARE_SIZE)};
        }

        void draw() const noexcept {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            draw_grid();

            if (!gameOver) {

                std::visit([this](auto map) { draw_map(map); }, maps);
                // std::visit([this](auto map) { draw_map_entities(map); }, maps);

                // Draw to player
                DrawRectangleV(world_to_screen(player.position - camera) + (screen_offset / 2), player.size,
                               player.color);

                // Draw to camera
                // DrawRectangleV(world_to_screen(camera) + (screen_offset / 2), player.size, PINK);

                if (pause) {
                    draw_pause_overlay();
                }

            } else [[unlikely]] {
                DrawText("PRESS [ENTER] TO PLAY AGAIN",
                         half_window_width() - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
                         half_window_height() - 50, 20, GRAY);
            }

            draw_game_debug_information();
            draw_gamepad_debug_information();

            EndDrawing();
        }

        void draw_game_debug_information() const {
            const auto left = cfg.window.width() - 150;
            DrawText(fmt::format("Player pos: ({},{})", player.position.x, player.position.y).c_str(), left,
                     cfg.window.height() - 40, 10, MAROON);
            DrawText(fmt::format("Camera pos: ({},{})", camera.x, camera.y).c_str(), left, cfg.window.height() - 20, 10,
                     MAROON);
        }

        void draw_grid() const noexcept {
            const auto half_offset = screen_offset / 2;
            const auto ho =
                    Vector2{static_cast<decltype(Vector2::x)>(half_offset.x),
                            static_cast<decltype(Vector2::y)>(half_offset.y)};

            for (auto i = 0; i < cfg.window.width() / SQUARE_SIZE + 1; ++i) {
                DrawLineV({SQUARE_SIZE * i + ho.x, ho.y}, {SQUARE_SIZE * i + ho.x, cfg.window.height() - ho.y},
                          LIGHTGRAY);
            }
            for (auto i = 0; i < cfg.window.height() / SQUARE_SIZE + 1; i++) {
                DrawLineV({ho.x, SQUARE_SIZE * i + ho.y}, {cfg.window.width() - ho.x, SQUARE_SIZE * i + ho.y},
                          LIGHTGRAY);
            }
        }

        template<typename Function>
        void draw_tile(int x, int y, Function f) const {
            const auto dt = point{x, y} + camera;
            DrawRectangleV(Vector2{static_cast<decltype(Vector2::x)>((x * SQUARE_SIZE) + map_offset.x),
                                   static_cast<decltype(Vector2::y)>((y * SQUARE_SIZE) + map_offset.y)},
                           {SQUARE_SIZE, SQUARE_SIZE}, std::invoke(f, dt.y, dt.x));
        }

        template<typename Function>
        void draw_tiles(Function f) const noexcept {
            for (auto y = 0; y < view.height(); ++y) {
                for (auto x = 0; x < view.width(); ++x) {
                    draw_tile(y, x, std::move(f));
                }
            }
        }

        template<typename Map>
        void draw_map(Map map) const noexcept {
            const auto func2 = [&map](auto row, auto col) {

                if (row > Map::rows_v || col > Map::cols_v)
                    return Color{0, 0, 0, 0};

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
#if 0
            draw_tiles(func2);
#else

            for (auto y = 0; y < view.height(); ++y) {
                for (auto x = 0; x < view.width(); ++x) {

                    const auto dt = point{x, y} + camera;
                    DrawRectangleV(Vector2{static_cast<decltype(Vector2::x)>((x * SQUARE_SIZE) + map_offset.x),
                                           static_cast<decltype(Vector2::y)>((y * SQUARE_SIZE) + map_offset.y)},
                                   {SQUARE_SIZE, SQUARE_SIZE}, std::invoke(func2, dt.y, dt.x));

                }
            }
#endif
        }

        template<typename Map>
        void draw_map_entities(Map map) const noexcept {
            const auto func2 = [&map](auto row, auto col) {
                return std::visit(overloaded{
                                          [](portal) {
                                              return Color{255, 255, 0, 255};
                                          },
                                          [](blocked) {
                                              return Color{0, 0, 0, 255};
                                          },
                                          [](chest) { return GOLD; },
                                          [](auto e) {
                                              return Color{0, 0, 0, 0};
                                          },
                                  },
                                  map.entities[row][col]);
            };

            draw_tiles(func2);
        }

        void draw_pause_overlay() const noexcept {
            DrawText("GAME PAUSED", half_window_width() - MeasureText("GAME PAUSED", 40) / 2, half_window_height() - 40,
                     40, GRAY);
        }

        static void draw_gamepad_debug_information() noexcept {
            struct gamepad {
                int id{-1};
                std::string name{};
                std::vector<double> axis_movement{};
            };

            std::vector<gamepad> gamepads{};

            for (auto i: {0, 1, 2, 3}) {
                if (IsGamepadAvailable(i)) {
                    gamepad gp{};
                    gp.id = i;
                    gp.name = GetGamepadName(gp.id);
                    const auto count = GetGamepadAxisCount(gp.id);
                    gp.axis_movement.resize(count);
                    for (auto k = 0; k < gp.axis_movement.size(); ++k) {
                        gp.axis_movement[k] = GetGamepadAxisMovement(i, k);
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
            for (auto i = 0; i < gamepads.size(); ++i) {
                it_y += 20;
                const auto e = gamepads[i];
                DrawText(fmt::format("GP: {}", e.name).c_str(), 10, it_y, 10, BLACK);
                it_y += 20;
                DrawText(fmt::format("Detected axis count: {}", e.axis_movement.size()).c_str(), 10, it_y, 10, MAROON);
                it_y += 20;
                for (auto k = 0; k < e.axis_movement.size(); ++k) {
                    DrawText(TextFormat("AXIS %i: %.02f", i, e.axis_movement[k]), 20, it_y, 10, DARKGRAY);
                    it_y += 20;
                }
            }
        }
    };
} // namespace orpg

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    try {
        // #ifndef NDEBUG
        spdlog::set_level(spdlog::level::debug);
        // #endif
        spdlog::info("Starting application: {} {}.{}", EXECUTABLE_NAME, VERSION_MAJOR, VERSION_MINOR);

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

    } catch (std::exception &ex) {
        spdlog::critical("Exception: {}", ex.what());
        return -1;
    }
    return 0;
}

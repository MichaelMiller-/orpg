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

    class rpg_camera {
        point pt{0, 0};
        const rect view{};;
    public:
        explicit rpg_camera(rect viewport) : view{std::move(viewport)} {
            spdlog::debug("rpg camera view: {}", to_string(view));
        }

        void move(point const &pt) {

            const auto h2 = view.height() / 2;
            const auto w2 = view.width() / 2;
            const auto mx = std::max(pt.x - w2, 0);
            const auto my = std::max(pt.y - h2, 0);

        }
    };

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

        rpg_camera cam;

        std::variant<village1_house1_t, village1_house2_t, village1_house3_t, world1_t, test_world_t> maps = world1_t{};

    public:
        application(settings defaults) : cfg{std::move(defaults)}, view{defaults.window / SQUARE_SIZE}, cam{view} {
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

            const auto ext = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};
            spdlog::debug("extents initial map: {}", to_string(ext));

            map_offset = {0, 0}; // view.center() - std::visit([](auto map) { return center(map); }, maps);

            if (view.is_inside(ext)) {
                map_offset = ext.centered(view.extent).top_left_position;
            }
            player.position = point{2, 2}; // view.center(); // world position
        }

        void update() {
            logic();
            draw();
        }

    private:
        [[nodiscard]] auto half_window_width() const noexcept { return cfg.window.width() / 2; }

        [[nodiscard]] auto half_window_height() const noexcept { return cfg.window.height() / 2; }

        auto move_camera(int dx, int dy) {

            const auto map_bounding_rect = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};

            const auto h2 = view.height() / 2;
            const auto w2 = view.width() / 2; // 10
            const auto mx = std::max(dx - w2, 0);
            const auto my = std::max(dy - h2, 0);

            camera.x = std::min(mx, map_bounding_rect.width() - view.width());
            camera.y = std::min(my, map_bounding_rect.height() - view.height());
        }

        auto move_player(int dx, int dy) {
            const auto delta = player.position + decltype(player.position){dx, dy};
            spdlog::debug("movement delta ({},{}) ", delta.x, delta.y);

            const auto move_allowed = [this](point pos) {
                return std::visit(
                        [p = pos](auto map) { return std::get_if<blocked>(&map.entities[p.y][p.x]) == nullptr; },
                        maps);
            };

            // movement is blocked by an entity
            if (move_allowed(delta) == false) {
                spdlog::debug("player movement to ({},{}) is blocked by an entity", delta.x, delta.y);
                return;
            }

            const auto map_bounding_rect = rect{{0, 0}, std::visit([](auto map) { return extent(map); }, maps)};
            if (map_bounding_rect.is_inside(delta) == false) {
                spdlog::debug("player movement to ({},{}) ----- map extent", delta.x, delta.y);
                return;
            }

            player.position = delta;

            if (view.is_inside(map_bounding_rect)) {
                // map is smaller than viewport -> no camera movement
                return;
            }
            move_camera(delta.x, delta.y);
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

                    const auto trigger = [this](point pos) {
                        spdlog::debug("trigger: {}", to_string(pos));

                        std::visit(
                                [this, p = pos](auto map) {
                                    spdlog::debug("map: {}", typeid(map).name());

                                    std::visit(overloaded{
                                            [this](portal const &v) {
                                                spdlog::debug("portal to '{}' triggered ... ", to_string(v.target));
                                                // select target (new) map
                                                std::visit(
                                                        [this, &v](auto e) {
                                                            const auto target_map = tile_map_by_tag_t<decltype(e), decltype(maps)>{};
                                                            const auto map_size = rect{{0, 0}, extent(target_map)};

                                                            // move player
                                                            player.position = v.position;
                                                            // set new map
                                                            maps = target_map;

                                                            if (view.is_inside(map_size)) {
                                                                spdlog::debug(
                                                                        "new map is smaller than view, therefore center it");
                                                                map_offset = map_size.centered(
                                                                        view.extent).top_left_position;
                                                                // reset camera
                                                                camera = {0, 0};
                                                            } else {
                                                                spdlog::debug(
                                                                        "new map is bigger than view");
                                                                map_offset = {0, 0};
                                                                // move camera
                                                                move_camera(player.position.x, player.position.y);
                                                            }
                                                        },
                                                        v.target);
                                            },
                                            [](chest const &v) {
                                                spdlog::debug("found chest!");
                                            },
                                            [](auto) {
                                                // nothing
                                            }
                                    }, map.entities[p.y][p.x]);
                                }, maps);
                    };

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                        IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                        trigger(player.position);
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
            return {static_cast<decltype(Vector2::x)>((v.x + map_offset.x) * SQUARE_SIZE),
                    static_cast<decltype(Vector2::y)>((v.y + map_offset.y) * SQUARE_SIZE)};
        }

        void draw() const noexcept {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            draw_grid();

            if (!gameOver) {

                std::visit([this](auto map) { draw_map(map); }, maps);
                std::visit([this](auto map) { draw_map_entities(map); }, maps);

                // Draw to player
                DrawRectangleV(world_to_screen(player.position - camera) + (screen_offset / 2), player.size,
                               player.color);

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
            DrawRectangleV(Vector2{static_cast<decltype(Vector2::x)>((x + map_offset.x) * SQUARE_SIZE),
                                   static_cast<decltype(Vector2::y)>((y + map_offset.y) * SQUARE_SIZE)},
                           {SQUARE_SIZE, SQUARE_SIZE}, std::invoke(f, dt.y, dt.x));
        }

        template<typename Map, typename Function>
        void draw_tiles(Function f) const noexcept {
            for (auto y = 0; y < std::min(view.height(), Map::rows_v); ++y) {
                for (auto x = 0; x < std::min(view.width(), Map::cols_v); ++x) {
                    draw_tile(x, y, std::move(f));
                }
            }
        }

        template<typename Map>
        void draw_map(Map map) const noexcept {
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

        template<typename Map>
        void draw_map_entities(Map map) const noexcept {
            const auto func2 = [&map](auto row, auto col) {
                return std::visit(overloaded{
                                          [](portal) { return PINK; },
                                          [](blocked) { return BLACK; },
                                          [](chest) { return GOLD; },
                                          [](auto e) { return BLANK; },
                                  },
                                  map.entities[row][col]);
            };

            draw_tiles<Map>(func2);
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

#include "game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>

enum { MENU, RULES, COUNTDOWN, PLAY, VICTORY };
enum {
    BOARD, BLUE_PIECE, PINK_PIECE, BLUE_CROWN, PINK_CROWN, CURSOR,
    MENU_IDLE, MENU_PLAY, MENU_RULES, RULES_IDLE, RULES_BACK,
    COUNT_3, COUNT_2, COUNT_1, COUNT_0, COUNT_GO,
    BLUE_WIN, BLUE_YES, BLUE_NO, PINK_WIN, PINK_YES, PINK_NO, IMAGE_COUNT
};
static const char *image_names[IMAGE_COUNT] = {
    "tabuleiro", "peca_azul_normal", "peca_rosa_normal", "peca_azul_dama",
    "peca_rosa_dama", "mouse", "menu1", "menu2", "menu3", "regras1",
    "regras2", "3", "2", "1", "0", "go", "azulv", "azulv2", "azulv3",
    "rosav", "rosav2", "rosav3"
};
enum { SELECT_SOUND, COUNT_SOUND, FAIL_SOUND, GOODBYE_SOUND, SOUND_COUNT };
static const char *sound_names[SOUND_COUNT] = {
    "som_selecionar", "contagem", "som_fail", "tururu"
};

typedef struct {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *images[IMAGE_COUNT];
    ALLEGRO_SAMPLE *sounds[SOUND_COUNT];
    ALLEGRO_AUDIO_STREAM *music, *victory;
    Game game;
    int screen, winner, selected_row, selected_col, mouse_x, mouse_y;
    double countdown_start;
    bool audio;
} App;

static bool inside(const App *app, int x1, int y1, int x2, int y2)
{
    return app->mouse_x >= x1 && app->mouse_x <= x2 &&
           app->mouse_y >= y1 && app->mouse_y <= y2;
}

static void play_sound(App *app, int sound)
{
    if (app->sounds[sound])
        al_play_sample(app->sounds[sound], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

static void update_title(App *app, const char *hint)
{
    char title[256];
    if (app->screen == PLAY) {
        snprintf(title, sizeof(title),
                 "Damas Espaciais | %s's turn | Blue: %d | Pink: %d%s%s",
                 app->game.turn == BLUE ? "Blue" : "Pink",
                 game_count(&app->game, BLUE), game_count(&app->game, PINK),
                 hint ? " | " : "", hint ? hint : "");
    } else {
        const char *screen_names[] = {"Menu", "Rules", "Countdown", "Game", "Victory"};
        snprintf(title, sizeof(title), "Damas Espaciais - Space Checkers | %s",
                 screen_names[app->screen]);
    }
    al_set_window_title(app->display, title);
}

static void set_screen(App *app, int screen)
{
    app->screen = screen;
    if (app->music) al_set_audio_stream_playing(app->music, screen == PLAY);
    if (app->victory) {
        if (screen == VICTORY) al_rewind_audio_stream(app->victory);
        al_set_audio_stream_playing(app->victory, screen == VICTORY);
    }
    update_title(app, NULL);
}

static void start_game(App *app)
{
    game_reset(&app->game);
    app->selected_row = app->selected_col = -1;
    app->winner = EMPTY;
    app->countdown_start = al_get_time();
    if (app->music) al_rewind_audio_stream(app->music);
    set_screen(app, COUNTDOWN);
    play_sound(app, COUNT_SOUND);
}

static void draw(App *app)
{
    int background = MENU_IDLE;
    if (app->screen == MENU) {
        if (inside(app, 190, 320, 450, 380)) background = MENU_PLAY;
        else if (inside(app, 170, 410, 470, 470)) background = MENU_RULES;
    } else if (app->screen == RULES) {
        background = inside(app, 230, 575, 410, 610) ? RULES_BACK : RULES_IDLE;
    } else if (app->screen == COUNTDOWN) {
        int step = (int)(al_get_time() - app->countdown_start);
        if (step > 4) step = 4;
        background = COUNT_3 + step;
    } else if (app->screen == PLAY) {
        background = BOARD;
    } else if (app->screen == VICTORY) {
        background = app->winner == BLUE ? BLUE_WIN : PINK_WIN;
        if (inside(app, 120, 550, 265, 600)) ++background;
        else if (inside(app, 350, 550, 490, 600)) background += 2;
    }
    al_draw_bitmap(app->images[background], 0, 0, 0);
    if (app->screen == PLAY) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                int piece = app->game.board[row][col];
                if (piece != EMPTY)
                    al_draw_bitmap(app->images[BLUE_PIECE + piece - 1],
                                   col * 80, row * 80, 0);
            }
        }
        if (app->selected_row >= 0) {
            float x = (float)(app->selected_col * 80);
            float y = (float)(app->selected_row * 80);
            al_draw_rectangle(x + 2, y + 2, x + 78, y + 78,
                              al_map_rgb(255, 230, 80), 3);
        }
    }
    al_draw_bitmap(app->images[CURSOR], app->mouse_x, app->mouse_y, 0);
    al_flip_display();
}

static void board_click(App *app, unsigned int button)
{
    if (app->mouse_x < 0 || app->mouse_x >= 640 ||
        app->mouse_y < 0 || app->mouse_y >= 640) return;
    int row = app->mouse_y / 80;
    int col = app->mouse_x / 80;
    if (button == 1 && app->selected_row < 0) {
        if (game_owner(app->game.board[row][col]) == app->game.turn) {
            app->selected_row = row;
            app->selected_col = col;
            play_sound(app, SELECT_SOUND);
            update_title(app, "Right-click to move");
        } else {
            play_sound(app, FAIL_SOUND);
            update_title(app, "Select one of your pieces");
        }
    } else if (button == 2 && app->selected_row >= 0) {
        if (game_move(&app->game, app->selected_row, app->selected_col, row, col)) {
            app->selected_row = app->selected_col = -1;
            play_sound(app, SELECT_SOUND);
            app->winner = game_winner(&app->game);
            if (app->winner) set_screen(app, VICTORY);
            else update_title(app, NULL);
        } else {
            play_sound(app, FAIL_SOUND);
            update_title(app, "Invalid move; choose another destination");
        }
    }
}

static void cleanup(App *app)
{
    if (app->timer) al_destroy_timer(app->timer);
    if (app->queue) al_destroy_event_queue(app->queue);
    if (app->music) al_destroy_audio_stream(app->music);
    if (app->victory) al_destroy_audio_stream(app->victory);
    if (al_is_audio_installed()) al_stop_samples();
    for (int i = 0; i < SOUND_COUNT; ++i)
        if (app->sounds[i]) al_destroy_sample(app->sounds[i]);
    for (int i = 0; i < IMAGE_COUNT; ++i)
        if (app->images[i]) al_destroy_bitmap(app->images[i]);
    if (app->display) al_destroy_display(app->display);
}

static bool initialize(App *app, bool no_audio)
{
    if (!al_init()) { fputs("Cannot initialize Allegro.\n", stderr); return false; }
    /* CMake copies assets beside the executable, so launching is independent of cwd. */
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    if (!path || !al_change_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP))) {
        if (path) al_destroy_path(path);
        fputs("Cannot open the executable's resource directory.\n", stderr);
        return false;
    }
    al_destroy_path(path);
    if (!al_init_image_addon() || !al_init_primitives_addon() ||
        !al_install_mouse() || !al_install_keyboard()) {
        fputs("Cannot initialize graphics or input.\n", stderr); return false;
    }
    app->display = al_create_display(640, 640);
    app->queue = al_create_event_queue();
    app->timer = al_create_timer(1.0 / 30.0);
    if (!app->display || !app->queue || !app->timer) {
        fputs("Cannot create the game window or event loop.\n", stderr); return false;
    }
    for (int i = 0; i < IMAGE_COUNT; ++i) {
        char name[128];
        snprintf(name, sizeof(name), "imagens/%s.png", image_names[i]);
        app->images[i] = al_load_bitmap(name);
        if (!app->images[i]) {
            fprintf(stderr, "Cannot load %s. Keep imagens/ beside the executable.\n", name);
            return false;
        }
    }
    app->audio = !no_audio && al_install_audio() && al_init_acodec_addon() &&
                 al_reserve_samples(4);
    if (app->audio) {
        for (int i = 0; i < SOUND_COUNT; ++i) {
            char name[128];
            snprintf(name, sizeof(name), "sons/%s.wav", sound_names[i]);
            app->sounds[i] = al_load_sample(name);
            if (!app->sounds[i]) fprintf(stderr, "Sound unavailable: %s\n", name);
        }
        app->music = al_load_audio_stream("sons/trilha_sonora_1.wav", 4, 1024);
        app->victory = al_load_audio_stream("sons/som_campeao.wav", 4, 1024);
        if (app->music) {
            al_set_audio_stream_playmode(app->music, ALLEGRO_PLAYMODE_LOOP);
            al_set_audio_stream_playing(app->music, false);
            al_attach_audio_stream_to_mixer(app->music, al_get_default_mixer());
        } else fputs("Background music unavailable.\n", stderr);
        if (app->victory) {
            al_set_audio_stream_playing(app->victory, false);
            al_attach_audio_stream_to_mixer(app->victory, al_get_default_mixer());
        } else fputs("Victory music unavailable.\n", stderr);
    } else if (!no_audio) fputs("Audio unavailable; continuing without sound.\n", stderr);
    al_register_event_source(app->queue, al_get_display_event_source(app->display));
    al_register_event_source(app->queue, al_get_mouse_event_source());
    al_register_event_source(app->queue, al_get_keyboard_event_source());
    al_register_event_source(app->queue, al_get_timer_event_source(app->timer));
    al_hide_mouse_cursor(app->display);
    al_start_timer(app->timer);
    set_screen(app, MENU);
    return true;
}

int main(int argc, char **argv)
{
    bool no_audio = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--no-audio") == 0) no_audio = true;
        else if (strcmp(argv[i], "--help") == 0) {
            puts("Usage: damas-espaciais [--no-audio]\n"
                 "Left-click selects; right-click moves. Escape returns to the menu.\n"
                 "Close the window to quit.");
            return 0;
        } else { fprintf(stderr, "Unknown option: %s\n", argv[i]); return 1; }
    }
    App app = {0};
    if (!initialize(&app, no_audio)) { cleanup(&app); return 1; }
    bool done = false;
    draw(&app);
    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(app.queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) done = true;
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            if (app.screen == MENU) done = true;
            else set_screen(&app, MENU);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES ||
                   event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            app.mouse_x = event.mouse.x;
            app.mouse_y = event.mouse.y;
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (app.screen == PLAY) board_click(&app, event.mouse.button);
                else if (event.mouse.button == 1) {
                    if (app.screen == MENU) {
                        if (inside(&app, 190, 320, 450, 380)) start_game(&app);
                        else if (inside(&app, 170, 410, 470, 470)) set_screen(&app, RULES);
                    } else if (app.screen == RULES && inside(&app, 230, 575, 410, 610)) {
                        set_screen(&app, MENU);
                    } else if (app.screen == VICTORY) {
                        if (inside(&app, 120, 550, 265, 600)) set_screen(&app, MENU);
                        else if (inside(&app, 350, 550, 490, 600)) done = true;
                    }
                }
            }
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            if (app.screen == COUNTDOWN && al_get_time() - app.countdown_start >= 10.0)
                set_screen(&app, PLAY);
            if (!done) draw(&app);
        }
    }
    cleanup(&app);
    return 0;
}

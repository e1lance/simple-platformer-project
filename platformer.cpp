#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"


void update_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            draw_menu();
        if (IsKeyDown(KEY_ENTER)) {
            game_state = GAME_STATE;
        }
        break;
        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                move_player_horizontally(MOVEMENT_SPEED);
            }

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            move_player_horizontally(-MOVEMENT_SPEED);
        }

        // Calculating collisions to decide whether the player is allowed to jump: don't want them to suction cup to the ceiling or jump midair
        is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
        if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
            player_y_velocity = -JUMP_STRENGTH;
        }
        if (IsKeyDown(KEY_E) && is_colliding ({player_pos.x, player_pos.y + 0.1f}, EXIT)) {
            load_level(1);
        }
        update_player();
        if (IsKeyDown(KEY_P)) {
            game_state = PAUSE_STATE;
        }
        break;
        case PAUSE_STATE:
            if (IsKeyDown(KEY_SPACE)) {
                game_state = GAME_STATE;
            }
        case AGAIN_STATE:
            if (IsKeyDown(KEY_ENTER)) {
                game_state = GAME_STATE;
                load_level();
            }
        case OVER_STATE:
            if (IsKeyDown(KEY_ENTER)) {
                game_state = MENU_STATE;
                reset_player_score();
            }
        case VICTORY_STATE:
            if (IsKeyDown(KEY_ENTER)) {
                game_state = MENU_STATE;
                reset_player_score();
            }
        break;
    }
}

void draw_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            draw_menu();
            break;
        case GAME_STATE:
            draw_level();
            draw_game_overlay();
            draw_player_lives();
            break;
        case PAUSE_STATE:
            draw_pause_menu();
            break;
        case AGAIN_STATE:
            draw_again_menu();
            break;
        case OVER_STATE:
            draw_over_menu();
            level_index = 0;
            load_level();
            break;
        case VICTORY_STATE:
            draw_victory_menu();
            break;
    }

    ClearBackground(BLACK);
}

int main() {
    InitWindow(1920, 1080, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();
    load_level();
    load_music();

    while (!WindowShouldClose()) {
        UpdateMusicStream(main_menu_music);
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }

    unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int checks;

/* Keep checks enabled in Release builds (unlike assert with NDEBUG). */
#define CHECK(condition) do { \
    ++checks; \
    if (!(condition)) { \
        fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

static void test_opening_and_turns(void)
{
    Game game;
    game_reset(&game);
    CHECK(game_count(&game, BLUE) == 12);
    CHECK(game_count(&game, PINK) == 12);
    CHECK(game_winner(&game) == EMPTY);
    CHECK(!game_move(&game, 5, 0, 4, 1));
    CHECK(game_move(&game, 2, 1, 3, 0));
    CHECK(game.turn == PINK);
    CHECK(game.board[2][1] == EMPTY && game.board[3][0] == BLUE);
    CHECK(game_move(&game, 5, 0, 4, 1));
    CHECK(game.turn == BLUE);
    CHECK(!game_move(&game, 3, 0, 2, 1));
    CHECK(!game_move(&game, 3, 0, 3, 2));
    CHECK(game_move(&game, 3, 0, 3, 0)); /* Original pass control. */
    CHECK(game.turn == PINK);
    game_reset(&game);
    CHECK(game.turn == BLUE && game_count(&game, BLUE) == 12);
    CHECK(game_count(&game, PINK) == 12);
}

static void test_captures_and_promotion(void)
{
    Game game = {.turn = BLUE};
    game.board[4][1] = BLUE;
    game.board[5][2] = PINK_KING;
    game.board[7][6] = PINK;
    CHECK(game_move(&game, 4, 1, 5, 2)); /* Click directly on the opponent. */
    CHECK(game.board[6][3] == BLUE && game.board[5][2] == EMPTY);
    CHECK(game_count(&game, BLUE) == 1 && game_count(&game, PINK) == 1);
    CHECK(game_move(&game, 7, 6, 6, 5));
    CHECK(game_move(&game, 6, 3, 7, 4));
    CHECK(game.board[7][4] == BLUE_KING && game_count(&game, BLUE) == 1);
    CHECK(game_winner(&game) == EMPTY); /* A king still counts as a live piece. */
    CHECK(game_move(&game, 6, 5, 5, 6));
    CHECK(game_move(&game, 7, 4, 6, 3)); /* Kings move backward. */

    game = (Game){.turn = PINK};
    game.board[2][3] = PINK;
    game.board[1][2] = BLUE_KING;
    CHECK(game_move(&game, 2, 3, 0, 1)); /* Click on the landing square. */
    CHECK(game.board[0][1] == PINK_KING && game.board[1][2] == EMPTY);
    CHECK(game_count(&game, PINK) == 1);
    CHECK(game_winner(&game) == PINK);

    game = (Game){.turn = BLUE};
    game.board[5][0] = BLUE;
    game.board[6][1] = PINK;
    CHECK(game_move(&game, 5, 0, 6, 1));
    CHECK(game.board[7][2] == BLUE_KING);
    CHECK(game_winner(&game) == BLUE);
}

static void test_invalid_moves_are_atomic(void)
{
    Game game = {.turn = BLUE};
    game.board[4][3] = BLUE;
    game.board[5][4] = PINK;
    game.board[6][5] = PINK;
    Game before = game;
    CHECK(!game_move(&game, 4, 3, 5, 4)); /* Blocked landing. */
    CHECK(memcmp(&game, &before, sizeof(game)) == 0);
    CHECK(!game_move(&game, 4, 3, 6, 1)); /* No opponent to jump. */
    CHECK(!game_move(&game, 4, 3, 7, 6)); /* No flying pieces. */
    CHECK(!game_move(&game, 4, 3, 3, 2)); /* No backward man move. */
    CHECK(memcmp(&game, &before, sizeof(game)) == 0);
    game.board[5][4] = BLUE_KING;
    CHECK(!game_move(&game, 4, 3, 5, 4)); /* Own king cannot be captured. */

    game = (Game){.turn = BLUE};
    game.board[6][1] = BLUE;
    game.board[7][2] = PINK;
    before = game;
    CHECK(!game_move(&game, 6, 1, 7, 2)); /* Would land outside the board. */
    CHECK(memcmp(&game, &before, sizeof(game)) == 0);
    for (int row = -2; row <= 9; ++row) {
        for (int col = -2; col <= 9; ++col) {
            if (!game_inside(row, col)) {
                CHECK(!game_move(&game, row, col, 5, 0));
                CHECK(!game_move(&game, 6, 1, row, col));
            }
        }
    }
}

static void test_blocked_player_and_king_capture(void)
{
    Game game = {.turn = PINK};
    game.board[1][0] = PINK;
    game.board[0][1] = BLUE_KING;
    CHECK(!game_has_moves(&game, PINK));
    CHECK(game_winner(&game) == BLUE);
    game = (Game){.turn = BLUE};
    game.board[4][3] = BLUE_KING;
    game.board[3][2] = PINK_KING;
    CHECK(game_move(&game, 4, 3, 2, 1));
    CHECK(game.board[2][1] == BLUE_KING);
    CHECK(game.board[3][2] == EMPTY);
    CHECK(game_winner(&game) == BLUE);
}

static void test_every_square_and_direction(void)
{
    /* Exercise every board edge and all piece directions under sanitizers. */
    for (int piece = BLUE; piece <= PINK_KING; ++piece) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if ((row + col) % 2 != 1) continue;
                for (int dr = -2; dr <= 2; ++dr) {
                    for (int dc = -2; dc <= 2; ++dc) {
                        Game game = {.turn = game_owner(piece)};
                        game.board[row][col] = piece;
                        bool diagonal = abs(dr) == abs(dc) && abs(dr) > 0;
                        if (abs(dr) == 2 && abs(dc) == 2 &&
                            game_inside(row + dr / 2, col + dc / 2))
                            game.board[row + dr / 2][col + dc / 2] = 3 - game.turn;
                        bool expected = game_inside(row + dr, col + dc) && diagonal &&
                            !(piece == BLUE && dr < 0) && !(piece == PINK && dr > 0);
                        CHECK(game_can_move(&game, row, col, row + dr, col + dc) == expected);
                        if (!expected) continue;
                        int owner = game.turn;
                        CHECK(game_move(&game, row, col, row + dr, col + dc));
                        CHECK(game_count(&game, owner) == 1);
                        CHECK(game_count(&game, 3 - owner) == 0);
                        CHECK(game.board[row][col] == EMPTY);
                    }
                }
            }
        }
    }
}

int main(void)
{
    test_opening_and_turns();
    test_captures_and_promotion();
    test_invalid_moves_are_atomic();
    test_blocked_player_and_king_capture();
    test_every_square_and_direction();
    printf("All game rule tests passed: %u checks; 3,200 square/direction cases.\n", checks);
    return 0;
}

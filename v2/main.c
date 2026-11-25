#include <allegro.h>
#include <stdbool.h>

#define SCREEN_X 800
#define SCREEN_Y 600
#define STATIC_PLAYER_SPRITE "C:\\Users\\tomga\\CLionProjects\\projet_allegro2\\static_player.bmp"
#define RIGHT_PLAYER_SPRITE "C:\\Users\\tomga\\CLionProjects\\projet_allegro2\\right_player.bmp"
#define LEFT_PLAYER_SPRITE "C:\\Users\\tomga\\CLionProjects\\projet_allegro2\\left_player.bmp"
#define BACKGROUND_1 "C:\\Users\\tomga\\CLionProjects\\projet_allegro2\\background_1.bmp"

#define GRAVITY 0.5
#define JUMP_STRENGTH -10
#define GROUND_LEVEL (SCREEN_Y - 100)

void safe_destroy_bitmap(BITMAP* bitmap)
{
    if (bitmap)
    {
        destroy_bitmap(bitmap);
        bitmap = NULL;
    }
}

void init()
{
    allegro_init();
    install_keyboard();
    install_mouse();

    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCREEN_X, SCREEN_Y, 0, 0))
    {
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    set_palette(desktop_palette);
}

typedef struct player
{
    int x, y;
    float vertical_velocity;
    int speed;
    bool is_on_ground;
    BITMAP* sprite;
    BITMAP* right_sprite;
    BITMAP* left_sprite;
    BITMAP* static_sprite;
} player_t;

void apply_gravity(player_t* player)
{
    player->vertical_velocity += GRAVITY;
    player->y += player->vertical_velocity;

    if (player->y + player->sprite->h >= GROUND_LEVEL)
    {
        player->y = GROUND_LEVEL - player->sprite->h;
        player->vertical_velocity = 0;
        player->is_on_ground = true;
    }
    else
    {
        player->is_on_ground = false;
    }
}

void update_player(player_t* player)
{
    bool moving = false;

    if (key[KEY_F])
    {
        player->x += player->speed;
        player->sprite = player->right_sprite;
        moving = true;
    }
    if (key[KEY_S])
    {
        player->x -= player->speed;
        player->sprite = player->left_sprite;
        moving = true;
    }

    if (key[KEY_SPACE] && player->is_on_ground)
    {
        player->vertical_velocity = JUMP_STRENGTH;
        player->is_on_ground = false;
    }

    if (!moving)
    {
        player->sprite = player->static_sprite;
    }

    if (player->x < 0)
    {
        player->x = 0;
    }
    else if (player->x + player->sprite->w > SCREEN_X)
    {
        player->x = SCREEN_X - player->sprite->w;
    }

    apply_gravity(player);
}

void render_player(player_t* player)
{
    if (player->sprite)
    {
        masked_blit(player->sprite, screen, 0, 0, player->x, player->y, player->sprite->w, player->sprite->h);
    }
}

player_t* create_player()
{
    player_t* player = malloc(sizeof(player_t));
    if (!player) return NULL;

    player->x = 100;
    player->y = 100;
    player->speed = 5;
    player->vertical_velocity = 0;
    player->is_on_ground = false;

    player->sprite = load_bitmap(STATIC_PLAYER_SPRITE, NULL);
    player->right_sprite = load_bitmap(RIGHT_PLAYER_SPRITE, NULL);
    player->left_sprite = load_bitmap(LEFT_PLAYER_SPRITE, NULL);
    player->static_sprite = load_bitmap(STATIC_PLAYER_SPRITE, NULL);

    if (!player->sprite || !player->right_sprite || !player->left_sprite || !player->static_sprite)
    {
        allegro_message("Erreur de chargement des sprites\n");

        safe_destroy_bitmap(player->sprite);
        safe_destroy_bitmap(player->right_sprite);
        safe_destroy_bitmap(player->left_sprite);
        safe_destroy_bitmap(player->static_sprite);

        free(player);
        return NULL;
    }

    return player;
}
int main(int argc, char** argv)
{
    init();

    BITMAP* background = load_bitmap(BACKGROUND_1, NULL);
    if (!background)
    {
        allegro_message("Erreur de chargement du background\n");
        return EXIT_FAILURE;
    }

    player_t* player = create_player();

    while (!key[KEY_ESC])
    {
        // Afficher l'image de fond sans couper
        masked_blit(background, screen, 0, 0, 0, 0, background->w, background->h);

        update_player(player);
        render_player(player);

        rest(10);
    }

    /*
    safe_destroy_bitmap(player->sprite);
    safe_destroy_bitmap(player->right_sprite);
    safe_destroy_bitmap(player->left_sprite);
    safe_destroy_bitmap(player->static_sprite);
    safe_destroy_bitmap(background);
    free(player);

    allegro_exit();
    */
}
END_OF_MAIN()

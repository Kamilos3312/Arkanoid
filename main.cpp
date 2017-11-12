#include "allegro.h"
#include <stdio.h>

volatile int counter = 0;
void increment() {counter++;}

struct Point{
    int x;
    int y;
};

int main()
{
    allegro_init();
    set_color_depth(32);
    install_timer();
    install_keyboard();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 720, 450, 0, 0);
    srand(time(NULL));

    LOCK_FUNCTION(increment);
    LOCK_VARIABLE(counter);
    install_int_ex(increment, BPS_TO_TIMER(60));

    BITMAP *backgroundImage = load_bitmap("images/background.bmp", 0);
    BITMAP *blockImage = load_bitmap("images/block01.bmp", 0);
	BITMAP *ballImage = load_bitmap("images/ball.bmp", 0);
    BITMAP *paddleImage = load_bitmap("images/paddle.bmp", 0);
	BITMAP *buffer = create_bitmap(screen->w, screen->h);

	int dx = 6, dy = 5;
	int score = 0;
	char txt[50];
	Point block[1000];
	Point ball;
    Point paddle;
	ball.x = 200;   ball.y = 300;
	paddle.x = 300; paddle.y = 440;

    int n = 0;
	for (int i = 0; i <= 6; i++)
        for (int j = 0; j <= 15; j++){
             block[n].y = i * 20;
             block[n].x = j * 45;
             n++;
        }

    while (!key[KEY_ESC])
    {
        clear_bitmap(buffer);
        while (counter > 0)
        {
            ball.x += dx;
            for (int i = 0; i < n; i++)
                if (ball.x >= block[i].x && ball.x <= (block[i].x + 45) && ball.y >= block[i].y && ball.y <= (block[i].y + 20))  {block[i].x = -100; score += 10; dx = -dx;}

            ball.y += dy;
            for (int i = 0; i < n; i++)
                if (ball.x >= block[i].x && ball.x <= (block[i].x + 45) && ball.y >= block[i].y && ball.y <= (block[i].y + 20))  {block[i].x = -100; score += 10; dy = -dy;}

            if (ball.x <= 0 || ball.x >= 708)  dx = -dx;
            if (ball.y <= 0)  dy = -dy;

            if (key[KEY_RIGHT] && paddle.x < (720 - paddleImage->w)) paddle.x += 6;
            if (key[KEY_LEFT] && paddle.x > 0)  paddle.x -= 6;

            if (ball.x > paddle.x && (ball.x + 12) <= (paddle.x + 90) && (ball.y >= paddle.y))
                {dy = -dy;  dy=-(rand()%3+3);}
            else if (ball.y >= 450) {sprintf(txt, "GAME OVER\nScore: %d", score);   allegro_message(txt); exit(0);}

            for (int i = 0; i <= n; i++){
                if (block[i].x != -100) break;
                sprintf(txt, "YOU WIN\nScore: %d", score);   allegro_message(txt); exit(0);
            }
            counter--;
        }

        draw_sprite(buffer, backgroundImage, 0, 0);
        draw_sprite(buffer, ballImage, ball.x, ball.y);
        draw_sprite(buffer, paddleImage, paddle.x , paddle.y);
        for (int i = 0; i <= n; i++)
            draw_sprite(buffer, blockImage, block[i].x, block[i].y);

        blit(buffer, screen, 0, 0, 0, 0, screen->w, screen->h);
        rest(20);
    }

    destroy_bitmap(blockImage);
	destroy_bitmap(backgroundImage);
	destroy_bitmap(ballImage);
	destroy_bitmap(paddleImage);
	destroy_bitmap(buffer);
	allegro_exit();

	return 0;
}
END_OF_MAIN()

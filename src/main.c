#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define WIDTH 100
#define HEIGHT 50

struct Point {
    int x, y;
};

struct Snake {
    struct Point body[100];
    int length;
    char dir;
};

//Check if any key is being pressed
int kbhit() {
    struct termios oldt, newt;
    int ch, oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


void draw(struct Snake *snake, struct Point *food) {
    system("clear");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                printf("#");
            } else if (i == food->y && j == food->x) {
                printf("*");
            } else {
                int isBody = 0;
                for (int k = 0; k < snake->length; k++) {
                    if (snake->body[k].x == j && snake->body[k].y == i) {
                        printf("o");
                        isBody = 1;
                    }
                }
                if (!isBody) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void moveSnake(struct Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    switch (snake->dir) {
        case 'u':
            snake->body[0].y--;
            break;
        case 'd':
            snake->body[0].y++;
            break;
        case 'l':
            snake->body[0].x--;
            break;
        case 'r':
            snake->body[0].x++;
            break;
    }
}

int main() {

    struct Snake snake = {
        .body = {{5, 5}, {4, 5}, {3, 5}},
        .length = 3,
        .dir = 'r'
    };

    struct Point food = {.x = 15, .y = 10};

    //set a random seed
    srand(time(NULL));

    while (1) {
        // Check for user input
        if (kbhit()) {
            char c = getchar();
            if (c == 'w' && snake.dir != 'd') snake.dir = 'u';
            if (c == 's' && snake.dir != 'u') snake.dir = 'd';
            if (c == 'a' && snake.dir != 'r') snake.dir = 'l';
            if (c == 'd' && snake.dir != 'l') snake.dir = 'r';
        }

        moveSnake(&snake);


        // Check if snake ate the food
        if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
            snake.length++;
            food.x = rand() % (WIDTH - 2) + 1;
            food.y = rand() % (HEIGHT - 2) + 1;
        }

        // Check for collision with walls
        if (snake.body[0].x == 0 || snake.body[0].x == WIDTH - 1 ||
            snake.body[0].y == 0 || snake.body[0].y == HEIGHT - 1) {
            break;
        }

        // Check for collision with snake itself
        for (int i = 1; i < snake.length; i++) {
            if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
                return 0;
            }
        }

        // Draw the game screen
        draw(&snake, &food);
        usleep(100000); // Delay for smoother animation
    }

    printf("Game Over\n");
    return 0;
}

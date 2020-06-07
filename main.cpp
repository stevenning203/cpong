#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>

int paddle_width = 10;
int paddle_height = 100;
int ball_width = 10;
int ball_height = 10;
int player_score = 0;
int computer_score = 0;
int tick = 0;

void end()
{
    std::cout << "Your final score was: " << tick << std::endl << ". Press enter to close the game. ";
    std::cin;
}

double convert(double c, double d)
{
    double percentage = c / d;
    percentage *= 2;
    percentage -= 1;
    return percentage;
}

double dconvert(double w, double t)
{
    double percentage = w / t;
    percentage *= 2;
    return percentage;
}

void drawrect(int x, int y, int width, int height)
{
    double realx = convert(x, 1280);
    double realy = -convert(y, 720);
    double realw = dconvert(width, 1280);
    double realh = dconvert(height, 720);

    glBegin(GL_POLYGON);
    glVertex2d(realx, realy);
    glVertex2d(realx + realw, realy);
    glVertex2d(realx + realw, realy - realh);
    glVertex2d(realx, realy - realh);
    glEnd();
}

class Ball
{
public:
    int position[2] = { 0, 0 };
    int velocity[2] = { 5, 5 };
    void update()
    {
        position[0] += velocity[0];
        position[1] += velocity[1];
    }
    void check_bounce()
    {
        if (position[1] <= 0)
        {
            position[1] = 0;
            velocity[1] *= -1;
        }
        else if (position[1] >= 720 - ball_height)
        {
            position[1] = 720 - ball_height;
            velocity[1] *= -1;
        }
    }
};

class Paddle
{
public:
    int position[2] = { 0, 0 };
    int velocity[2] = { 0, 0 };
    void update()
    {
        position[0] += velocity[0];
        position[1] += velocity[1];
    }
};

Paddle computer_paddle;
Paddle player_paddle;
Ball ball;

int main(void)
{
    computer_paddle.position[0] = 1280 - 2 * paddle_width;
    player_paddle.position[0] = paddle_width;
    player_paddle.position[1] = 640 - paddle_height / 2;
    computer_paddle.position[1] = 640 - paddle_height / 2;
    ball.position[0] = 640 - ball_width / 2;
    ball.position[1] = 360 - ball_height / 2;
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 720, "Pong", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        tick += 1;
        double mousex, mousey;
        glfwGetCursorPos(window, &mousex, &mousey);
        mousey = 720 - mousey;

        drawrect(computer_paddle.position[0], computer_paddle.position[1], paddle_width, paddle_height);
        drawrect(player_paddle.position[0], player_paddle.position[1], paddle_width, paddle_height);
        drawrect(ball.position[0], ball.position[1], ball_width, ball_height);

        glfwSwapBuffers(window);

        glfwPollEvents();
        player_paddle.velocity[1] = -(player_paddle.position[1] - (720 - mousey - paddle_height / 2)) / 10;
        computer_paddle.position[1] = ball.position[1] - paddle_height * 0.5;
        computer_paddle.update();
        player_paddle.update();
        ball.update();
        ball.check_bounce();
        if (ball.position[0] <= 0)
        {
            computer_score += 1;
            ball.position[0] = 640 - ball_width / 2;
            ball.position[1] = 360 - ball_height / 2;
            ball.velocity[0] *= -1;
            ball.velocity[1] *= -1;
        }
        else if (ball.position[0] >= 1280)
        {
            player_score += 1;
            ball.position[0] = 640 - ball_width / 2;
            ball.position[1] = 360 - ball_height / 2;
            ball.velocity[0] *= -1;
            ball.velocity[1] *= -1;
        }
        if (ball.position[0] <= player_paddle.position[0] + paddle_width && ball.position[0] >= player_paddle.position[0] && ball.position[1] >= player_paddle.position[1] && ball.position[1] <= player_paddle.position[1] + paddle_height - ball_height)
        {
            ball.velocity[0] *= -1;
        }
        else if (ball.position[0] >= computer_paddle.position[0] - paddle_width && ball.position[0] <= computer_paddle.position[0] && ball.position[1] >= computer_paddle.position[1] && ball.position[1] <= computer_paddle.position[1] + paddle_height - ball_height)
        {
            ball.velocity[0] *= -1;
        }
        if (tick % 120 == 30 && paddle_height > 40)
        {
            paddle_height -= 1;
            std::cout << paddle_height << " is the paddleheight." << std::endl;
        }
        if (computer_score >= 5)
        {
            end();
            return 0;
        }
    }

    glfwTerminate();
    return 0;
}
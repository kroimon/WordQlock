/**
 * PluginPong.cpp
 * Plugin to play the classic pong game.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginPong.h"
#include "Global.h"

#define PADDLE_WIDTH 4

#define ANGLE_UP         0
#define ANGLE_UP_RIGHT   1
#define ANGLE_DOWN_RIGHT 2
#define ANGLE_DOWN       3
#define ANGLE_DOWN_LEFT  4
#define ANGLE_UP_LEFT    5

/**
 *
 */
PluginPong::PluginPong()
  : Plugin(250, false), // 2.75 millisecond update interval, no realtime sync
    gameOver(false)
{
  randomSeed(millis());
  paddleTop.y = 0;
  paddleBottom.y = MATRIX_ROWS - 1;
}

void PluginPong::show() {
  gameOver = false;

  // Both paddles start in the middle
  paddleTop.x = MATRIX_COLS / 2  - (PADDLE_WIDTH / 2);
  paddleBottom.x = MATRIX_COLS / 2 - (PADDLE_WIDTH / 2);

  // Ball starts in the center of the screen with a random direction
  ball.x = (MATRIX_COLS / 2);
  ball.y = (MATRIX_ROWS / 2) - 1;
  ball.speed = 1;
  ball.angle = random(0, 6);
}

void PluginPong::update(unsigned long timeDiff, boolean realtimeSync) {
  if (!gameOver) {
    // Update the game
    movePaddles();
    moveBall();
    gameOver = isGameOver();
  }

  // Draw it!
  matrix.clear();
  matrix.drawLine(paddleTop.x, paddleTop.y, paddleTop.x + PADDLE_WIDTH - 1, paddleTop.y);
  matrix.drawLine(paddleBottom.x, paddleBottom.y, paddleBottom.x + PADDLE_WIDTH - 1, paddleBottom.y);
  matrix.setPixel(ball.x, ball.y);
}

void PluginPong::movePaddles() {
  // Top paddle is always perfect
  paddleTop.x = ball.x - (PADDLE_WIDTH / 2);

  // Bottom paddle moves slower
  if (ball.x <= paddleBottom.x) {
    paddleBottom.x--;
  } else if (ball.x >= (paddleBottom.x + PADDLE_WIDTH - 1)) {
    paddleBottom.x++;
  }

  // Check borders
  paddleTop.x    = constrain(paddleTop.x,    0, MATRIX_COLS - PADDLE_WIDTH);
  paddleBottom.x = constrain(paddleBottom.x, 0, MATRIX_COLS - PADDLE_WIDTH);
}

void PluginPong::moveBall() {
  boolean done = false;
  
  while (!done) {
    char newX, newY;

    switch (ball.angle) {
      case ANGLE_UP:  // Up
        newX = ball.x;
        newY = ball.y - ball.speed;
        break;
      case ANGLE_UP_RIGHT:  // Up and right
        newX = ball.x + ball.speed;
        newY = ball.y - ball.speed;
        break;
      case ANGLE_DOWN_RIGHT:  // Down and right
        newX = ball.x + ball.speed;
        newY = ball.y + ball.speed;
        break;
      case ANGLE_DOWN:  // Down
        newX = ball.x;
        newY = ball.y + ball.speed;
        break;
      case ANGLE_DOWN_LEFT:  // Down and left
        newY = ball.y + ball.speed;
        newX = ball.x - ball.speed;
        break;
      case ANGLE_UP_LEFT:  // Up and left
        newY = ball.y - ball.speed;
        newX = ball.x - ball.speed;
        break;
    }

    switch (checkHit(newX, newY)) {
      case NO_HIT:
        ball.x = newX;
        ball.y = newY;
        done = true;
        break;
      case HIT_UP:
      case HIT_UPPER_PADDLE:
        ball.angle = random(2, 5); // 2,3,4 = DOWN
        break;
      case HIT_DOWN:
      case HIT_LOWER_PADDLE:
        // Hack
        ball.angle = random(0, 3);
        if (ball.angle == 2)
          ball.angle = 5;
        break;
      case HIT_RIGHT:
        if (ball.angle == ANGLE_DOWN_RIGHT) {
          ball.angle = ANGLE_DOWN_LEFT;
        } else if (ball.angle == ANGLE_UP_RIGHT) {
          ball.angle = ANGLE_UP_LEFT;
        }
        break;
      case HIT_LEFT:
        if (ball.angle == ANGLE_DOWN_LEFT) {
          ball.angle = ANGLE_DOWN_RIGHT;
        } else if (ball.angle == ANGLE_UP_LEFT) {
          ball.angle = ANGLE_UP_RIGHT;
        }
        break;
    }
  }
}

PluginPong::PongHit PluginPong::checkHit(char x, char y) {
  // Borders
  if (x < 0)
    return HIT_LEFT;
  if (y < 0)
    return HIT_UP;
  if (x > MATRIX_COLS - 1)
    return HIT_RIGHT;
  if (y > MATRIX_ROWS - 1)
    return HIT_DOWN;

  // Lower paddle
  if (y == paddleTop.y && x >= paddleTop.x && x < paddleTop.x + PADDLE_WIDTH) {
    return HIT_UPPER_PADDLE;
  }
  // Upper paddle
  if (y == paddleBottom.y && x >= paddleBottom.x && x < paddleBottom.x + PADDLE_WIDTH) {
    return HIT_LOWER_PADDLE;
  }

  return NO_HIT;
}

boolean PluginPong::isGameOver() {
  // Lower paddle
  if (ball.y == paddleTop.y && (ball.x < paddleTop.x || ball.x >= paddleTop.x + PADDLE_WIDTH)) {
    return true;
  }
  // Upper paddle
  if (ball.y == paddleBottom.y && (ball.x < paddleBottom.x || ball.x >= paddleBottom.x + PADDLE_WIDTH)) {
    return true;
  }

  return false;
}


void PluginPong::buttonPressed(PressedButton button) {
  if (gameOver) {
    show(); // reset game
  }
}

/**
 * PluginPong.h
 * Plugin to play the classic pong game.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginPong_h
#define PluginPong_h

#include <Wiring.h>
#include "Plugins.h"

class PluginPong : public Plugin {
  public:
    PluginPong();

    virtual void show();
    virtual void update(unsigned long timeDiff, boolean realtimeSync);
    virtual void buttonPressed(PressedButton button);

  private:
    struct PongPaddle {
      char x;
      char y;
    };
    struct PongBall{
      char x;
      char y;
      byte angle;
      byte speed;
    };
    enum PongHit {
      NO_HIT,
      HIT_UPPER_PADDLE,
      HIT_LOWER_PADDLE,
      HIT_RIGHT,
      HIT_LEFT,
      HIT_UP,
      HIT_DOWN
    };

    PongPaddle paddleTop;
    PongPaddle paddleBottom;
    PongBall   ball;
    boolean gameOver;

    void movePaddles();
    void moveBall();
    PongHit checkHit(char x, char y);
    boolean isGameOver();
};

#endif

#include <iostream>
#include <vector>
#include "SDL.h"

struct Vector2 {
    float x;
    float y;
};

struct Ball {
    Vector2 position;
    Vector2 velocity;
};

class Game {
public:
    Game();
    // Initialise the game
    bool Initialise();
    // Run the game loop until the game is over
    void RunLoop();
    // Shutdown game
    void Shutdown();

private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer created by SDL
    SDL_Renderer* mRenderer;
    // Should the game run
    bool mIsRunning;

    const int THICKNESS = 15;
    const int PADDLE_H = 150.0f;

    Vector2 mPaddlePos;
    Vector2 mPaddleRPos;
    int mPaddleDir;
    int mPaddleRDir;

    Vector2 mBallPos;
    Vector2 mBallVel;

    std::vector<Ball> balls;

    Uint32 mTicksCount;
};

Game::Game() {
    mWindow = nullptr;
    mIsRunning = true;
    
    mPaddlePos = { 50, 384 };
    mPaddleRPos = { 974, 384 };

    mBallPos = { 512, 384 };
    mBallVel = { -200.0f, 235.0f };

    balls.push_back(Ball());

    mTicksCount = 0;
}

bool Game::Initialise() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    if (sdlResult != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100,  // Top left x-coordinate of window
        100,  // Top left y-coordinate of window
        1024, // Width of window
        768,  // Height of window
        0     // Flags (0 for no flags set)
    );

    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,    // Window to create renderer for
        -1,         // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::RunLoop() {
    while (mIsRunning) 
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown() {
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::ProcessInput() {
    
    SDL_Event event;

    // While there are still events in the queue
    while (SDL_PollEvent(&event)) {
        // Handle different event types here
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // Update paddle dir
    mPaddleDir = 0;
    mPaddleRDir = 0;

    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }

    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }


    if (state[SDL_SCANCODE_UP]) {
        mPaddleRDir -= 1;
    }

    if (state[SDL_SCANCODE_DOWN]) {
        mPaddleRDir += 1;
    }
}

void Game::UpdateGame() {
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // Delta time is the difference in ticks since last frame.
    // ( converted to seconds )
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    
    // Update ticks count for next frame.
    mTicksCount = SDL_GetTicks();

    // Clamp maximum delta time value
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // Update paddle position
    if (mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        
        // Make sure paddle doesnt move off screen vertically
        if (mPaddlePos.y < (PADDLE_H / 2.0f + THICKNESS)) {
            mPaddlePos.y = PADDLE_H / 2.0f + THICKNESS;
        }
        else if (mPaddlePos.y > (768.0f - (PADDLE_H / 2.0f) - THICKNESS)) {
            mPaddlePos.y = 768.0f - (PADDLE_H / 2.0f) - THICKNESS;
        }
    }

    if (mPaddleRDir != 0) {
        mPaddleRPos.y += mPaddleRDir * 300.0f * deltaTime;

        // Make sure paddle doesnt move off screen vertically
        if (mPaddleRPos.y < (PADDLE_H / 2.0f + THICKNESS)) {
            mPaddleRPos.y = PADDLE_H / 2.0f + THICKNESS;
        }
        else if (mPaddleRPos.y > (768.0f - (PADDLE_H / 2.0f) - THICKNESS)) {
            mPaddleRPos.y = 768.0f - (PADDLE_H / 2.0f) - THICKNESS;
        }
    }

    // Update ball x & y position
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    if (mBallPos.y <= THICKNESS && mBallVel.y < 0.0f) {
        mBallVel.y *= -1;
    }
    else if (mBallPos.y >= (768.0f - THICKNESS) && mBallVel.y > 0.0f) {
        mBallVel.y *= -1;
    }

    if (mBallPos.x <= THICKNESS && mBallVel.x < 0.0f) {
        mBallVel.x *= -1;
    }
    else if (mBallPos.x >= (1024.0f - THICKNESS) && mBallVel.x > 0.0f) {
        mBallVel.x *= -1;
    }

    float diffL = abs(mPaddlePos.y - mBallPos.y);
    float diffR = abs(mPaddleRPos.y - mBallPos.y);

    if (
        // The y diff is small enough.
        diffL <= PADDLE_H / 2.0f &&
        // Ball is at the correct x-position.
        mBallPos.x <= 60.0f && mBallPos.x >= 55.0f &&
        // Ball is moving to the left.
        mBallVel.x <= 0.0f
       ) 
    {
        mBallVel.x *= -1.0f;
    }

    if (
        // The y diff is small enough.
        diffR <= PADDLE_H / 2.0f &&
        // Ball is at the correct x-position.
        mBallPos.x <= 969.0f && mBallPos.x >= 964.0f &&
        // Ball is moving to the left.
        mBallVel.x >= 0.0f
        )
    {
        mBallVel.x *= -1.0f;
    }
}

void Game::GenerateOutput() {
    // Sets the render draw colour
    SDL_SetRenderDrawColor (
        mRenderer,
        0,      // R
        0,      // G
        0,      // B
        255     // A
    );

    // Clear the back buffer to the current draw colour
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    
    // Drawing the ball and paddle as SDL_Rects

    SDL_Rect ball{
        static_cast<int>(mBallPos.x - THICKNESS / 2),
        static_cast<int>(mBallPos.y - THICKNESS / 2),
        THICKNESS,
        THICKNESS
    };
    SDL_RenderFillRect(mRenderer, &ball);
    
    SDL_Rect paddle {
        static_cast<int>(mPaddlePos.x - THICKNESS / 2),
        static_cast<int>(mPaddlePos.y - PADDLE_H / 2),
        THICKNESS,
        PADDLE_H
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_Rect paddleR{
        static_cast<int>(mPaddleRPos.x - THICKNESS / 2),
        static_cast<int>(mPaddleRPos.y - PADDLE_H / 2),
        THICKNESS,
        PADDLE_H
    };
    SDL_RenderFillRect(mRenderer, &paddleR);


    // Swap the front and back buffers
    SDL_RenderPresent(mRenderer);
}

int main(int argc, char* argv[])
{
    Game game;
    bool success = game.Initialise();

    if (success) {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}

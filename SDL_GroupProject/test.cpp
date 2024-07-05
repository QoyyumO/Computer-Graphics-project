#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

// Constants for window size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// SDL variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// Load a texture from a file
SDL_Texture* loadTexture(const std::string& path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

// Initialize SDL components
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gWindow = SDL_CreateWindow("2D Rendering- Group 5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Clean up and close SDL
void closeSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    SDL_Texture* textureLeft = loadTexture("./Assets/oct.jpg");
    SDL_Texture* textureRight = loadTexture("./Assets/view1.jpg");
    SDL_Texture* textureLeft1 = loadTexture("./Assets/frame.png");
    SDL_Texture* textureRight1 = loadTexture("./Assets/frame1.png");
    SDL_Texture* backgroundTexture = loadTexture("./Assets/view5.jpg");

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(gRenderer);

        // Render the background texture to cover the entire window
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

        // Define position and size for the left image
        SDL_Rect leftDest = { 76, 212, 299, 170 }; // Adjust position and size as needed
        SDL_RenderCopy(gRenderer, textureLeft, NULL, &leftDest);
        
        // Define position and size for the left frame
        SDL_Rect left1Dest = { 63, 200, 326, 195 }; // Adjust position and size as needed
        SDL_RenderCopy(gRenderer, textureLeft1, NULL, &left1Dest);

        // Define position and size for the right image
        SDL_Rect rightDest = { 429, 213, 300, 171 }; // Adjust position and size as needed
        SDL_RenderCopy(gRenderer, textureRight, NULL, &rightDest);
        
        // Define position and size for the right frame
        SDL_Rect right1Dest = { 417, 200, 326, 195 }; // Adjust position and size as needed
        SDL_RenderCopy(gRenderer, textureRight1, NULL, &right1Dest);

        SDL_RenderPresent(gRenderer);
    }

    // Free resources and close SDL
    SDL_DestroyTexture(textureLeft);
    SDL_DestroyTexture(textureRight);
    SDL_DestroyTexture(backgroundTexture);
    closeSDL();

    return 0;
}
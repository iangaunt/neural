#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>

using std::cout;
using std::endl;
using std::ofstream;

const int WIDTH = 20;
const int HEIGHT = 20;

void wipe_screen(int* pixels, int len) {
    for (int i = 0; i < len; i++) {
        pixels[i] = 0x0;
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        "SDL", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        WIDTH * 8, HEIGHT * 8, 
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    
    SDL_Texture* texture = 	texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        WIDTH, HEIGHT
    );

    if (window == NULL) return 1;

    SDL_Event event;
    int* pixels = new int[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        pixels[i] = 0x0;
    }

    ofstream nums;
    nums.open("sets/numbers.data");

    bool quit = false;
    bool mdown = false;

    while (!quit) {
        SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(int));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                    break;
                }  
                
                case SDL_MOUSEBUTTONDOWN: {
                    mdown = true;
                    break;
                }

                case SDL_MOUSEBUTTONUP: {
                    mdown = false;
                    break;
                }

                case SDL_MOUSEMOTION: {
                    if (mdown) {
                        pixels[(event.motion.y) * WIDTH + (event.motion.x)] = 0xFFFFFF;
                    }

                    break;
                }
            
                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE: { 
                            quit = true; 
                            break;
                        }

                        case SDLK_r: {
                            wipe_screen(pixels, WIDTH * HEIGHT);
                            break;
                        }

                        case SDLK_s: {
                            for (int i = 0; i < WIDTH * HEIGHT; i++) {
                                nums << (pixels[i] == 0x0 ? "0" : "1") << ",";
                            }
                            nums << "4\n";

                            wipe_screen(pixels, WIDTH * HEIGHT);
                            break;
                        }
                        
                        default: { break; }
                    }

                    break;
			    } 
            }
        }
    }

    nums.close();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
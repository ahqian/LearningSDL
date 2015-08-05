#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//starts up SDL and SDLImage and returns a bool indicating success.
bool init();

//loads necessary media.
bool loadMedia();

//frees media and closes SDL.
void close();

//loads an SDL surface
SDL_Surface* loadSurface(std::string);

//loads a texture
SDL_Texture* loadTexture(std::string);

//The main window
SDL_Window* gWindow = NULL; //Naming convention of gName to indicate global

//The renderer. Necessary to display textures
SDL_Renderer* gRenderer = NULL;

//the surface contained by the mainWindow
SDL_Surface* gScreenSurface = NULL;

//currently displayed PNG surface
SDL_Surface* mainPNGSurface = NULL;

//currently displayed texture
SDL_Texture* mainTexture = NULL;

//------------------------------- IMPLEMENTATION -----------------

bool init() {
	bool success = true;

	//sdl_init returns -1 if an error occured
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error loading SDL! Error: %s\n", SDL_GetError());
	}
	else {
		//create the main window
		gWindow = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
		if (gWindow == NULL) {
			printf("Error creating the main window! Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Init the renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Error creating Renderer! Error: %s\n", SDL_GetError());
			}
			else {
				//initialize rendering colour
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//init flags for SDL_image
				int imgFlags = IMG_INIT_PNG;

				//IMG_Init will return the flags passed into it if everything initialized properly
				if (IMG_Init(imgFlags) != imgFlags) {
					printf("Error loading SDL_IMG! Error: %s\n", IMG_GetError());
					success = false;
				}
				else {
					//if everything has gone smoothly, get the surface from the window
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}
			}
		}
	}
	return success;
}

void close() {
	SDL_FreeSurface(mainPNGSurface);
	mainPNGSurface = NULL; //remember to set pointers to null after freeing
	SDL_DestroyTexture(mainTexture);
	mainTexture = NULL;

	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//exit SDL_IMG and SDL
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia() {
	bool success = true;
	
	//load any preliminary images here.

	return success;
}

//Loads and optimizes a surface from file.
SDL_Surface* loadSurface(std::string path) {	
	SDL_Surface* optimizedImage = NULL;
	SDL_Surface* loadedImage = IMG_Load(path.c_str());

	if (loadedImage == NULL) {
		printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		optimizedImage = SDL_ConvertSurface(loadedImage, gScreenSurface->format, NULL);
		if (optimizedImage == NULL) {
			printf("Unable to optimize image %s! Error: %s\n", path.c_str(), IMG_GetError());
		}
		SDL_FreeSurface(loadedImage);
	}
	return optimizedImage;
}

//Loads a surface from file and converts it into a texture.
SDL_Texture* loadTexture(std::string path) {
	SDL_Texture* texture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Error loading surface %s! Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (texture == NULL) {
			printf("Couldn't convert surface %s to texture! Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return texture;
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			//main loof flag
			bool quit = false;

			//event handler
			SDL_Event e;

			while (!quit) {
				//sdl poll event return 0 if current event stack is empty
				while (SDL_PollEvent(&e) != 0) {
					//handling for all sorts of events
					if (e.type == SDL_QUIT) {
						quit = true;
					}

					if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_w:
								//do stuff
								break;
							case SDLK_a:
								//do stuff
								break;
							case SDLK_s:
								//do stuff
								break;
							case SDLK_d:
								//do stuff;
								break;
						}
					}
				}

				////apply new png image
				//SDL_BlitSurface(mainPNGSurface, NULL, gScreenSurface, NULL);

				////update the surface with the new image
				//SDL_UpdateWindowSurface(gWindow);

				//Clear Screen
				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderClear(gRenderer);

				//render texture to screen
				SDL_RenderCopy(gRenderer, mainTexture, NULL, NULL);

				//update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();

	return 0;
}
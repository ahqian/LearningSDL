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

//The main window
SDL_Window* mainWindow = NULL;

//the surface contained by the mainWindow
SDL_Surface* screenSurface = NULL;

//currently displayed PNG surface
SDL_Surface* mainPNGSurface = NULL;

//------------------------------- IMPLEMENTATION -----------------

bool init() {
	bool success = true;

	//sdl_init returns -1 if an error occured
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error loading SDL! Error: %s\n", SDL_GetError());
	}
	else {
		//create the main window
		mainWindow = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
		if (mainWindow == NULL) {
			printf("Error creating the main window! Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//init flags for SDL_image
			int imgFlags = IMG_INIT_PNG;

			//IMG_Init will return the flags passed into it if everything initialized properly
			if (IMG_Init(imgFlags) != imgFlags) {
				printf("Error loading SDL_IMG! Error: %s\n", IMG_GetError());
				success = false;
			}
			else {
				//if everything has gone smoothly, get the surface from the window
				screenSurface = SDL_GetWindowSurface(mainWindow);
			}			
		}
	}
	return success;
}

void close() {
	SDL_FreeSurface(mainPNGSurface);
	mainPNGSurface = NULL; //remember to set pointers to null after freeing

	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	SDL_DestroyWindow(mainWindow);

	//exit SDL_IMG and SDL
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia() {
	bool success = true;
	
	//load any preliminary images here.

	return success;
}

SDL_Surface* loadSurface(std::string path) {	
	SDL_Surface* optimizedImage = NULL;
	SDL_Surface* loadedImage = IMG_Load(path.c_str());

	if (loadedImage == NULL) {
		printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		optimizedImage = SDL_ConvertSurface(loadedImage, screenSurface->format, NULL);
		if (optimizedImage == NULL) {
			printf("Unable to optimize image %s! Error: %s\n", path.c_str(), IMG_GetError());
		}
		SDL_FreeSurface(loadedImage);
	}
	return optimizedImage;
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
				}

				//apply new png image
				SDL_BlitSurface(mainPNGSurface, NULL, screenSurface, NULL);

				//update the surface with the new image
				SDL_UpdateWindowSurface(mainWindow);
			}
		}
	}
	close();

	return 0;
}
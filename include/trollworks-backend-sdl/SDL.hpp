#pragma once

#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

std::string SDL_RWreadAll(SDL_RWops* rw, bool freesrc);

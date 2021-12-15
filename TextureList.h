#ifndef TEXTURE_LIST_H
#define TEXTURE_LIST_H

#include <vector>
#include "texture2D.h"

Texture2D normalTile("textures/marble.ppm");
Texture2D specialTile("textures/carpet.ppm");
Texture2D winTile("textures/black.ppm");

std::vector<Texture2D> TextureList = {normalTile, specialTile, winTile};

#endif 
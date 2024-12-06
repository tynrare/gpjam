#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "resources.h"

#define TEXTURES_AUTO_RELOAD
#define ASSETS_SOUNDS_COUNT 4
#define ASSETS_TEXTURES_COUNT 3

static const char* const assets_sounds_filenames[] = {
    SOUNDS_PATH "story_mode.ogg",
    SOUNDS_PATH "switch1.ogg",
    SOUNDS_PATH "chip-lay-1.ogg",
    SOUNDS_PATH "ready.ogg"
};

static const char* const assets_textures_filenames[] = {
    TEXTURES_PATH "palette.png",
    TEXTURES_PATH "suit_hearts.png",
    TEXTURES_PATH "hexagon.png"
};

typedef struct Assets {
    Sound sounds[ASSETS_SOUNDS_COUNT];
    int audio_file_mod_times[ASSETS_SOUNDS_COUNT];
    Texture textures[ASSETS_TEXTURES_COUNT];
     int texture_file_mod_times[ASSETS_SOUNDS_COUNT];
} Assets;

Assets *assets = { 0 };

#define ASSET_SOUND_STORYMODE   assets->sounds[0]
#define ASSET_SOUND_SWITCH1            assets->sounds[1]
#define ASSET_SOUND_CHIP1                   assets->sounds[2]
#define ASSET_SOUND_READY                 assets->sounds[3]

#define ASSET_TEXTURE_PALETTE     assets->textures[0]
#define ASSET_TEXTURE_HEART           assets->textures[1]
#define ASSET_TEXTURE_HEXAGON   assets->textures[2]



static void assets_load_sound(Assets *assets, int index) {
    Sound sound = LoadSound(assets_sounds_filenames[index]);
    /*
    if (texture.id == 0) {
        return;
    }
    */
    if (assets->audio_file_mod_times[index] != 0) {
         UnloadSound(assets->sounds[index]);
    }
    assets->sounds[index] = sound;
    assets->audio_file_mod_times[index] = GetFileModTime(assets_sounds_filenames[index]);
}


static void load_texture(Assets *assets, int index) {
    Texture texture = LoadTexture(assets_textures_filenames[index]);
    if (texture.id == 0) {
        return;
    }
    if (assets->texture_file_mod_times[index] != 0) {
         UnloadTexture(assets->textures[index]);
    }
    assets->textures[index] = texture;
    assets->texture_file_mod_times[index] = GetFileModTime(assets_textures_filenames[index]);
}

void assets_load(Assets *assets) {
    for (int i = 0; i < ASSETS_SOUNDS_COUNT; i++) {
        assets->audio_file_mod_times[i] = 0;
        assets_load_sound(assets, i);
    }
    
    for (int i = 0; i < ASSETS_TEXTURES_COUNT; i++) {
        assets->texture_file_mod_times[i] = 0;
        load_texture(assets, i);
    }
}

void assets_unload(Assets *assets) {
    for (int i = 0; i < ASSETS_SOUNDS_COUNT; i++) {
        assets->audio_file_mod_times[i] = 0;
        UnloadSound(assets->sounds[i]);
    }
   
   for (int i = 0; i < ASSETS_TEXTURES_COUNT; i++) {
        assets->texture_file_mod_times[i] = 0;
        UnloadTexture(assets->textures[i]);
    }
}

void assets_update(Assets *assets) {
 #ifdef TEXTURES_AUTO_RELOAD
  for (int i = 0; i < ASSETS_TEXTURES_COUNT; i++) {
      if (file_modified(assets_textures_filenames[i], assets->texture_file_mod_times[i])) {
         load_texture(assets, i);
      }
  }
 #endif
}

#endif
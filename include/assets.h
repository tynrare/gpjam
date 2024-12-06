#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "resources.h"

#define TEXTURES_AUTO_RELOAD
#define SHADERS_AUTO_RELOAD


#define ASSETS_SOUNDS_COUNT 4
#define ASSETS_TEXTURES_COUNT 3
#define ASSETS_SHADERS_COUNT 3

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

typedef struct TynShaderGeneric {
    Shader shader;
    int time_loc;
    int tpalette_loc; 
} TynShaderGeneric;

static const char* const assets_shaders_filenames[] = {
    SHADERS_PATH "sprite_generic.fs",
    SHADERS_PATH "chromakey.fs",
    SHADERS_PATH "sprite_sdf.fs"
};

typedef struct Assets {
    Sound sounds[ASSETS_SOUNDS_COUNT];
    int audio_file_mod_times[ASSETS_SOUNDS_COUNT];
    Texture textures[ASSETS_TEXTURES_COUNT];
     int texture_file_mod_times[ASSETS_SOUNDS_COUNT];
     TynShaderGeneric shaders[ASSETS_SHADERS_COUNT];
     int shader_file_mod_times[ASSETS_SHADERS_COUNT];
} Assets;

Assets *assets = { 0 };

#define ASSET_SOUND_STORYMODE   assets->sounds[0]
#define ASSET_SOUND_SWITCH1            assets->sounds[1]
#define ASSET_SOUND_CHIP1                   assets->sounds[2]
#define ASSET_SOUND_READY                 assets->sounds[3]

#define ASSET_TEXTURE_PALETTE     assets->textures[0]
#define ASSET_TEXTURE_HEART           assets->textures[1]
#define ASSET_TEXTURE_HEXAGON   assets->textures[2]

#define ASSET_SHADER_SPRITE_GENERIC   assets->shaders[0].shader
#define ASSET_SHADER_CHROMAKEY                assets->shaders[1].shader
#define ASSET_SHADER_SDF                                       assets->shaders[2].shader


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

static void load_shader(Assets *assets, int index) {
     const char *filename = assets_shaders_filenames[index];
    Shader shader = LoadShader(0,  filename);
    if (shader.id == rlGetShaderIdDefault()) { 
        return;
    }
          
    TynShaderGeneric *tsg = &assets->shaders[index];
     if (assets->shader_file_mod_times[index] != 0) {
        UnloadShader(tsg->shader);
     }

    tsg->shader = LoadShader(0,  filename);
    assets->shader_file_mod_times[index] = GetFileModTime(filename);

     tsg->time_loc = GetShaderLocation(tsg->shader , "time");
     tsg->tpalette_loc = GetShaderLocation(tsg->shader , "tex_palette");
         
    SetShaderValue(tsg->shader, tsg->tpalette_loc, &ASSET_TEXTURE_PALETTE, SHADER_UNIFORM_SAMPLER2D);
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
    
    for (int i = 0; i < ASSETS_SHADERS_COUNT; i++) {
        assets->shader_file_mod_times[i] = 0;
        load_shader(assets, i);
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
    
   
    for (int i = 0; i < ASSETS_SHADERS_COUNT; i++) {
        #ifdef SHADERS_AUTO_RELOAD
        if (file_modified(assets_shaders_filenames[i], assets->shader_file_mod_times[i])) {
            load_shader(assets, i);
        }
        #endif
        
        float time = (float)GetTime();
        SetShaderValue(assets->shaders[i].shader, assets->shaders[i].time_loc, &time, SHADER_UNIFORM_FLOAT);
    }


}

#endif
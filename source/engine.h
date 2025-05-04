#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

// You should implement there's methods for engine client:
// void app_init(); // Initialization
// void app_shutdown(); // Turing off
// void app_update(float dt); // Frame Tick

#ifdef __cplusplus
extern "C" {
#endif

// Types
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef int image_handle;

// Image API

image_handle load_image(const char* fn);
int get_image_desc(image_handle i, int* x, int* y);

// Sprite API

void draw_sprite_image(float x, float y, float w, float h, image_handle i);

// General API

int get_width(void);
int get_height(void);
float get_widthf(void);
float get_heightf(void);

void reset_state(void);
void set_2d_matrix(float x, float y, float w, float h);
void set_obj_matrix(float x, float y, float a); // rotation in radians

void set_debug_text(const char* txt);

float get_time(void);

#ifdef __cplusplus
}
#endif

#endif
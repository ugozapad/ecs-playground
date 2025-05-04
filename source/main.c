// Little engine written in ANSI C
//
#include <time.h>
#include "engine.h"

#define SOKOL_LOG_IMPL
#define SOKOL_APP_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLUE_IMPL
#define SOKOL_GL_IMPL
#define SOKOL_GLCORE
#define SOKOL_DEBUGTEXT_IMPL
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gl.h"
#include "sokol_debugtext.h"

extern void app_init();
extern void app_shutdown();
extern void app_update(float dt);

char textbuffer[256];

// Input
uchar keys[SAPP_KEYCODE_MENU+1];

// GFX

sg_pass_action pass_action;
sg_sampler default_sampler;

// BMP loading

#pragma pack(push, 1)

typedef struct {
    ushort type;
    ulong size;
    ushort reserved1;
    ushort reserved2;
    ulong offsetdata;
} BMPHEADER;

typedef struct {
    ulong size;
    long width;
    long height;
    ushort planes;
    ushort bitcount;
    ulong compression;
    ulong sizeimage;
    long xpelspermeter;
    long ypelspermeter;
    ulong clrused;
    ulong clrimportant;
} BMPINFOHEADER;

typedef struct {
    ulong size;
    ushort width;
    ushort height;
    ushort planes;
    ushort bitcount;
} BMPCOLORHEADER;

#pragma pack(pop)

uchar temppixeldata[128 * 128 * 4];
uchar pixeldata[128 * 128 * 4];
sg_image images[64];
int imagecount = 0;

image_handle load_image(const char* fn)
{
    BMPHEADER h;
    BMPINFOHEADER ih;
    BMPCOLORHEADER ch;

    memset(&h, 0, sizeof(h));
    memset(&ih, 0, sizeof(ih));
    memset(&ch, 0, sizeof(ch));

    FILE* fp = fopen(fn, "rb");
    if (!fp) {
        printf("Failed to load BMP file. File '%s' dont exist\n", fn);
        return -1;
    }

    fread(&h, sizeof(h), 1, fp);
    fread(&ih, sizeof(ih), 1, fp);
    fread(&ch, sizeof(ch), 1, fp);

    // sanity check
    if (h.type != 0x4d42) {
        printf("Failed to load BMP file. File '%s' is not a BMP\n", fn);
        return -1;
    }

    // sanity check
    if (ih.width > 128 || ih.height > 128) {
        printf("Failed to load BMP file. File '%s' have size greater than 128x128\n", fn);
        fclose(fp);
        return -1;
    }

    // seek to bitmap pixels
    fseek(fp, h.offsetdata, SEEK_SET);

    // read pixels

    int size = ih.width * ih.height * 4;
    fread(temppixeldata, ih.sizeimage, 1, fp);

    // flip pixels (BGR->RGB) and convert it to RGBA
    for (ulong i = 0, i2 = 0; i < size; i += 4, i2 += 3) {
        pixeldata[i] = temppixeldata[i2+2];
        pixeldata[i+1] = temppixeldata[i2+1];
        pixeldata[i+2] = temppixeldata[i2];
        pixeldata[i+3] = 0xff;
    }

    fclose(fp);

    // create image

    if (imagecount > 64) {
        printf("Failed to load BMP file. Max images reached (%i)\n", imagecount);
        return -1;
    }

    sg_image_desc imagedesc;
    memset(&imagedesc, 0, sizeof(imagedesc));
    imagedesc.type = SG_IMAGETYPE_2D;
    imagedesc.width = ih.width;
    imagedesc.height = ih.height;
    imagedesc.pixel_format = SG_PIXELFORMAT_RGBA8;
    imagedesc.num_mipmaps = 1;
    imagedesc.usage = SG_USAGE_IMMUTABLE;
    imagedesc.data.subimage[0][0].ptr = pixeldata;
    imagedesc.data.subimage[0][0].size = ih.width * ih.height * 4;

    image_handle handle = imagecount++;
    images[handle] = sg_make_image(&imagedesc);
    return handle;
}

int get_image_desc(image_handle i, int* x, int* y)
{
    if (i >= imagecount) {
        printf("Image handle is greaten than image count.\n");
        return -1;
    }

    sg_image_desc desc = sg_query_image_desc(images[i]);
    if (x) *x = desc.width;
    if (y) *y = desc.height;
    return 1;
}

int draw_debug = 0;

void draw_sprite_image(float x, float y, float w, float h, image_handle i)
{
    sgl_enable_texture();
    sgl_texture(images[i], default_sampler);

    sgl_begin_quads();
    sgl_v2f_t2f(x, y, 0.0f, 0.0f);
    sgl_v2f_t2f(x + w, y, 1.0f, 0.0f);
    sgl_v2f_t2f(x + w, y + h, 1.0f, 1.0f);
    sgl_v2f_t2f(x, y + h, 0.0f, 1.0f);
    sgl_end();
    
    sgl_disable_texture();

    if (draw_debug > 0) {

        // outline
        sgl_begin_lines();
        sgl_v2f_c3b(x, y, 255, 0, 0);
        sgl_v2f_c3b(x + w, y, 255, 0, 0);
        sgl_v2f_c3b(x + w, y, 255, 0, 0);
        sgl_v2f_c3b(x + w, y + h, 255, 0, 0);
        sgl_v2f_c3b(x + w, y + h, 255, 0, 0);
        sgl_v2f_c3b(x, y + h, 255, 0, 0);
        sgl_v2f_c3b(x, y + h, 255, 0, 0);
        sgl_v2f_c3b(x, y, 255, 0, 0);
        sgl_end();

        // pivot

        sgl_point_size(5.f);
        sgl_begin_points();
        sgl_v2f_c3b(x + w * 0.5f, y + h * 0.5f, 255, 255, 0);
        sgl_end();
    }
}

int get_width(void)
{
    return sapp_width();
}

int get_height(void)
{
    return sapp_height();
}

float get_widthf(void)
{
    return sapp_widthf();
}

float get_heightf(void)
{
    return sapp_heightf();
}

void reset_state(void)
{
    sgl_set_context(SGL_DEFAULT_CONTEXT);
    sgl_defaults();
}

void set_2d_matrix(float x, float y, float w, float h)
{
    sgl_matrix_mode_projection();
    sgl_load_identity();
    sgl_ortho(x, w, h, y, -1.0f, 1.0f);
}

void set_obj_matrix(float x, float y, float a)
{
    sgl_matrix_mode_modelview();
    sgl_load_identity();

    sgl_translate(x, y, 0.f);
    sgl_rotate(sgl_rad(a), 0.f, 0.f, 1.f);
}

void set_debug_text(const char* txt)
{
    strcpy(textbuffer, txt);
}

float get_time(void)
{
    float t = (float)clock() / CLOCKS_PER_SEC;
    return t;
}

static void init(void)
{
    // initialize sokol gfx
    sg_desc sdesc;
    memset(&sdesc, 0, sizeof(sdesc));
    sdesc.environment = sglue_environment();
    sdesc.logger.func = slog_func;
    sg_setup(&sdesc);

    // initialize sokol gl
    sgl_desc_t sgldesc;
    memset(&sgldesc, 0, sizeof(sgldesc));
    sgldesc.logger.func = slog_func;
    sgl_setup(&sgldesc);

    // initialize sokol debug font

    sdtx_desc_t dbgfont;
    memset(&dbgfont, 0, sizeof(dbgfont));
    dbgfont.fonts[0] = sdtx_font_cpc();
    //dbgfont.fonts[0] = sdtx_font_c64();
    dbgfont.logger.func = slog_func;
    sdtx_setup(&dbgfont);

    // help text
    sdtx_canvas(sapp_width() * 0.5f, sapp_height() * 0.5f);
    sdtx_pos(0.0f, 0.0f);
    sdtx_color3b(255, 255, 255);

    // initialize pass action
    memset(&pass_action, 0, sizeof(pass_action));
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value.r = 0.5f;
    pass_action.colors[0].clear_value.g = 0.5f;
    pass_action.colors[0].clear_value.b = 0.5f;
    pass_action.colors[0].clear_value.a = 1.0f;

    // create default sampler
    sg_sampler_desc smpdesc;
    memset(&smpdesc, 0, sizeof(smpdesc));
    smpdesc.wrap_u = SG_WRAP_CLAMP_TO_BORDER;
    smpdesc.wrap_v = SG_WRAP_CLAMP_TO_BORDER;
    smpdesc.min_filter = SG_FILTER_NEAREST;
    smpdesc.mag_filter = SG_FILTER_NEAREST;
    default_sampler = sg_make_sampler(&smpdesc);

    // reset
    reset_state();

    // initialize app
    app_init();
}

static void frame(void)
{
    // update app
    app_update(sapp_frame_duration());

    if (textbuffer[0] != 0x0)
        sdtx_puts(textbuffer);

    /* Render the sokol-gfx default pass, all sokol-gl commands
       that happened so far are rendered inside sgl_draw(), and this
       is the only sokol-gl function that must be called inside
       a sokol-gfx begin/end pass pair.
       sgl_draw() also 'rewinds' sokol-gl for the next frame.
    */
    sg_pass pass;
    memset(&pass, 0, sizeof(pass));
    pass.action = pass_action;
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    sgl_draw();

    //__dbgui_draw();

    sdtx_draw();

    sg_end_pass();
    sg_commit();
}

static void cleanup(void)
{
    // shutdown app
    app_shutdown();

    sdtx_shutdown();
    sgl_shutdown();
    sg_shutdown();
}

static void event(const sapp_event* ev)
{
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
        keys[ev->key_code] = 1;
    } else if (ev->type == SAPP_EVENTTYPE_KEY_UP) {
        keys[ev->key_code] = 0;
    }

    // reset keys on unfocus
    if (ev->type == SAPP_EVENTTYPE_UNFOCUSED) {
        memset(keys, 0, sizeof(keys));
    }
}

sapp_desc sokol_main(int argc, char* argv[])
{
    sapp_desc app_desc;
    memset(&app_desc, 0, sizeof(app_desc));
    app_desc.window_title = "ECS Game";
    app_desc.width = 1024;
    app_desc.height = 768;
    app_desc.init_cb = init;
    app_desc.frame_cb = frame;
    app_desc.cleanup_cb = cleanup;
    app_desc.event_cb = event;
    app_desc.logger.func = slog_func;
    return app_desc;
}

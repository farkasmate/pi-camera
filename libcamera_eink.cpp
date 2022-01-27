/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020, Raspberry Pi (Trading) Ltd.
 *
 * Based on:
 *   libcamera_hello.cpp - libcamera "hello world" app.
 *   libcamera_still.cpp - libcamera stills capture app.
 */

#include <chrono>

#include "core/frame_info.hpp"
#include "core/libcamera_app.hpp"
#include "core/still_options.hpp"

#include "image/image.hpp"

using namespace std::placeholders;

extern "C"
{
    #include "DEV_Config.h"
    #include "EPD_2in13_V2.h"
    #include "GUI_Paint.h"
};

// FIXME: remove globals
UBYTE *Image;
UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;

static void eink_open()
{
    if((Image = (UBYTE *)malloc(Imagesize)) == NULL)
        throw std::runtime_error("Failed to allocate eink memory");

    Paint_NewImage(Image, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 0, WHITE);
    Paint_SelectImage(Image);
    Paint_SetMirroring(MIRROR_VERTICAL);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(0, 0, "Camera is", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(0, 20, "starting...", &Font16, WHITE, BLACK);

    if(DEV_Module_Init() != 0)
        throw std::runtime_error("Failed to initialize eink module");
}

static void eink_draw_focus(int focus)
{
    Paint_Clear(WHITE);
    Paint_DrawString_EN(0, 0, "FOCUS:", &Font16, WHITE, BLACK);
    Paint_DrawNum(0, 20, focus, &Font16, BLACK, WHITE);
}

static void eink_display_base()
{
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_DisplayPartBaseImage(Image);

    EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
}

static void eink_display_partial()
{
    EPD_2IN13_V2_DisplayPart(Image);
}

static void eink_close()
{
    EPD_2IN13_V2_Sleep();
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    DEV_Module_Exit();

    free(Image);
    Image = NULL;
}

static StillOptions* get_still_options(LibcameraApp &app)
{
    return static_cast<StillOptions *>(app.GetOptions());
}

static bool is_request_complete(LibcameraApp::Msg msg)
{
    if (msg.type == LibcameraApp::MsgType::Quit)
        return false;

    if (msg.type != LibcameraApp::MsgType::RequestComplete)
        throw std::runtime_error("unrecognised message!");

    return true;
}

static bool viewfinder_loop(LibcameraApp &app)
{
    StillOptions const *options = get_still_options(app);

    app.ConfigureViewfinder();
    app.StartCamera();

    auto start_time = std::chrono::high_resolution_clock::now();

    eink_display_base();

    while(true)
    {
        LibcameraApp::Msg msg = app.Wait();

        if (!is_request_complete(msg))
            return false;

        if (!app.ViewfinderStream())
            throw std::runtime_error("ViewfinderStream is not available");

        // timeout
        auto now = std::chrono::high_resolution_clock::now();
        if (options->timeout && now - start_time > std::chrono::milliseconds(options->timeout))
        {
            std::cerr << "viewfinder timed out, saving photo" << std::endl;
            app.StopCamera();
            app.Teardown();

            return true;
        }

        // print focus value
        CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
        FrameInfo frame_info(completed_request->metadata);
        std::cout << "FOCUS: " << frame_info.focus << std::endl;

        eink_draw_focus(frame_info.focus);
        eink_display_partial();
    }
}

static void save_still(LibcameraApp &app)
{
    StillOptions const *options = get_still_options(app);

    app.ConfigureStill(LibcameraApp::FLAG_STILL_NONE);
    app.StartCamera();

    LibcameraApp::Msg msg = app.Wait();

    if (!is_request_complete(msg))
        return;

    app.StopCamera();
    std::cerr << "Still capture image received" << std::endl;

    if (options->output.empty())
    {
        std::cerr << "No output specified, skip saving still" << std::endl;
        return;
    }

    std::string filename = options->output;

    libcamera::Stream *stream = app.StillStream();
    CompletedRequestPtr &payload = std::get<CompletedRequestPtr>(msg.payload);
    StreamInfo info = app.GetStreamInfo(stream);
    const std::vector<libcamera::Span<uint8_t>> mem = app.Mmap(payload->buffers[stream]);
    jpeg_save(mem, info, payload->metadata, filename, app.CameraId(), options);

    return;
}

// The main event loop for the application.

static void event_loop(LibcameraApp &app)
{
    app.OpenCamera();

    bool ready_to_save_still = viewfinder_loop(app);

    if (!ready_to_save_still)
        throw std::runtime_error("Can't save photo");

    save_still(app);

    // DO NOT call app.CloseCamera() explicitly
}

int main(int argc, char *argv[])
{
    eink_open();

    try
    {
        LibcameraApp app(std::make_unique<StillOptions>());
        StillOptions *options = get_still_options(app);

        if (options->Parse(argc, argv))
        {
            // hardcode options
            options->nopreview = true;
            // TODO: lores viewfinder for eink preview?

            if (options->verbose)
                options->Print();

            event_loop(app);
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "ERROR: *** " << e.what() << " ***" << std::endl;
        eink_close();

        return -1;
    }

    eink_close();

    return 0;
}

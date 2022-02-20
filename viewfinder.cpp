#include "eink.cpp"

#include "core/frame_info.hpp"
#include "core/libcamera_app.hpp"
#include "core/still_options.hpp"

#include "libcamera_helper.cpp"

// FIXME: remove globals
bool shutter = false;

static void draw_viewfinder(UBYTE *image, std::vector<libcamera::Span<uint8_t>> const &mem, StreamInfo const &info)
{
    unsigned w = info.width, h = info.height, stride = info.stride;
    uint8_t *Y = (uint8_t *)mem[0].data();
    uint8_t *U = Y + stride * h;
    uint8_t *V = U + (stride / 2) * (h / 2); // FIXME: /4

    for (uint8_t j = 0; j < h; j++)
    {
        for (uint8_t i = 0; i < w; i++)
        {
            uint8_t yValue = *(Y + j * stride + i);
            uint8_t uValue = *(U + (j / 2) * (stride / 2) + (i / 2));
            uint8_t vValue = *(V + (j / 2) * (stride / 2) + (i / 2));

            int rTmp = yValue + (351*(vValue-128))>>8;
            int gTmp = yValue - (179*(vValue-128) + 86*(uValue-128))>>8;
            int bTmp = yValue + (443*(uValue-128))>>8;

            uint8_t r = std::clamp(rTmp, 0, 255);
            uint8_t g = std::clamp(gTmp, 0, 255);
            uint8_t b = std::clamp(bTmp, 0, 255);

            // FIXME: 16 is from ceil(EPD_2IN13_V2_WIDTH / 8F) see ImageSize
            // FIXME: draw bitmap in a "window"
            if (yValue > 4 * bayer[j%8][i%8])
                image[(EINK_HEIGHT - 1 - i) * 16 + (EINK_WIDTH - j)/8] |= 1UL<<(7-((EINK_WIDTH - j)%8));
            else
                image[(EINK_HEIGHT - 1 - i) * 16 + (EINK_WIDTH - j)/8] &= ~(1UL << (7-((EINK_WIDTH - j)%8)));
        }
    }
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
            std::cerr << "viewfinder timed out" << std::endl;
            app.StopCamera();
            app.Teardown();

            return false;
        }

        // shutter
        if (shutter)
        {
            std::cerr << "saving photo" << std::endl;
            app.StopCamera();
            app.Teardown();

            return true;
        }

        // draw focus value
        CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
        FrameInfo frame_info(completed_request->metadata);
        libcamera::Stream *stream = app.LoresStream();
        StreamInfo info = app.GetStreamInfo(stream);
        const std::vector<libcamera::Span<uint8_t>> mem = app.Mmap(completed_request->buffers[stream]);

        eink_clear();
        eink_draw_focus(frame_info.focus);
        eink_draw_time((now - start_time) / std::chrono::seconds(1));

        draw_viewfinder(Image, mem, info);

        eink_display_partial();
    }
}

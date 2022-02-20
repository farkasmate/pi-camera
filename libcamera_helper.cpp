#pragma once

#include "core/libcamera_app.hpp"
#include "core/still_options.hpp"

#include "image/image.hpp"

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

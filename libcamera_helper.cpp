#pragma once

#include "core/libcamera_app.hpp"
#include "core/still_options.hpp"

#include "image/image.hpp"

class LibcameraEinkApp : public LibcameraApp
{
    private:
    Msg msg = Msg(MsgType::Quit);

    public:
    LibcameraEinkApp() : LibcameraApp(std::make_unique<StillOptions>()) {}

    StillOptions *GetOptions() const { return static_cast<StillOptions *>(options_.get()); }

    bool IsRequestComplete()
    {
        if (msg.type == MsgType::Quit)
            return false;

        if (msg.type != MsgType::RequestComplete)
            throw std::runtime_error("unrecognised message!");

        return true;
    }

    void SaveStill()
    {
        ConfigureStill(FLAG_STILL_NONE);
        StartCamera();

        Wait();

        if (!IsRequestComplete())
            return;

        StopCamera();
        std::cerr << "Still capture image received" << std::endl;

        if (options_->output.empty())
        {
            std::cerr << "No output specified, skip saving still" << std::endl;
            return;
        }

        std::string filename = options_->output;

        libcamera::Stream *stream = StillStream();
        CompletedRequestPtr &payload = std::get<CompletedRequestPtr>(msg.payload);
        StreamInfo info = GetStreamInfo(stream);
        const std::vector<libcamera::Span<uint8_t>> mem = Mmap(payload->buffers[stream]);
        jpeg_save(mem, info, payload->metadata, filename, CameraId(), GetOptions());

        return;
    }

    Msg Wait()
    {
        msg = LibcameraApp::Wait();
        return msg;
    }
};

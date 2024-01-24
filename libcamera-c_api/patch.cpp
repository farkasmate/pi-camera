#include "patch.h"

extern "C" {

void libcamera_camera_configuration_set_orientation(libcamera_camera_configuration_t *config, libcamera_camera_configuration_orientation_t orientation) {
    config->orientation = orientation;
}

const size_t libcamera_camera_streams_size(const libcamera_camera_t *cam) {
    return cam->get()->streams().size();
}

const int libcamera_control_focus_fo_m() {
    return libcamera::controls::FOCUS_FO_M;
}

}

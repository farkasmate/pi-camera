#include "patch.h"

extern "C" {

void libcamera_camera_configuration_set_transform(libcamera_camera_configuration_t *config, libcamera_camera_configuration_transform_t transform) {
    config->transform = transform;
}

const size_t libcamera_camera_streams_size(const libcamera_camera_t *cam) {
    return cam->get()->streams().size();
}

}

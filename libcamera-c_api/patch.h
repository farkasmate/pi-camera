#ifndef __LIBCAMERA_C_PATCH__
#define __LIBCAMERA_C_PATCH__

#include "camera.h"

#ifdef __cplusplus
#include <libcamera/camera.h>
#include <libcamera/libcamera.h>

enum libcamera_camera_configuration_orientation {
    Rotate0 = 1,
    Rotate0Mirror,
    Rotate180,
    Rotate180Mirror,
    Rotate90Mirror,
    Rotate270,
    Rotate270Mirror,
    Rotate90,
};

typedef libcamera::Orientation libcamera_camera_configuration_orientation_t;

extern "C" {
#else
typedef enum libcamera_camera_configuration_orientation libcamera_camera_configuration_orientation_t;
#endif

void libcamera_camera_configuration_set_orientation(libcamera_camera_configuration_t *config, libcamera_camera_configuration_orientation_t orientation);
const size_t libcamera_camera_streams_size(const libcamera_camera_t *cam);
const int libcamera_control_focus_fo_m();

#ifdef __cplusplus
}
#endif

#endif

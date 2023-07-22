#ifndef __LIBCAMERA_C_PATCH__
#define __LIBCAMERA_C_PATCH__

#include "camera.h"

#ifdef __cplusplus
#include <libcamera/camera.h>

enum libcamera_camera_configuration_transform {
    Identity = 0,
    Rot0 = Identity,
    HFlip = 1,
    VFlip = 2,
    HVFlip = HFlip | VFlip,
    Rot180 = HVFlip,
    Transpose = 4,
    Rot270 = HFlip | Transpose,
    Rot90 = VFlip | Transpose,
    Rot180Transpose = HFlip | VFlip | Transpose
};

typedef libcamera::Transform libcamera_camera_configuration_transform_t;

extern "C" {
#else
typedef enum libcamera_camera_configuration_transform libcamera_camera_configuration_transform_t;
#endif

void libcamera_camera_configuration_set_transform(libcamera_camera_configuration_t *config, libcamera_camera_configuration_transform_t transform);
const size_t libcamera_camera_streams_size(const libcamera_camera_t *cam);

#ifdef __cplusplus
}
#endif

#endif

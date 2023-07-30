module PiCamera
  class Cam
    # NOTE: Keep ordering
    @[Link("stdc++")]
    @[Link("camera-base")]
    @[Link("camera")]
    @[Link("camera_c_api")]
    lib LibCamera
      alias Camera = Pointer(Void)
      alias CameraConfigurationList = Pointer(Void)
      alias CameraList = Pointer(Void)
      alias CameraManager = Pointer(Void)
      alias ControlList = Pointer(Void)
      alias ControlValue = Pointer(Void)
      alias FrameBuffer = Pointer(Void)
      alias FrameBufferAllocator = Pointer(Void)
      alias FrameBufferList = Pointer(Void)
      alias FrameBufferPlane = Pointer(Void)
      alias FrameBufferPlaneList = Pointer(Void)
      alias Request = Pointer(Void)
      alias Stream = Pointer(Void)

      enum ConfigurationStatus
        VALID = 0
        ADJUSTED = 1
        INVALID = 2
      end

      enum ControlId
        FOCUS = 20
      end

      struct PixelFormat
        fourcc : FourCC
        modifier : UInt64
      end

      enum RequestStatus
        PENDING = 0
        COMPLETE = 1
        CANCELLED = 2
      end

      enum ReuseFlag
        DEFAULS = 0
        REUSE_BUFFERS = 1
      end

      struct Size
        width : LibC::UInt
        height : LibC::UInt
      end

      struct StreamConfiguration
        pixel_format : PixelFormat
        size : Size
        stride : LibC::UInt
        frame_size : LibC::UInt
        buffer_count : LibC::UInt
      end

      enum StreamRole
        RAW = 0
        STILL_CAPTURE = 1
        VIDEO_RECORDING = 2
        VIEW_FINDER = 3
      end

      fun camera_manager_create = libcamera_camera_manager_create : CameraManager
      fun camera_manager_destroy = libcamera_camera_manager_destroy(manager : CameraManager) : Void
      fun camera_manager_start = libcamera_camera_manager_start(manager : CameraManager) : LibC::Int
      fun camera_manager_stop = libcamera_camera_manager_stop(manager : CameraManager) : Void
      fun camera_manager_cameras = libcamera_camera_manager_cameras(manager : CameraManager) : CameraList

      fun camera_list_destroy = libcamera_camera_list_destroy(camera_list : CameraList) : Void
      fun camera_list_size = libcamera_camera_list_size(camera_list : CameraList) : LibC::SizeT
      fun camera_list_get = libcamera_camera_list_get(camera_list : CameraList, index : LibC::SizeT) : Camera

      fun camera_destroy = libcamera_camera_destroy(camera : Camera) : Void
      fun camera_acquire = libcamera_camera_acquire(camera : Camera) : LibC::Int
      fun camera_release = libcamera_camera_release(camera : Camera) : LibC::Int
      fun camera_generate_configuration = libcamera_camera_generate_configuration(camera : Camera, roles : Pointer(StreamRole), roles_size : LibC::SizeT) : CameraConfigurationList
      fun camera_configure = libcamera_camera_configure(camera : Camera, configs : CameraConfigurationList) : LibC::Int
      fun camera_create_request = libcamera_camera_create_request(camera : Camera, cookie : UInt64) : Request
      fun camera_queue_request = libcamera_camera_queue_request(camera : Camera, request : Request) : LibC::Int
      fun camera_start = libcamera_camera_start(camera : Camera, controls : ControlList) : LibC::Int
      fun camera_stop = libcamera_camera_stop(camera : Camera) : LibC::Int
      fun camera_streams_size = libcamera_camera_streams_size(camera : Camera) : LibC::SizeT

      fun camera_configuration_destroy = libcamera_camera_configuration_destroy(configs : CameraConfigurationList) : Void
      fun camera_configuration_size = libcamera_camera_configuration_size(configs : CameraConfigurationList) : LibC::SizeT
      fun camera_configuration_at = libcamera_camera_configuration_at(configs : CameraConfigurationList, index : LibC::SizeT) : Pointer(StreamConfiguration)
      fun camera_configuration_validate = libcamera_camera_configuration_validate(configs : CameraConfigurationList) : ConfigurationStatus
      fun camera_configuration_set_transform = libcamera_camera_configuration_set_transform(configs : CameraConfigurationList, transform : Transform) : Void

      fun control_list_get = libcamera_control_list_get(controls : ControlList, id : ControlId) : ControlValue

      fun control_value_get = libcamera_control_value_get(value : ControlValue) : Pointer(Void)

      fun framebuffer_allocator_create = libcamera_framebuffer_allocator_create(camera : Camera) : FrameBufferAllocator
      fun framebuffer_allocator_destroy = libcamera_framebuffer_allocator_destroy(allocator : FrameBufferAllocator) : Void
      fun framebuffer_allocator_allocate = libcamera_framebuffer_allocator_allocate(allocator : FrameBufferAllocator, stream : Stream) : LibC::Int
      fun framebuffer_allocator_free = libcamera_framebuffer_allocator_free(allocator : FrameBufferAllocator, stream : Stream) : LibC::Int
      fun framebuffer_allocator_buffers = libcamera_framebuffer_allocator_buffers(allocator : FrameBufferAllocator, stream : Stream) : FrameBufferList

      fun framebuffer_list_get = libcamera_framebuffer_list_get(buffers : FrameBufferList, index : LibC::SizeT): FrameBuffer

      fun framebuffer_planes = libcamera_framebuffer_planes(buffer : FrameBuffer) : FrameBufferPlaneList
      fun framebuffer_planes_at = libcamera_framebuffer_planes_at(planes : FrameBufferPlaneList, index : LibC::SizeT) : FrameBufferPlane
      fun framebuffer_plane_fd = libcamera_framebuffer_plane_fd(plane : FrameBufferPlane) : LibC::Int
      fun framebuffer_plane_length = libcamera_framebuffer_plane_length(plane : FrameBufferPlane) : LibC::SizeT

      fun stream_configuration_stream = libcamera_stream_configuration_stream(config : Pointer(StreamConfiguration)) : Stream

      fun request_destroy = libcamera_request_destroy(request : Request) : Void
      fun request_add_buffer = libcamera_request_add_buffer(request : Request, stream : Stream, buffer : FrameBuffer) : LibC::Int
      fun request_reuse = libcamera_request_reuse(request : Request, flags : ReuseFlag) : Void
      fun request_status = libcamera_request_status(request : Request) : RequestStatus
      fun request_metadata = libcamera_request_metadata(request : Request) : ControlList
    end
  end
end

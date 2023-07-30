require "./bindings"
require "./image_data"
require "./request"

module PiCamera
  class Cam
    class Camera
      @still_image : ImageData?
      @viewfinder_image : ImageData?

      def initialize(cameras : LibCamera::CameraList, index : Int)
        @camera = LibCamera.camera_list_get(cameras, index)
        raise "Can't aquire camera" if LibCamera.camera_acquire(@camera) < 0

        @allocator = LibCamera.framebuffer_allocator_create(@camera)

        at_exit do
          LibCamera.camera_stop(@camera)
          @allocator.try { |allocator| LibCamera.framebuffer_allocator_destroy(allocator) }
          LibCamera.camera_release(@camera)
          LibCamera.camera_destroy(@camera)
        end
      end

      private def allocate_memory(request : Request, config : LibCamera::StreamConfiguration, stream : LibCamera::Stream) : ImageData
        raise "Failed to allocate memory" if LibCamera.framebuffer_allocator_allocate(@allocator, stream) < 1

        # NOTE: Single buffers only, RGB is single-plane, YUV420 has 3 planes, but we only need the first for monochrome image
        buffers = LibCamera.framebuffer_allocator_buffers(@allocator, stream)
        buffer = LibCamera.framebuffer_list_get(buffers, 0)

        planes = LibCamera.framebuffer_planes(buffer)
        plane = LibCamera.framebuffer_planes_at(planes, 0)

        plane_length = LibCamera.framebuffer_plane_length(plane)
        fd = LibCamera.framebuffer_plane_fd(plane)
        memory = LibC.mmap(nil, plane_length, LibC::PROT_READ | LibC::PROT_WRITE, LibC::MAP_SHARED, fd, 0)

        request.add_buffer(stream, buffer)

        return ImageData.new(
          bytes: memory.as(Pointer(UInt8)),
          bytesize: plane_length,
          width: config.size.width,
          height: config.size.height,
          stride: config.stride,
          format: config.pixel_format.fourcc,
        )
      end

      def configure(still_config : StreamConfig, viewfinder_config : StreamConfig?, transform : Transform) : {ImageData, ImageData?}
        roles = [LibCamera::StreamRole::STILL_CAPTURE]
        roles << LibCamera::StreamRole::VIEW_FINDER if viewfinder_config && streams_size > 1
        configs = LibCamera.camera_generate_configuration(@camera, roles.to_unsafe, roles.size)

        LibCamera.camera_configuration_set_transform(configs, transform)

        still = LibCamera.camera_configuration_at(configs, 0)
        still.value.buffer_count = 1
        still.value.pixel_format.fourcc = still_config.four_cc
        still.value.size.width = still_config.width if still_config.width > 0
        still.value.size.height = still_config.height if still_config.height > 0

        if viewfinder_config && LibCamera.camera_configuration_size(configs) > 1
          viewfinder = LibCamera.camera_configuration_at(configs, 1)
          viewfinder.value.buffer_count = 1
          viewfinder.value.pixel_format.fourcc = viewfinder_config.four_cc
          viewfinder.value.size.width = viewfinder_config.width if viewfinder_config.width > 0
          viewfinder.value.size.height = viewfinder_config.height if viewfinder_config.height > 0
        end

        p! LibCamera.camera_configuration_validate(configs)
        raise "Failed to configure camera" if LibCamera.camera_configure(@camera, configs) < 0

        still_stream = LibCamera.stream_configuration_stream(still)
        @still_image = allocate_memory(request, still.value, still_stream)

        if viewfinder
          viewfinder_stream = LibCamera.stream_configuration_stream(viewfinder)
          @viewfinder_image = allocate_memory(request, viewfinder.value, viewfinder_stream)
        end

        at_exit do
          LibCamera.camera_configuration_destroy(configs)

          @allocator.try do |allocator|
            LibCamera.framebuffer_allocator_free(allocator, still_stream)
            LibCamera.framebuffer_allocator_free(allocator, viewfinder_stream) if viewfinder_stream
          end
        end

        still_image = @still_image
        raise "Still image is not initialized" unless still_image

        return still_image, @viewfinder_image
      end

      def queue_request(request : Request)
        raise "Failed to queue request" if LibCamera.camera_queue_request(@camera, request.to_unsafe) < 0
      end

      def request
        request = @request
        return request if request

        @request = Request.new(@camera)
      end

      def start
        raise "Failed to start camera" if LibCamera.camera_start(@camera, nil) < 0
      end

      def streams_size
        return LibCamera.camera_streams_size(@camera)
      end
    end
  end
end

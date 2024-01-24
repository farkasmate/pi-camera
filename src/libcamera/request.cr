require "./bindings"

module PiCamera
  class Cam
    class Request
      def initialize(camera : LibCamera::Camera)
        @request = LibCamera.camera_create_request(camera, 0)

        at_exit { @request.try { |request| LibCamera.request_destroy(request) } }
      end

      def add_buffer(stream : LibCamera::Stream, buffer : LibCamera::FrameBuffer)
        LibCamera.request_add_buffer(@request, stream, buffer)
      end

      def cancelled?
        return Cam::LibCamera.request_status(@request) == LibCamera::RequestStatus::CANCELLED
      end

      def complete?
        return LibCamera.request_status(@request) == LibCamera::RequestStatus::COMPLETE
      end

      def focus
        return 0 if !complete?

        metadata = LibCamera.request_metadata(@request)
        value = LibCamera.control_list_get(metadata, LibCamera.control_focus_fo_m)

        return value.null? ? 0 : LibCamera.control_value_get(value).as(Pointer(Int32)).value
      end

      def reuse
        LibCamera.request_reuse(@request, LibCamera::ReuseFlag::REUSE_BUFFERS)
      end

      def to_unsafe
        return @request
      end
    end
  end
end

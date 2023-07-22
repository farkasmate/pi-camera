require "./bindings"
require "./camera"

module PiCamera
  class Cam
    class CameraManager
      def initialize
        @manager = LibCamera.camera_manager_create
        raise "Can't create camera manager" if LibCamera.camera_manager_start(@manager) < 0

        @cameras = LibCamera.camera_manager_cameras(@manager)
        raise "No camera found" if size < 1

        at_exit do
          @cameras.try { |cameras| LibCamera.camera_list_destroy(cameras) }
          LibCamera.camera_manager_stop(@manager)
          LibCamera.camera_manager_destroy(@manager)
        end
      end

      def size
        return LibCamera.camera_list_size(@cameras)
      end

      def [](index : Int)
        return Camera.new(@cameras, index)
      end
    end
  end
end

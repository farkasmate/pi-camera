require "./libcamera/camera"
require "./libcamera/camera_manager"

module PiCamera
  class Cam
    @camera : Camera
    @request : Request

    class StreamConfig
      getter four_cc, width, height

      def initialize(@four_cc : FourCC, @width : Int = 0, @height : Int = 0)
      end
    end

    def initialize(still_config : StreamConfig, viewfinder_config : StreamConfig? = nil, orientation : Orientation = Orientation::ROTATE0)
      manager = CameraManager.new

      @camera = manager[0]
      @still_image, @viewfinder_image = @camera.configure(still_config, viewfinder_config, orientation)
      @request = @camera.request
    end

    def capture(&request_callback : ImageData, ImageData?, Int32 -> Bool)
      @camera.start
      @camera.queue_request(@request)

      while true
        while !@request.complete?
          return if @request.cancelled?

          puts "Waiting for capture to finish..."
          sleep 100.milliseconds
        end
        puts "Calling user callback"

        return if request_callback.call(@still_image, @viewfinder_image, @request.focus)

        @request.reuse
        @camera.queue_request(@request)
      end
    end
  end
end

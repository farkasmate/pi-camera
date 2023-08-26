require "./libcamera/enums"
require "./libcamera/image_data"

module PiCamera
  class Sync
    def initialize(ui : Ui)
      Dir["./DCIM_test/pi_camera_*.yaml"].each do |file|
        Log.info { "Reading #{file}" }
        raw = File.read(file).to_slice
        image_data = ImageData.from_metadata_file(file)
        Log.info { "converting to JPEG..." }
        File.write("/tmp/#{File.basename(file, suffix: ".yaml")}.jpg", image_data.to_jpeg)
      end
    end
  end
end

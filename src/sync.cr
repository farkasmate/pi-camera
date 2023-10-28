require "./dcim"
require "./libcamera/enums"
require "./libcamera/image_data"

module PiCamera
  class Sync
    def initialize(ui : Ui)
      DCIM.each_image do |name, image_data|
        DCIM.save_jpeg(image_data)
        # TODO: Upload to Google Photos
        DCIM.archive(name)
      end
    end
  end
end

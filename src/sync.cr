require "./dcim"
require "./google_photos"
require "./libcamera/enums"
require "./libcamera/image_data"

module PiCamera
  class Sync
    def initialize(ui : Ui)
      DCIM.each_image do |name, image_data|
        jpeg_path = DCIM.save_jpeg(image_data)
        GooglePhotos.upload_to_album(jpeg_path)
        DCIM.archive(name)
      end
    end
  end
end

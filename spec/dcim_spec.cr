require "./spec_helper"
require "../src/dcim"

module PiCamera
  describe DCIM do
    it "converts to JPEG successfully" do
      DCIM.each_image do |name, image_data|
        DCIM.save_jpeg(image_data)
      end
    end

    it "archives" do
      File.write(Path.new(DCIM::DIRECTORY, "test.jpg"), "JPEG")
      File.write(Path.new(DCIM::DIRECTORY, "test.raw"), "RAW")
      File.write(Path.new(DCIM::DIRECTORY, "test.yaml"), "YAML")

      DCIM.archive("test")

      File.delete(Path.new(DCIM::DIRECTORY, "archive", "test.jpg"))
      File.delete(Path.new(DCIM::DIRECTORY, "archive", "test.raw"))
      File.delete(Path.new(DCIM::DIRECTORY, "archive", "test.yaml"))
    end
  end
end

require "log"

require "./libcamera/image_data"

module PiCamera
  module DCIM
    extend self

    DIRECTORY = "./DCIM_test" # FIXME
    PREFIX    = "pi_camera_"

    def archive(name : String)
      Dir.mkdir_p(File.join(DIRECTORY, "archive"))

      ["jpg", "raw", "yaml"].each do |extension|
        from = File.join(DIRECTORY, "#{name}.#{extension}")
        to = File.join(DIRECTORY, "archive", "#{name}.#{extension}")

        File.rename(from, to) if File.exists? from
      end
    end

    def each_image(&block : String, ImageData -> Void)
      Dir[File.join(DIRECTORY, "#{PREFIX}*.yaml")].each do |metadata_file|
        Log.debug { "Reading metadata #{metadata_file}" }
        name = File.basename(metadata_file, suffix: ".yaml")

        yield name, load_raw(name)
      end
    end

    def load_raw(name : String) : ImageData
      metadata_path = File.read(File.join(DIRECTORY, "#{name}.yaml"))
      image_data = ImageData.from_yaml(metadata_path)

      raw_path = File.join(DIRECTORY, image_data.raw_file)

      Log.debug { "Reading RAW #{raw_path}" }
      image_data.bytes = File.read(raw_path).to_slice

      return image_data
    end

    def save_jpeg(image_data : ImageData)
      name = image_data.raw_file.empty? ? "#{PREFIX}#{Time.utc.to_unix_ms}" : File.basename(image_data.raw_file, suffix: ".raw")
      jpeg_path = File.join(DIRECTORY, "#{name}.jpg")

      Log.debug { "Writing JPEG #{jpeg_path}" }
      File.write(jpeg_path, image_data.to_jpeg)
    end

    def save_raw(image_data : ImageData)
      name = "#{PREFIX}#{Time.utc.to_unix_ms}"

      image_data.raw_file = "#{name}.raw"
      metadata_path = File.join(DIRECTORY, "#{name}.yaml")
      raw_path = File.join(DIRECTORY, image_data.raw_file)

      Log.debug { "Writing RAW #{raw_path}" }
      File.write(metadata_path, image_data.to_yaml)
      File.write(raw_path, image_data.bytes)
    end
  end
end

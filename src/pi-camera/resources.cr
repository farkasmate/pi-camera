require "baked_file_system"

module Pi::Camera
  class Resources
    extend BakedFileSystem

    bake_folder "resources/"

    def self.get_frame(name : String) : Frame
      data = get(name).gets_to_end.split("\n", remove_empty: true)
      Frame.new(width: data[0].size, height: data.size) do |x, y|
        data[y][x] == '#' ? Frame::Color::Black : Frame::Color::White
      end
    end
  end
end

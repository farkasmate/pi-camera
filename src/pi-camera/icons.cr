require "./file_storage"
require "./frame"

module Pi::Camera::Icons
  module Icon
    extend self

    Camera = Icon.from_resource("camera.txt")

    def from_resource(resource_name : String) : Frame
      data = FileStorage.get(resource_name).gets_to_end.split("\n", remove_empty: true)
      Frame.new(width: data[0].size, height: data.size) do |x, y|
        data[y][x] == '#' ? Frame::Color::Black : Frame::Color::White
      end
    end
  end
end

module Pi::Camera
  class Frame
    include Icons
  end
end

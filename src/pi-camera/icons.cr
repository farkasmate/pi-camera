require "./frame"
require "./resources"

module Pi::Camera::Icons
  module Icon
    extend self

    Camera = [
      Icon.from_resource("icons/camera/frame_0.txt"),
      Icon.from_resource("icons/camera/frame_1.txt"),
      Icon.from_resource("icons/camera/frame_2.txt"),
      Icon.from_resource("icons/camera/frame_3.txt"),
    ]

    def from_resource(name : String) : Frame
      data = Resources.get(name).gets_to_end.split("\n", remove_empty: true)
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

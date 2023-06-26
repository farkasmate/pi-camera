require "./frame"
require "./resources"

module Pi::Camera
  module Icons
    extend self

    Camera = [
      Resources.get_frame("icons/camera/frame_0.txt"),
      Resources.get_frame("icons/camera/frame_1.txt"),
      Resources.get_frame("icons/camera/frame_2.txt"),
      Resources.get_frame("icons/camera/frame_3.txt"),
    ]

    Cloud = [
      Resources.get_frame("icons/cloud/frame_0.txt"),
      Resources.get_frame("icons/cloud/frame_1.txt"),
      Resources.get_frame("icons/cloud/frame_2.txt"),
      Resources.get_frame("icons/cloud/frame_3.txt"),
    ]

    Wrench = [
      Resources.get_frame("icons/wrench/frame_0.txt"),
      Resources.get_frame("icons/wrench/frame_1.txt"),
      Resources.get_frame("icons/wrench/frame_2.txt"),
      Resources.get_frame("icons/wrench/frame_3.txt"),
    ]
  end
end

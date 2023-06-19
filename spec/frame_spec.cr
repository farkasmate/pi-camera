require "./spec_helper"
require "../src/pi-camera/frame"

module Pi::Camera
  describe Frame do
    it "pretty prints" do
      frame = Frame.new(width: 4, height: 3)
      frame.fill(Frame::Color::Black)
      frame.set(0, 0, Frame::Color::White)
      frame.set(2, 1, Frame::Color::White)
      frame.to_s.should eq <<-FRAME
         ###
        ## #
        ####
        FRAME
    end

    it "scales correctly" do
      white_square = Frame.new(width: 2, height: 2)

      frame = Frame.new(width: 10, height: 7)
      frame.fill(Frame::Color::Black)
      frame.draw(white_square, x_offset: 1, y_offset: 2, scale: 2)
      frame.to_s.should eq <<-FRAME
        ##########
        ##########
        #    #####
        #    #####
        #    #####
        #    #####
        ##########
        FRAME
    end
  end
end

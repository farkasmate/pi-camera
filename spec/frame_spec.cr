require "./spec_helper"
require "../src/frame"

module PiCamera
  describe Frame do
    it "gets last pixel" do
      frame = Frame.new(width: 4, height: 3)
      frame.get(3, 2).should eq Frame::Color::White
    end

    it "fails on getting out of bounds pixel" do
      expect_raises(IndexError) do
        frame = Frame.new(width: 4, height: 3)
        frame.get(4, 3)
      end
    end

    it "sets last pixel" do
      frame = Frame.new(width: 4, height: 3)
      frame.set(3, 2, Frame::Color::Black)
      frame.get(3, 2).should eq Frame::Color::Black
    end

    it "fails on setting out of bounds pixel" do
      expect_raises(IndexError) do
        frame = Frame.new(width: 4, height: 3)
        frame.set(5, 6, Frame::Color::Black)
      end
    end

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

    it "clips correctly" do
      white_square = Frame.new(width: 2, height: 2)

      frame = Frame.new(width: 10, height: 5)
      frame.fill(Frame::Color::Black)
      frame.draw(white_square, x_offset: 1, y_offset: 2, scale: 2)
      frame.to_s.should eq <<-FRAME
        ##########
        ##########
        #    #####
        #    #####
        #    #####
        FRAME
    end

    it "shifts left correctly" do
      frame = Frame.new(width: 4, height: 3)
      frame.fill(Frame::Color::Black)
      frame.set(1, 0, Frame::Color::White) # ##
      frame.set(2, 1, Frame::Color::White) ## #
      frame.set(2, 2, Frame::Color::White) ## #
      frame.shift_left!
      frame.to_s.should eq <<-FRAME
         ###
        # ##
        # ##
        FRAME
    end

    it "shifts around correctly" do
      frame = Frame.new(width: 4, height: 3)
      frame.fill(Frame::Color::Black)
      frame.set(1, 0, Frame::Color::White) # ##
      frame.set(2, 1, Frame::Color::White) ## #
      frame.set(2, 2, Frame::Color::White) ## #
      frame.shift_left! frame.width
      frame.to_s.should eq <<-FRAME
        # ##
        ## #
        ## #
        FRAME
    end

    it "shifts up correctly" do
      frame = Frame.new(width: 4, height: 3)
      frame.fill(Frame::Color::Black)
      frame.set(1, 0, Frame::Color::White) # ##
      frame.set(2, 1, Frame::Color::White) ## #
      frame.set(2, 2, Frame::Color::White) ## #
      frame.shift_up!
      frame.to_s.should eq <<-FRAME
        ## #
        ## #
        # ##
        FRAME
    end

    it "shifts down correctly" do
      frame = Frame.new(width: 5, height: 5)
      frame.fill(Frame::Color::Black)
      frame.set(1, 0, Frame::Color::White) # ###
      frame.set(2, 1, Frame::Color::White) ## ##
      frame.set(2, 2, Frame::Color::White) ## ##
      frame.set(3, 3, Frame::Color::White) ### #
      frame.set(3, 4, Frame::Color::White) ### #
      frame.shift_down!
      frame.to_s.should eq <<-FRAME
        ### #
        # ###
        ## ##
        ## ##
        ### #
        FRAME
    end
  end
end

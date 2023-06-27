require "./spec_helper"
require "../src/pi-camera/fonts"

module Pi::Camera
  describe Fonts::Terminus do
    it "loads font file correctly" do
      frame = Fonts::Terminus.text "Hi"
      frame.to_s.should eq <<-FRAME
                        
                        
         #    #    #    
         #    #    #    
         #    #         
         #    #   ##    
         ######    #    
         #    #    #    
         #    #    #    
         #    #    #    
         #    #    #    
         #    #   ###   
                        
                        
                        
                        
        FRAME
    end
  end
end
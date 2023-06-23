require "./spec_helper"
require "../src/pi-camera/fonts"

module Pi::Camera::Fonts
  describe Font::Terminus do
    it "loads font file correctly" do
      frame = Font::Terminus.text "Hi"
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

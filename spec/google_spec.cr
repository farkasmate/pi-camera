# TODO: DELETE ME
require "./spec_helper"
require "../src/google_photos"

module PiCamera
  describe GooglePhotos do
    it "authenticates" do
      p! GooglePhotos.list
      # GooglePhotos.create_album "crystal_test"
    end
  end
end

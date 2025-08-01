require "json"
require "kemal"
require "log"
require "oauth2"

require "./google_auth"

module PiCamera
  module GooglePhotos
    extend self

    def auth : OAuth2::Session
      oauth2_client = OAuth2::Client.from_json(File.read("client_secret.json"))

      if File.exists? "token.json"
        access_token = OAuth2::AccessToken::Bearer.from_json(File.read("token.json"))
      else
        authorize_uri = oauth2_client.get_authorize_uri(
          scope: "https://www.googleapis.com/auth/photoslibrary",
          state: Base64.encode(Random::Secure.random_bytes(16)),
        )

        puts "Visit to authenticate:"
        puts "  http://localhost:8080/auth"

        authorization_code = redirect_user(authorize_uri)
        access_token = oauth2_client.get_access_token_using_authorization_code(authorization_code)
      end

      OAuth2::Session.new(oauth2_client: oauth2_client, access_token: access_token) do |session|
        File.write("token.json", session.access_token.to_json)
      end
    end

    def client : HTTP::Client
      photos_client = HTTP::Client.new("photoslibrary.googleapis.com", tls: true)

      session = auth()

      begin
        session.authenticate(photos_client)
      rescue exception : OAuth2::Error
        File.delete("token.json")
        return client
      end

      photos_client
    end

    def list
      data = client.get("/v1/albums").body
      albums = JSON.parse(data)["albums"].as_a.map { |album| album["title"] }

      albums.each { |album| puts album }
    end

    # TODO: Batch create?
    def upload_to_album(path : Path)
      bytes = File.open(path) do |file|
        file.getb_to_end
      end

      data = client.post(
        path: "/v1/mediaItems:batchCreate",
        headers: HTTP::Headers{
          "Content-type" => "application/json",
        },
        body: {
          albumId:       "ADbj3g-4oJPnY9nnKRMK4lZYQdYlZjok-m23viQ3Vj6YTsbj5QJBQb063GyQpkZhKgfzFaCLcQRc", # FIXME: Create album
          newMediaItems: [
            {
              simpleMediaItem: {
                fileName:    path.basename,
                uploadToken: upload_jpeg(bytes),
              },
            },
          ],
        }.to_json,
      )

      p! data
    end

    private def create_album(name : String)
      data = client.post(
        path: "/v1/albums",
        headers: HTTP::Headers{
          "Content-type" => "application/json",
        },
        body: {
          album: {
            title: name,
          },
        }.to_json,
      )

      p! data
    end

    private def upload_jpeg(bytes : Bytes)
      client.post(
        path: "/v1/uploads", # TODO: url?
        headers: HTTP::Headers{
        "Content-type"               => "application/octet-stream",
        "X-Goog-Upload-Content-Type" => "image/jpeg",
        "X-Goog-Upload-Protocol"     => "raw",
      },
        body: bytes,
      ).body
    end

    private def redirect_user(uri : String) : String
      state = URI.parse(uri).query_params["state"]
      code = ""

      get "/auth" do |env|
        env.redirect uri
      end

      get "/callback" do |env|
        halt env, status_code: 400 if state != env.params.query["state"]

        code = env.params.query["code"]
        "Successfully logged in"
      end

      after_get "/callback" do
        Kemal.stop
      end

      Kemal.config.port = 8080
      Kemal.run

      code
    end
  end
end

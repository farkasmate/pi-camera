require "json"
require "oauth2"

module OAuth2
  class GoogleClientSecret
    include JSON::Serializable

    class GoogleClientSecretDetail
      include JSON::Serializable

      getter client_id : String
      getter project_id : String
      getter auth_uri : String
      getter token_uri : String
      getter auth_provider_x509_cert_url : String
      getter client_secret : String
      getter redirect_uris : Array(String)
    end

    private getter installed : GoogleClientSecretDetail

    {% for method in GoogleClientSecretDetail.methods %}
      delegate {{ method.name.id }}, to: installed
    {% end %}
  end

  class Client
    def self.from_json(json : String) : OAuth2::Client
      config = GoogleClientSecret.from_json(json)

      OAuth2::Client.new(
        host: "",
        authorize_uri: config.auth_uri,
        token_uri: config.token_uri,
        client_id: config.client_id,
        client_secret: config.client_secret,
        redirect_uri: config.redirect_uris[0],
      )
    end
  end
end

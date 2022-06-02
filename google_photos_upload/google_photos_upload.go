package main

import (
    "context"
    "io/ioutil"
    "golang.org/x/oauth2"
    "golang.org/x/oauth2/google"
    "fmt"
    "log"
    "net/http"
    "encoding/json"
    "encoding/base64"
    "crypto/rand"

    gphotos "github.com/gphotosuploader/google-photos-api-client-go/v2"
)

import "C"

// Required for '-buildmode=c-archive'
func main() {}

func randomString(n int) string {
    b := make([]byte, n)
    _, err := rand.Read(b)
    if err != nil {
        log.Fatal("31 ", err)
    }

    s := base64.StdEncoding.EncodeToString(b)
    return string(s[:n])
}

func config() *oauth2.Config {
    data, err := ioutil.ReadFile("./client_secret.json")
    if err != nil {
        log.Fatal("12 ", err)
    }
    conf, err := google.ConfigFromJSON(data, gphotos.PhotoslibraryScope)
    if err != nil {
        log.Fatal("13 ", err)
    }

    return conf
}

//export authenticate
func authenticate() {
    conf := config()
    token := auth(*conf)

    log.Print(token)

    j, err := json.Marshal(token)
    if err != nil {
        log.Fatal(err)
    }
    fmt.Println(string(j))

    err = ioutil.WriteFile("./test_token.json", j, 0644)
    if err != nil {
        log.Fatal(err)
    }
}

func auth(conf oauth2.Config) *oauth2.Token {
    ctx := context.Background()

    conf.RedirectURL = "http://localhost:8080" // FIXME
    csrfToken := randomString(16)
    url := conf.AuthCodeURL(csrfToken)

    fmt.Printf("Visit the URL for the auth dialog:\n%v\n", url)

    server := http.Server{Addr: ":8080", Handler: nil}
    code := ""
    http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
        if r.FormValue("state") == csrfToken {
            code = r.FormValue("code")
        }

        server.Shutdown(ctx)
    })

    err := server.ListenAndServe()
    if err != http.ErrServerClosed {
        log.Fatal(err)
    }

    token, err := conf.Exchange(ctx, code)
    if err != nil {
        log.Fatal("11 ", err)
    }

    return token
}

//export upload_test_image
func upload_test_image(album_name *C.char, image_path *C.char) {
    UploadTestImage(C.GoString(album_name), C.GoString(image_path))
}

func UploadTestImage(albumName string, imagePath string) {
    ctx := context.Background()

////

    data, err := ioutil.ReadFile("./test_token.json")
    if err != nil {
        log.Fatal("12 ", err)
    }
    var token *oauth2.Token
    err = json.Unmarshal(data, &token)
    if err != nil {
        log.Fatal(err)
    }

////

    conf := config()
    tc := conf.Client(ctx, token)
    client, err := gphotos.NewClient(tc)
    if err != nil {
        log.Fatal("4 ", err)
    }

    log.Print("uploading file...")

    album, err := client.Albums.GetByTitle(ctx, albumName)
    //media, err :=  client.UploadFileToLibrary(ctx, "/go/src/test.jpg")
    media, err :=  client.UploadFileToAlbum(ctx, album.ID, imagePath)
    if err != nil {
        log.Fatal("5 ", err)
    }

    log.Print("DONE")

    log.Print("URL: ", media.ProductURL)
}

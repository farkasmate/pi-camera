package main

import (
	"context"
	"crypto/rand"
	"encoding/base64"
	"encoding/json"
	"errors"
	"fmt"
	"golang.org/x/oauth2"
	"golang.org/x/oauth2/google"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"path/filepath"

	gphotos "github.com/gphotosuploader/google-photos-api-client-go/v2"
)

// FIXME
var configFile string
var tokenFile string

func config(path string) *oauth2.Config {
	if _, err := os.Stat(path); err != nil && errors.Is(err, os.ErrNotExist) {
		log.Fatal(path, " not found. Follow instructions and download credentials in JSON format: https://gphotosuploader.github.io/gphotos-uploader-cli/#/configuration?id=apiappcredentials")
	}

	data, err := ioutil.ReadFile(path)
	if err != nil {
		log.Fatal(err)
	}
	conf, err := google.ConfigFromJSON(data, gphotos.PhotoslibraryScope)
	if err != nil {
		log.Fatal(err)
	}

	return conf
}

func googleAuth(conf oauth2.Config) *oauth2.Token {
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
		log.Fatal(err)
	}

	return token
}

func randomString(n int) string {
	b := make([]byte, n)
	_, err := rand.Read(b)
	if err != nil {
		log.Fatal(err)
	}

	s := base64.StdEncoding.EncodeToString(b)
	return string(s[:n])
}

func readToken(path string) *oauth2.Token {
	if _, err := os.Stat(path); err != nil && errors.Is(err, os.ErrNotExist) {
		log.Fatal(path, " not found. Pass `--auth` to generate a new token.")
	}

	data, err := ioutil.ReadFile(path)
	if err != nil {
		log.Fatal(err)
	}
	var token *oauth2.Token
	err = json.Unmarshal(data, &token)
	if err != nil {
		log.Fatal(err)
	}

	return token
}

func writeToken(path string, token *oauth2.Token) {
	j, err := json.Marshal(token)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(string(j))

	err = ioutil.WriteFile(path, j, 0644)
	if err != nil {
		log.Fatal(err)
	}
}

func Authenticate() {
	conf := config(configFile)
	token := googleAuth(*conf)

	writeToken(tokenFile, token)
}

func UploadImage(albumName string, imagePath string) {
	ctx := context.Background()

	token := readToken(tokenFile)

	conf := config(configFile)
	tc := conf.Client(ctx, token)
	client, err := gphotos.NewClient(tc)
	if err != nil {
		log.Fatal(err)
	}

	log.Print("uploading file...")

	album, err := client.Albums.GetByTitle(ctx, albumName)
	//media, err :=  client.UploadFileToLibrary(ctx, "/go/src/test.jpg")
	media, err := client.UploadFileToAlbum(ctx, album.ID, imagePath)
	if err != nil {
		log.Fatal(err)
	}

	log.Print("DONE")

	log.Print("URL: ", media.ProductURL)
}

func SetConfig(configDir string) {
	configFile = filepath.Join(configDir, "client_secret.json")
	tokenFile = filepath.Join(configDir, "token.json")
}
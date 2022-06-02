all: google_photos_upload.a

google_photos_upload.a:
	go build \
	  -buildmode=c-archive \
	  -o google_photos_upload.a \
	  google_photos_upload.go

clean:
	rm -f google_photos_upload.a google_photos_upload.h

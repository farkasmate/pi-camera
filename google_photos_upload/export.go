package main

import "C"

//export authenticate
func authenticate() {
	Authenticate()
}

//export upload_test_image
func upload_test_image(album_name *C.char, image_path *C.char) {
	UploadTestImage(C.GoString(album_name), C.GoString(image_path))
}

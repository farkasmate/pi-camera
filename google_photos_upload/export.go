package main

import "C"

//export google_photos_authenticate
func google_photos_authenticate() {
	Authenticate()
}

//export google_photos_set_config
func google_photos_set_config(config_dir *C.char) {
	SetConfig(C.GoString(config_dir))
}

//export google_photos_upload_image
func google_photos_upload_image(album_name *C.char, image_path *C.char) {
	UploadImage(C.GoString(album_name), C.GoString(image_path))
}

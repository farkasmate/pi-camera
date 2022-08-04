package main

import "C"

//export google_photos_get_auth_url
func google_photos_get_auth_url() *C.char {
	// NOTE: free() needs to be called on CString
	return C.CString(GetAuthUrl())
}

//export google_photos_set_config
func google_photos_set_config(config_dir *C.char) {
	SetConfig(C.GoString(config_dir))
}

//export google_photos_upload_image
func google_photos_upload_image(album_name *C.char, image_path *C.char) {
	UploadImage(C.GoString(album_name), C.GoString(image_path))
}

//export google_photos_wait_for_token
func google_photos_wait_for_token(auth_url *C.char) {
	WaitForToken(C.GoString(auth_url))
}

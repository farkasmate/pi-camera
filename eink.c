#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "DEV_Config.h"
#include "EPD_2in13_V2.h"
#include "GUI_BMPfile.h"
#include "GUI_Paint.h"

void Handler(int signo) {
    DEV_Module_Exit();
    exit(0);
}

int main(int argc, char *argv[]) {
    struct stat stat_buffer;
    UBYTE *Image;
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;

    signal(SIGINT, Handler);

    if(argc != 2) {
        printf("USAGE: %s <image_path>\n", argv[0]);
        return 1;
    }

    if(stat(argv[1], &stat_buffer) == -1) {
        printf("File not found: %s\n", argv[1]);
        return 2;
    }

    if((Image = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    Paint_NewImage(Image, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    Paint_SelectImage(Image);
    Paint_SetMirroring(MIRROR_VERTICAL);
    GUI_ReadBmp(argv[1], 0, 0);

    if(DEV_Module_Init() != 0){ return -1; }

    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Display(Image);

    DEV_Module_Exit();

    free(Image);
    Image = NULL;

    return 0;
}

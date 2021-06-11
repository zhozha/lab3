#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(push, 2)
struct BITMAPHEADER {
    unsigned char bfType[2];
    unsigned char bfSize[4];
    unsigned char bfReserved1[2];
    unsigned char bfReserved2[2];
    unsigned char bfOffsetBits[4];
    
    unsigned char biSize[4];
    unsigned char biWidth[4];
    unsigned char biHeight[4];
    unsigned char biPlanes[2];
    unsigned char biBitCount[2];
    unsigned char biCompression[4];
    unsigned char biSizeImage[4];
    unsigned char biXpelsPerMeter[4];
    unsigned char biYpelsPerMeter[4];
    unsigned char biClrUsed[4];
    unsigned char biClrImportant[4];
 
} bmpHeader;
#pragma pack(pop)

struct PIXEL {
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
};

class BitmapFile {
    BITMAPHEADER head;
    PIXEL* img;
    int width;
    int height;
    int size;
    
    public:
        void bmp_read(char *filename);
        void bmp_write(char *filename);
        void bmp_invert_horizontal();
        
        void bmp_increase_Y(int arg);
};

void BitmapFile::bmp_read(char *filename) {
    unsigned int i;
    FILE* file;
    file = fopen(filename, "rb");
    
    fread(&bmpHeader, sizeof(bmpHeader), 1, file);
    
    width = *(int*)bmpHeader.biWidth;
    height = *(int*)bmpHeader.biHeight;
    size = width * height;
    
    img = new PIXEL[size];
    for(i = 0; i < size; i++)
        fread(&img[i], sizeof(PIXEL), 1, file);
        
    fclose(file);
}

void BitmapFile::bmp_write(char *filename) {
    unsigned int i;
    FILE* file;
    file = fopen(filename, "wb");
    
    fwrite(&bmpHeader, sizeof(bmpHeader), 1, file);
    for(i = 0; i < size; i++)
        fwrite(&img[i], sizeof(PIXEL), 1, file);
    
    fclose(file);
}

void BitmapFile::bmp_invert_horizontal() {
    unsigned int i, j;
    FILE* file;
    PIXEL temp;
    for(i = 0; i < height / 2; i++)
        for(j = 0; j < width; j++)
        {
            temp = img[width * i + j];
            img[width * i + j] = img[size - width * (i + 1) + j];
            img[size - width * (i + 1) + j] = temp;
        }                  
}

int main(int argc, char **argv)
{
    int i;
    int arg;
    BitmapFile picture;
    
    if (argc != 3) {
		printf("Usage: .exe <picture.bmp> <modifier>\n");
		printf("Modifiers:\n");
		printf("h - inverts picture horizontally\n");
		printf("<int> - modifies brightness by given value\n");
		return -1;
	}
    
    picture.bmp_read(argv[1]);
    
	if (argv[2][0] == 'h') {
	    picture.bmp_invert_horizontal();
    }
   
    
    picture.bmp_write(argv[1]);
  

    return 0;
}

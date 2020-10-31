// Loads general libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Loads image libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Start Program
int main(void) {

	// Define variables
	int width, height, channels;
	


	// Load Image and its information to memory
	unsigned char *img = stbi_load("Lena_large.jpg",&width,&height,&channels,0);
	
	// Check if the image was loaded correctly
	if(img == NULL) {
		printf("Error in loading the image\n");
		exit(1);
	}

	// Prints image information
	// printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
	
	// Calculates the size of the image
	size_t img_size = width * height * channels;
	
	// Defines the number of channels for the gray image
	int gray_channels = channels == 4 ? 2 : 1;  // if(channels == 4)
	                                            // then gray_channels = 2
												// else gray_channels = 1;
	// para JPG podria quedar: int gray_channels = 1;
	// para PNG podria quedar: int gray_channels = 2;
	
	// Calculates the size of the gray image
	size_t gray_img_size = width * height * gray_channels;
	


	// Reserves Memory for the gray image
	unsigned char *gray_img = malloc(gray_img_size);
	
	// Check if the memory for the gray image was allocated correctly
	if(gray_img == NULL) {
		printf("Unable to allocate memory for the gray image.\n");
		exit(1);
	}



	// It starts reading at address 'img' and 'p' increases by '3' since the input image has 3 channels
	// It starts writting at address 'gray_img' and 'pg' increases by '1' since the output image has 1 channel
    // the LOOP finished at address 'img + img_size'
	unsigned char *p;
	unsigned char *pg;
	for(p = img, pg = gray_img; p != img + img_size; p +=channels, pg += gray_channels)
	{
		// Gray convesion Process: Gray_Image = Red*0.21 + Green*0.72 + Blue*0.07
		*pg = (uint8_t)(((*p)*0.21 + (*(p+1))*0.72 + (*(p+2))*0.07));
	}
	
	// write in a file the resulting image
	stbi_write_jpg("Lena_large_gray.jpg",width,height,gray_channels,gray_img,100);




    // define Prewitt kernells for the convolution
	int kernellV[3][3] = {{1,0,-1},{1,0,-1},{1,0,-1}};    // Vertical Kernell
	int kernellH[3][3] = {{1,1,1},{0,0,0},{-1,-1,-1}};    // Horizontal Kernell

	
	
	// Reserves Memory for the edge image
	unsigned char *edge_img = malloc(gray_img_size);
	
	// Check if the memory for the edge image was allocated correctly
	if(edge_img == NULL) {
		printf("Unable to allocate memory for the edge image.\n");
		exit(1);
	}

	// Copy the gray gray to the edge image
	unsigned char *pe, *pi;
	for(pe=edge_img, pi=gray_img; pi != gray_img + gray_img_size; pe++, pi++){
		*pe=(uint8_t)(*pi);	
	}
	
	
	
	// Perform the convolution with Horizontal Kernell and
	// Perform the convolution with Vertical Kernell then add them up
	for(pe=edge_img+1+width; pe != edge_img+(height*width-width-1); pe++){
			*pe = (*(pe-1-width)*kernellH[0][0] + *(pe-width)*kernellH[0][1] + *(pe+1-width)*kernellH[0][2]
			           + *(pe-1)*kernellH[1][0] +       *(pe)*kernellH[1][1] +       *(pe+1)*kernellH[1][2]
			     + *(pe-1+width)*kernellH[2][0] + *(pe+width)*kernellH[2][1] + *(pe+1+width)*kernellH[2][2])
		        + (*(pe-1-width)*kernellV[0][0] + *(pe-width)*kernellV[0][1] + *(pe+1-width)*kernellV[0][2]
			           + *(pe-1)*kernellV[1][0] +       *(pe)*kernellV[1][1] +       *(pe+1)*kernellV[1][2]
			     + *(pe-1+width)*kernellV[2][0] + *(pe+width)*kernellV[2][1] + *(pe+1+width)*kernellV[2][2]);
		// This threshold cleans the image and leaves the brightest pixels like edges
		if(*pe<190){
			*pe=0;
		}else{
			*pe=255;
		}
	}

	// write in a file the resulting image
	stbi_write_jpg("Lena_large_edge.jpg",width,height,gray_channels,edge_img,100);
	
	
	
	// release image memory
	stbi_image_free(img);

	// release gray image memory
	free(gray_img);
	
	// End Program
	return 0;
}

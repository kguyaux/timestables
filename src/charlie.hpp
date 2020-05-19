#ifndef __CHARLIE_HPP__
#define __CHARLIE_HPP__

#include <iostream>
#include <bitset>

#include "debug.hpp"
#include "stb_image.h"

// Charlie is a class to load & convert a font-bitmap to an
// array that holds all the pixeldata of all the fonts in the bitmap
// it is not very polished, but works for the single purpose of loading 
// the msx-fonts for the png-file.
class Charlie {
    private:
        int FMAP_stride = 0;

    public:
        int FMAP_numchars = 0; 
        char* FMAP = NULL;
        int fwidth = 0;
        int fheight = 0;

        // contructor:
        Charlie(const char* fontfilepath, int fw, int fh)
        {
            // image must be 8bit RGB[A]
            int scale = 2;
            fwidth = fw;
            fheight = fh;
            int width, height, nrChannels;
            unsigned char *imgdata = stbi_load(fontfilepath, &width, &height, &nrChannels, 0);
            if (imgdata) {
#ifdef TT_DEBUG
                std::cout << "Loaded chactermap of size: " << width << " x " << height << " x " << nrChannels << std::endl;
#endif
                int xnum = (width / scale) / fwidth;
                int ynum = (height / scale) / fheight;

                // setup FMAP-storage
                FMAP_numchars = xnum * ynum;
                FMAP_stride = 1 * fheight; // allocation in map per character
                FMAP = (char*)calloc(FMAP_numchars * FMAP_stride, sizeof(char));

                for (int j = 0; j < ynum; j++) {
                    for (int i = 0; i < xnum; i++) {
                        int FMAP_charindex = i + (j*xnum);
                        size_t img_offset = ((i * fwidth) + (j * width * fheight)) * nrChannels * scale;
                        for (int y=0; y < fheight; y++) {
                            unsigned char byte = 0;
                            for (int x=0; x < fwidth; x++) {
                                size_t datapos = img_offset + (x + y * width) * nrChannels * scale;
                                unsigned char r = imgdata[datapos];
                                unsigned char g = imgdata[datapos + 1];
                                unsigned char b = imgdata[datapos + 2];
                                // ..and ignore Alpha, if present..

                                int bw = (r + g + b) / 3;
                                if (bw < 128)
                                    byte = byte | 0x01;
                                if (x<7)
                                    byte = byte << 1;
                            }
                            FMAP[FMAP_charindex * FMAP_stride + y] = byte;
                        }
                    }
                }
#ifdef TT_DEBUG
                std::cout << "Loaded: " << FMAP_numchars << "characters." << std::endl;
#endif
            } else {
                log_err("Could not load font!");
            }
            stbi_image_free(imgdata);
        }

        // get the pointer
        size_t get_fontptr(int fontcharindex)
        {
            size_t pos = fontcharindex * FMAP_stride;
            return pos;
        }
    
        // for testing
        void test()
        {
            int charindex = 33;
            for (int i=0; i < fheight; i++) {
               char byte = FMAP[charindex * FMAP_stride + i]; 
               std::bitset<8> x(byte);
            }
        }

        void destroy()
        {
            free(FMAP);
        }

        // Charlies destructor
        ~Charlie() {};
};

#endif

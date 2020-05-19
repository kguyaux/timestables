#include "charlie.hpp"
#include "text.hpp"

Text::Text()
{
    charlie = new Charlie((const char*)"resources/msxfont_bitmap.png", 8, 8);
    for (int i=0; i < charlie->FMAP_numchars; i++) {
        std::vector<glm::vec3> tBlox = getTextBloxels(i);
        charBloxels.push_back(tBlox);
        charSprites.push_back(Sprite(tBlox));
    }
}


// return a list of char indexes.. map MSX charcodes to ascii
std::vector<int> Text::getStringBloxels(std::string &str)
{
    std::vector<int> ret;
    for(char& c : str) {
        ret.push_back(charmapper(c));
    }
    return ret;
}


// map MSX to char
int Text::charmapper(char val)
{
    int ret = 0;
    if (val >= 32) ret = val-32;  
    return ret;
}


//returns the positions of the textbloxels
std::vector<glm::vec3> Text::getTextBloxels(int charindex)
{
    size_t fontptr = charlie->get_fontptr(charindex);
    std::vector<glm::vec3> bloxels;
    for (int y=0; y < charlie->fheight; y++) {
        unsigned char byte = charlie->FMAP[fontptr + y];
        for (int x=0; x < charlie->fwidth; x++) {
            int bit = (byte >> x) & 0x01;
            if (bit == 1) {
                int vx = x * scale;
                int vy = y * scale;
                bloxels.push_back(glm::vec3(float(vx), float(vy), 0.0f));
            }
        }
    }
    return bloxels;
}


Text::~Text()
{
    delete charlie;
}

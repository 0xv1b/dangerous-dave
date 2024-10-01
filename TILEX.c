/* Extract tileset from Dangerous Dave binary outputting each tile as a bitmap*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
    const uint32_t vga_data_addr = 0x120f0;
    const uint32_t vga_pal_addr = 0x26b0a;

    /*Open EXE and go to VGA pixel data*/
    FILE* fin;
    fin = fopen("./oassets/DAVE.EXE", "rb");
    fseek(fin, vga_data_addr, SEEK_SET);

    /*Decode run length encoding*/
    /*Read decoded length - first dword (4 bytes) (little endian)*/
    uint32_t final_length;
    final_length = 0;

    final_length |= fgetc(fin); // characters are 8 bits
    final_length |= fgetc(fin) << 8;
    final_length |= fgetc(fin) << 16;
    final_length |= fgetc(fin) << 24;

    /*Read each bite and decode*/
    uint32_t current_length;
    unsigned char out_data[150000];
    uint8_t byte_buffer;

    current_length = 0;
    memset(&out_data, 0, sizeof(out_data));

    while (current_length < final_length)
    {
        byte_buffer = fgetc(fin);

        if (byte_buffer & 0x80)
        {
            byte_buffer &= 0x7F;
            byte_buffer++;
            while(byte_buffer)
            {
                out_data[current_length++] = fgetc(fin);
                byte_buffer--;
            }
        }
        else
        {
            byte_buffer += 3;
            char next = fgetc(fin);
            while(byte_buffer)
            {
                out_data[current_length++] = next;
                byte_buffer--;
            }
        }
    }

    /*Read in VGA palette*/
    fseek(fin, vga_pal_addr, SEEK_SET);
    uint8_t palette[768]; // 3 byte RGB value with 256 possible colours
    uint32_t i,j;
    for (i=0; i < 256; i++)
    {
        for(j=0; j<3;j++)
        {
            palette[i*3 + j] = fgetc(fin);
            palette[i*3 + j] <<= 2; // 6 bit colours must be left shifted
        }
    }

    fclose(fin);

    /*Total tile count*/
    uint32_t tile_count;
    tile_count = 0;
    tile_count |= out_data[0];
    tile_count |= out_data[1] << 8;
    tile_count |= out_data[2] << 16;
    tile_count |= out_data[3] << 24;

    /*Offset index*/

    return 0;
}
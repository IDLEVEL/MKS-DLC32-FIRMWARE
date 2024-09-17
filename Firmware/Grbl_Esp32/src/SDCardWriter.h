#pragma once
#include "Grbl.h"

struct SDCardWriter
{
    File file;
    bool is_writing = false;
    int32_t lines_writed = 0;
    uint32_t start_write_time = 0;
    const char* file_name = "/TEMP.GCODE";

    uint8_t buffer[512];
    uint16_t buffer_pos = 0;

        
    void flush_sd_buf()
    {
        sd_card_writer.file.write(sd_card_writer.buffer, sd_card_writer.buffer_pos);

        sd_card_writer.buffer_pos = 0;
    }

    void process_char(char ch)
    {
        uint8_t m29;

        if(is_writing)
        {
            if(ch == 'M')
            {
                m29++;
            }
            else if(m29 == 1 && ch == '2')
            {
                m29++;
            }
            else if(m29 == 2 && ch == '9')
            {
                finish();
            }
        }
    }

    Error process_line(char* line, uint8_t client)
    {
        if(strcmp(line, "@END") == 0)
        {
            if(!sd_card_writer.is_writing)
                return Error::FsFailedOpenFile;

            flush_sd_buf();
            
            sd_card_writer.file.close();
            sd_card_writer.is_writing = false;

            uint32_t total_time = ((millis() - sd_card_writer.start_write_time) / 1000);

            grbl_sendf(CLIENT_ALL, "%s END LINES: %d SEC: %d \n", sd_card_writer.file_name, sd_card_writer.lines_writed, (int)total_time);

            return Error::Ok;
        }
        else if(sd_card_writer.is_writing)
        {
            uint16_t str_len = strlen(line);

            uint16_t bytes_to_write = str_len + 1;

            if(bytes_to_write + sd_card_writer.buffer_pos >= sizeof(sd_card_writer.buffer))
                flush_sd_buf();

            memcpy(sd_card_writer.buffer + sd_card_writer.buffer_pos, line, str_len);

            sd_card_writer.buffer_pos += str_len;

            sd_card_writer.buffer[sd_card_writer.buffer_pos++] = '\0';

            sd_card_writer.lines_writed++;

            return Error::Ok;
        }
        else if(strcmp(line, "@BEGIN") == 0)
        {
            sd_card_writer.file = SD.open(sd_card_writer.file_name, "w");

            if(!sd_card_writer.file)
                return Error::FsFailedOpenFile;

            sd_card_writer.is_writing = true;
            sd_card_writer.lines_writed = 0;

            sd_card_writer.start_write_time = millis();
            
            grbl_sendf(CLIENT_ALL, "%s BEGIN\n", sd_card_writer.file_name);

            return Error::Ok;
        }
    }
    

} sd_card_writer;
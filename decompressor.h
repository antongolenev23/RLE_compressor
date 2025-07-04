#pragma once

#include <string>
#include <fstream>

inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
    using namespace std;

    ifstream in(src_name, ios::binary);
    if(!in) return false;

    ofstream out(dst_name, ios::binary);

    while(true) {
        int header_byte = in.get();
        if(header_byte == EOF) break;

        unsigned char header = static_cast<unsigned char>(header_byte);
        int block_type = (header & 1);
        int data_size = (header >> 1) + 1;

        if(block_type == 0) {
            char buffer[128];

            while(data_size > 0) {
                int to_read = std::min(data_size, static_cast<int>(sizeof(buffer)));
                in.read(buffer, to_read);
                int read_count = static_cast<int>(in.gcount());
                if(to_read != read_count) break;
                out.write(buffer, read_count);
                data_size -= read_count;
            }
        } else {
            int symbol = in.get();
            if(symbol == EOF) break;
            char c = static_cast<char>(symbol);
            std::string repeated(data_size, c);
            out.write(repeated.data(), repeated.size());
        }

    }

    return true;
}

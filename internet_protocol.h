#ifndef INTERNET_PROTOCOL_H
#define INTERNET_PROTOCOL_H
#include <string>

struct InternetSession {
    std::string start_time;
    std::string end_time;
    long long received_bytes;
    long long sent_bytes;
    std::string program_path;
};
#endif


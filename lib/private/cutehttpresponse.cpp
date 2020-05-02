#include "cutehttpresponse.h"

CuteHttpResponse::CuteHttpResponse()
{
    m_data = "HTTP/1.1 404 Not Found\r\n" \
            "Content-Length: 0\r\n" \
            "\r\n";
}

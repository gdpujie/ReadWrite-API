#include "reader.h"

Reader::Reader(): m_frame_buffer(NULL) {
}
void Reader::setFrameBuffer(Buffer<QByteArray> *buffer) {
    m_frame_buffer = buffer;
}

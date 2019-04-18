#ifndef READER_H
#define READER_H
#include <QThread>
#include <QObject>
#include "buffer.h"
class Reader: public QThread {
public:
    Reader();
    void setFrameBuffer(Buffer<QByteArray> *buffer);

    //private:
    Buffer<QByteArray> *m_frame_buffer;
};

#endif // READER_H

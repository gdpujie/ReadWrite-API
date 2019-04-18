#include "fread.h"
#include <windows.h>
#include <stdio.h>
#include <windef.h>
//#include "Config_All.h"
#define JUMP_CNT 10
FileReader::FileReader():m_led_win(NULL),m_paused(false)

{

}

FileReader::~FileReader() {
}

int FileReader::get_fid() {
    return m_frame_id;
}
/*
void FileReader::setLedWin(LedWin *led_win) {
    if (m_led_win) {
        disconnect(m_led_win, SIGNAL(jumpTo(int)), this, SLOT(jumpTo(int)));
    }

    m_led_win = led_win;

    if (m_led_win) {
        connect(m_led_win, SIGNAL(jumpTo(int)), this, SLOT(jumpTo(int)));
    }
}
*/

void FileReader::openFile(QString fileName) {
    qDebug("\nopening file %s", fileName.toLatin1().data());

    if (m_file.isOpen()) {
        this->stop();
        m_file.close();
    }

    m_file.setFileName(fileName);

    if (!m_file.exists()) {
        qDebug("file not exists");
        return ;
    }

    m_file.open(QIODevice::ReadOnly);
    //set slider
    if(fileName.endsWith(".txt")) {
        ;
    } else if (fileName.endsWith(".fd")) {
//        m_led_win->setSliderRange(0, m_file.size() / USB_PACKAGE_SAVE_SIZE - 1);
        this->m_frame_id = 0;
    }
}

void FileReader::run() {
    m_paused = false;

    while (true) {
        m_mutex.lock();

        //read
        if (!m_file.isOpen()) {
            m_mutex.unlock();
            sleep(1);
            continue;
        }

        push(m_frame_id);

        if (m_frame_id == -1) {
            m_frame_id += 1;
            m_mutex.unlock();
            return;
        }

        m_frame_id += 1;
        m_mutex.unlock();
    }
}

void FileReader::play() {
    if (!this->isRunning()) {
        qDebug("start");

        //this->f_id = 0;
        if (m_paused) {
            m_mutex.unlock();
            m_paused = false;
        }

        this->start();
    }
}

void FileReader::stop() {
    if (m_paused) {
        m_mutex.unlock();
        m_paused = false;
    }

    if (this->isRunning()) {
        m_mutex.lock();
        this->terminate();
        m_mutex.unlock();
        qDebug("terminate");
    }

    this->m_frame_id = 0;
}
void FileReader::push(int i) {
    static int last_id = -1;

    if (i == last_id) {
        return;

    } else {
        last_id = i;
    }

    if (!m_frame_buffer) {
        return;
    }

    if (!m_file.isOpen()) {
        return;
    }

    if (i < 0) {
        m_frame_id = 0;
        emit updateFid(m_frame_id);
        return;
    }

    int pack_size = USB_PACKAGE_SAVE_SIZE;
    //    if (g_SCAN_TYPE == TYPE_LINE) {
    //        pack_size = PACKAGE_SIZE;
    //    } else if (g_SCAN_TYPE == TYPE_SECTOR){
    //        pack_size = DMAMAXNUM_SECTOR;
    //    }

    if (!m_file.seek(i * pack_size)) {
        qDebug("file end!!");
  //      emit updateFid(m_frame_id);
        m_frame_id = -1;
        //        m_file.seek(0);
        return;
    }

  //  if (m_led_win) {
 //       m_led_win->setSliderID(i);
  //  }

    //    qDebug("tid: %d\tenqueue fid :\t%d", this->currentThreadId(), f_id);
    QByteArray f_data = m_file.read(pack_size);

    if (f_data.isEmpty() || f_data.size() != pack_size) {
  //      emit updateFid(m_frame_id);
        m_frame_id = -1;

    } else {
    //    emit updateFid(m_frame_id);
        m_frame_buffer->add(f_data);
    }

    //push data to stream
}

void FileReader::pr() {
    if (!this->isRunning()) {
        return;
    }

    if (m_paused) {
        resume();

    } else {
        pause();
    }
}

void FileReader::back() {
    if (m_paused || !this->isRunning()) {
        m_frame_id = MAX(0, m_frame_id - 1);
        push(m_frame_id);
    }
}

void FileReader::backB() {
    if (m_paused || !this->isRunning()) {
        m_frame_id = MAX(0, m_frame_id - 10);
        push(m_frame_id);
    }
}

void FileReader::forward() {
    if (m_paused || !this->isRunning()) {
        push(++m_frame_id);
    }
}

void FileReader::jumpTo(int fid) {
    if (m_paused || !this->isRunning()) {
        push(fid);
        m_frame_id = fid;
    }
}

void FileReader::forwardF() {
    if (m_paused || !this->isRunning()) {
        m_frame_id += JUMP_CNT;
        push(++m_frame_id);
    }
}

void FileReader::pause() {
    if (this->isRunning() && !m_paused) {
        qDebug("pause");
        m_mutex.lock();
        m_paused = true;
    }
}

void FileReader::resume() {
    if (this->isRunning() && m_paused) {
        qDebug("resume");
        m_mutex.unlock();
        m_paused = false;
    }
}

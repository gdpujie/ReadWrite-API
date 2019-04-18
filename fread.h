#ifndef FileReader_H
#define FileReader_H

#include <QMutex>
#include <QFile>

#include "reader.h"
//#include "ledwin.h"
class FileReader : public Reader {
    Q_OBJECT
public:

    FileReader();
    ~FileReader();
    int get_fid();
   // void setLedWin(LedWin *led_win);


private:
    void run();
    int m_frame_id = 0;
    bool m_paused;
    QMutex m_mutex;
    QFile m_file;
    LedWin *m_led_win;

signals:
    void updateFid(int);// led GUI interaction
public slots:
    void openFile(QString fileName);
    void play();
    void stop();

    void pause();
    void resume();
    void pr();
    void back();
    void backB();
    void forward();
    void jumpTo(int fid);
    void forwardF();
    void push(int i = -1);
    //    void stop();
    //    void start(Priority = InheritPriority);
    //    void terminate();
    //    void quit();

};

#endif // FileReader_H

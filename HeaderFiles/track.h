#ifndef TRACK_H
#define TRACK_H
#include <string>
#include <QString>
#include <tstring.h>
#include <QImage>

class track {
private:
    void loadtrackdata(const char* pict, int pic_size, TagLib::String artist, TagLib::String title, int duration);
    void error_reproduct(std::string &pth);
    int _duration;
    QString _artist;
    std::string _path;
    QString _title;
    QImage cover; // обложка
    friend class TrackInfoWidget;
public:
    track(std::string p);
    std::string& getpath();
};

#endif // TRACK_H

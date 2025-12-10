#ifndef SCROLLBARTRACKS_H
#define SCROLLBARTRACKS_H
#include "TrackInfoAbstract.h"
#include <vector>

class TrackInfoRepresentation: public TrackInfoBase {
    Q_OBJECT
private:
    int sizecover = 100;
public:
    TrackInfoRepresentation(const track& trk, QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;
};

class TrackInfoScroll: public QWidget {
    Q_OBJECT
private:
    std::vector<TrackInfoRepresentation*> track_widgets;
public:
    TrackInfoScroll(const std::vector<track>& tracks, int width, int height, QWidget* parent = nullptr);
};

#endif // SCROLLBARTRACKS_H

#ifndef SCROLLBARTRACKS_H
#define SCROLLBARTRACKS_H
#include "TrackInfoAbstract.h"
#include <QVector>
#include <QBoxLayout>
#include <QScrollArea>

class TrackInfoRepresentation: public TrackInfoBase {
    Q_OBJECT
private:
    int index_of_track;
    bool selectedTrack; // флаг актуального трека
public:
    TrackInfoRepresentation(const track& trk, int index, bool flag = false, QWidget* parent = nullptr);
    void setSelected(bool flag);
protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
signals:
    void setTrackFromPanel(int ind);
};

class TrackInfoScroll: public QWidget {
    Q_OBJECT
private:
    std::vector<TrackInfoRepresentation*> track_widgets;
    QVBoxLayout* layout;
    QScrollArea* scrollArea;
    QWidget* containerWidget;
    QVBoxLayout* mainLayuot;
    TrackInfoRepresentation* current;
    void clear_tracks();
public:
    TrackInfoScroll(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void clicked(int ind);
    void loadTracks(const QVector<track>& tracks);
signals:
    void SetNewTrackPanel(int ind);
};

#endif // SCROLLBARTRACKS_H

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
public:
    TrackInfoRepresentation(const track& trk, int index, QWidget* parent = nullptr);
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
public:
    TrackInfoScroll(const QVector<track>& tracks, QWidget* parent = nullptr);
    void loadTracks(const QVector<track>& tracks);
protected:
    void resizeEvent(QResizeEvent* event);
public slots:
    void clicked(int ind);
signals:
    void SetNewTrackPanel(int ind);
};

#endif // SCROLLBARTRACKS_H

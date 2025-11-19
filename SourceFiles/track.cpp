#include "track.h"
#include <taglib.h>
#include <tfile.h>
#include <cctype>
#include <algorithm>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mp4file.h>
#include <mp4coverart.h>
#include <flacfile.h>
#include <apetag.h>
#include <id3v1tag.h>
#include <QDebug>
#include <oggflacfile.h>
#include "DataManager.h"
#include <cmath>

#include <QFile>
void track::loadtrackdata(const char* pict, int pic_size, TagLib::String artist, TagLib::String title, int duration) {
    _title = QString::fromStdString(title.to8Bit(true));
    _artist = QString::fromStdString(artist.to8Bit(true));
    if (duration == 0) {
        int dur = int(std::ceil(GetDuration(_path.c_str())));
        if (dur == -1) {
            _duration = 180;
        }
        else {
            _duration = dur;
        }
    }
    else {
        _duration = duration;
    }
    if (pict)
        cover.loadFromData(reinterpret_cast<const uchar*>(pict), pic_size);
    else {
        cover = QImage(":/Resources/Icons/CoverMus.png");
    }
}

track::track(std::string path) {
    _path = path;
    // определить формат файла
    std::string format = _path.substr(_path.rfind('.') + 1);
    std::transform(format.begin(), format.end(), format.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (format == "mp3") {
        TagLib::MPEG::File f(path.c_str());
        if (!f.isValid() || !f.audioProperties()){
            error_reproduct(path);
        }
        TagLib::ByteVector dat;
        TagLib::String artist, title;
        if (f.hasAPETag()) {
            TagLib::APE::Tag* tag = f.APETag();
            artist = tag->artist();
            title = tag->title();
            const TagLib::APE::ItemListMap &items = tag->itemListMap();
            if (items.contains("COVER ART (FRONT)") && items["COVER ART (FRONT)"].type() == TagLib::APE::Item::Binary) {
                dat = items["COVER ART (FRONT)"].binaryData();
            }
            else if (items.contains("COVER ART (BACK)") && items["COVER ART (BACK)"].type() == TagLib::APE::Item::Binary) {
                dat = items["COVER ART (BACK)"].binaryData();
            }
        }
        if (f.hasID3v2Tag()) {
            TagLib::ID3v2::Tag* tag = f.ID3v2Tag();
            if (artist.isEmpty()){
                artist = tag->artist();
            }
            if (title.isEmpty()){
                title = tag->title();
            }
            if (dat.isEmpty()){
                const TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
                if (!frames.isEmpty()) {
                    for (auto it = frames.begin(); it != frames.end(); ++it){
                        auto *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it); // базовый frame не содержит методов type и picture
                        if (!frame->picture().isEmpty()) { // обложка
                            dat = frame->picture();
                            break;
                        }
                    }
                }
            }
        }
        if (artist.isEmpty() || title.isEmpty()) {
            standartprocessNames(f.tag(), artist, title, path);
        }
        loadtrackdata(dat.data(), dat.size(), artist, title, f.audioProperties()->lengthInSeconds());
    }
    else if (format == "m4a") {
        TagLib::MP4::File f(path.c_str());
        if (!f.isValid() || !f.audioProperties()) {
            error_reproduct(path);
        }
        TagLib::Tag* tag = f.tag();
        TagLib::String artist, title;
        standartprocessNames(tag, artist, title, path);

        TagLib::MP4::Tag *mp4tag = f.tag();
        auto &map = mp4tag->itemMap();
        auto iter = map.find("covr");
        TagLib::ByteVector dat;
        if (iter != map.end()){
            TagLib::MP4::CoverArtList covList = iter->second.toCoverArtList();
            if (!covList.isEmpty()) {
                TagLib::ByteVector dat;
                for (const auto &cover: covList){
                    if (cover.format() == TagLib::MP4::CoverArt::JPEG || cover.format() == TagLib::MP4::CoverArt::PNG){
                        dat = cover.data();
                        break;
                    }
                }
            }
        }
        loadtrackdata(dat.data(), dat.size(), artist, title, f.audioProperties()->lengthInSeconds());
    }
    else if (format == "flac"){
        TagLib::FLAC::File f(path.c_str());
        if (!f.isValid() || !f.audioProperties()){
            error_reproduct(path);
        }
        TagLib::String artist, title;
        if (f.hasXiphComment()) {
            TagLib::Ogg::XiphComment* tag = f.xiphComment();
            artist = tag->artist();
            title = tag->title();
        }
        if (f.hasID3v2Tag()) {
            TagLib::ID3v2::Tag *tag = f.ID3v2Tag();
            if (artist.isEmpty()){
                artist = tag->artist();
            }
            if (title.isEmpty()){
                title = tag->title();
            }
        }
        if (artist.isEmpty() || title.isEmpty()) {
            standartprocessNames(f.tag(), artist, title, path);
        }
        TagLib::ByteVector dat;
        auto pictures = f.pictureList();
        if (!pictures.isEmpty()) {
            for (auto *pic : pictures) {
                if (pic && pic->type() == TagLib::FLAC::Picture::FrontCover) {
                    dat = pic->data();
                    break;
                }
            }
        }
        loadtrackdata(dat.data(), dat.size(), artist, title, f.audioProperties()->lengthInSeconds());
    }
    else {
        TagLib::FileRef f(path.c_str());
        if (f.isNull() || !f.tag() || !f.audioProperties()){
            error_reproduct(path);
        }
        TagLib::Tag *tag = f.tag();
        TagLib::String artist, title;
        standartprocessNames(tag, artist, title, path);
        if (tag) {
            loadtrackdata(nullptr, 0, artist, title, f.audioProperties()->lengthInSeconds());
        }
    }
}

void track::standartprocessNames(TagLib::Tag* tag, TagLib::String& artist, TagLib::String& title, const std::string& path) {
    if (artist.isEmpty()) {
        artist = tag->artist();
        if (artist.isEmpty()) {
            auto start_artistname = path.rfind('/');
            auto end_artistname = path.rfind('-');
            if (start_artistname != std::string::npos && end_artistname != std::string::npos) {
                artist = path.substr(start_artistname + 1, end_artistname - start_artistname - 1);
            }
            else {
                artist = "Unknown artist";
            }
        }
    }
    if (title.isEmpty()) {
        title = tag->title();
        if (title.isEmpty()) {
            auto start_title = path.rfind('-');
            if (start_title != std::string::npos) {
                title = path.substr(start_title + 1, path.rfind('.') - start_title - 1);
            }
            else {
                title = "Unknown title";
            }
        }
    }
}

void track::error_reproduct(std::string& pth){
    throw std::runtime_error("Unsupported or invalid audio file: " + pth);
}

std::string& track::getpath() {
    return _path;
}

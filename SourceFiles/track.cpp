#include "track.h"
#include <taglib.h>
#include <fileref.h>
#include <tfile.h>
#include <cctype>
#include <algorithm>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mp4file.h>
#include <mp4coverart.h>
#include <flacfile.h>

void track::loadtrackdata(const char* pict, int pic_size, TagLib::String artist, TagLib::String title, int duration) {
    _title = QString::fromStdString(title.to8Bit(true));
    _artist = QString::fromStdString(artist.to8Bit(true));
    _duration = duration;
    if (pict)
        cover.loadFromData(reinterpret_cast<const uchar*>(pict), pic_size);
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
        if (!f.hasID3v2Tag() || !f.audioProperties()){
            error_reproduct(path);
        }
        TagLib::ID3v2::Tag *tag = f.ID3v2Tag(true);
        const TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
        TagLib::ByteVector dat;
        if (!frames.isEmpty()) {
            for (auto it = frames.begin(); it != frames.end(); ++it){
                auto *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it); // базовый frame не содержит методов type и picture
                if (frame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover){ // обложка
                    dat = frame->picture();
                    break;
                }
            }
        }
        loadtrackdata(dat.data(), dat.size(), tag->artist(), tag->title(), f.audioProperties()->lengthInSeconds());
    }
    else if (format == "m4a") {
        TagLib::MP4::File f(path.c_str());
        if (!f.hasMP4Tag() || !f.audioProperties()) {
            error_reproduct(path);
        }
        TagLib::MP4::Tag *tag = f.tag();
        auto &map = tag->itemMap();
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
        loadtrackdata(dat.data(), dat.size(), tag->artist(), tag->title(), f.audioProperties()->lengthInSeconds());
    }
    else if (format == "flac"){
        TagLib::FLAC::File f(path.c_str());
        if (!f.hasID3v2Tag() || !f.audioProperties()){
            error_reproduct(path);
        }
        TagLib::ID3v2::Tag *tag = f.ID3v2Tag(true);
        auto piclist = f.pictureList();
        TagLib::ByteVector dat;
        if (!piclist.isEmpty()){
            for (auto &pic: piclist){
                if (pic->type() == TagLib::FLAC::Picture::FrontCover){ // обложка
                    dat = pic->data();
                    break;
                }
            }
        }
        loadtrackdata(dat.data(), dat.size(), tag->artist(), tag->title(), f.audioProperties()->lengthInSeconds());
    }
    else {
        TagLib::FileRef f(path.c_str());
        if (f.isNull() || !f.tag() || !f.audioProperties()){
            error_reproduct(path);
        }
        TagLib::Tag *tag = f.tag();
        if (tag) {
            loadtrackdata(nullptr, 0, tag->artist(), tag->title(), f.audioProperties()->lengthInSeconds());
        }
    }
}

void track::error_reproduct(std::string& pth){
    throw std::runtime_error("Unsupported or invalid audio file: " + pth);
}

std::string& track::getpath() {
    return _path;
}

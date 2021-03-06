//Copyright (C) 2011  Groza Cristian, N3
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "VideoEntry.hpp"

VideoEntry::VideoEntry(VideoInfo* video_data): wxListItem(), video(video_data) {}

VideoInfo* VideoEntry::getVideoData()
{
    if (video != 0) return video;
    else return 0;
}

void VideoEntry::setVideoData(VideoInfo* video_data)
{
    video = video_data;
}

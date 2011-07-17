#ifndef VIDEOLISTCTRL_H
#define VIDEOLISTCTRL_H
#include <wx/wx.h>
#include <string>
#include <vector>
#include "VideoEntry.hpp"
#include "Enums.hpp"
#include "VideoInfo.hpp"


class VideoListCtrl: public wxListView {

/* This class was created to facilitate video entries management easier.
 * Feel free to add any method that you may think it will make things easier.
 */

public:
    /*Basic constructor.*/
    VideoListCtrl(wxWindow* parent);

    /*This method creates a wxListItem and sets its arguments as labels. */
    void AddVideo(VideoInfo* video_data);

    VideoEntry* GetVideoEntry(long index);
    bool DeleteAllItems();
private:


    std::vector<VideoEntry*> entries;

    wxListItem itemTitle;  //title column
    wxListItem itemRating; //rating column
    wxListItem itemViews;
    wxListItem itemAuthor;


protected:

};


#endif

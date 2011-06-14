#include "MainFrame.hpp"



MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, -1, title, pos, size)
{
    //Here we will initalize our controls
    // initialize jpeg image handler
    wxInitAllImageHandlers();
    listed_videos = new std::vector<VideoInfo*>();
    event_manager = new EventManager();

    //Combo box options
    wxArrayString choice_string;
    choice_string.Add(wxT("Videos"));
    choice_string.Add(wxT("User Videos"));
    choice_string.Add(wxT("Playlist"));

    splitter_win = new wxSplitterWindow(this);
    splitter_win -> SetMinimumPaneSize(1);
    upper_panel = new wxPanel(splitter_win);
    lower_panel = new wxPanel(splitter_win);
    lower_notebook = new wxNotebook(lower_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    description = new VideoDescription(lower_notebook, event_manager, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    lower_notebook -> AddPage(description, wxT("Description"));

    //Combo box, (option box), to give the user a more specific search
    combo_box = new wxComboBox(upper_panel, ID_COMBOBOX, wxT("Videos"), wxDefaultPosition,
                                             wxDefaultSize, choice_string, wxCB_READONLY);

    combo_box -> SetSelection(0); //initially, it is set to -1
    //Search box, this is where the user types in the url, user name, or other relevant info.
    search_box = new wxTextCtrl(upper_panel, TEXT_Search, wxT("Search"), wxDefaultPosition, wxSize(-1,-1),
                                    wxTE_RICH | wxTE_PROCESS_ENTER , wxDefaultValidator, wxTextCtrlNameStr);

    //Go Button, this initiates the search
    go_button = new wxButton(upper_panel, BUTTON_Go, _T("Go"), wxDefaultPosition, wxDefaultSize);

    //List control, this contains the video information
    video_list = new VideoListCtrl(upper_panel);
    //List control initial items

    box_sizer = new wxBoxSizer(wxHORIZONTAL);

    box_sizer->Add(search_box,
                             1,
                             wxEXPAND |
                             wxALL,
                             0);

    box_sizer->Add(combo_box,
		   0,            //make vertically strechable
		   wxEXPAND  |   //make horizontally stretchable
		   wxALL,        //and make border all around
		   0);           //set border width to 0

    box_sizer->Add(go_button,
                            0,
                            wxEXPAND |
                            wxALL,
                            0);

    topsizer = new wxBoxSizer(wxVERTICAL);

    topsizer->Add(box_sizer,
                            0,
                            wxEXPAND |
                            wxALIGN_CENTER);

    topsizer->Add(video_list,
                            1,
                            wxEXPAND |
                            wxALL,
                            0);

    upper_panel -> SetSizerAndFit(topsizer);

    lower_sizer = new wxBoxSizer(wxHORIZONTAL);
    lower_sizer -> Add(lower_notebook, 1, wxEXPAND | wxALL, 0);
    lower_panel -> SetSizerAndFit(lower_sizer);

    splitter_win -> SplitHorizontally(upper_panel, lower_panel, 1); // split the window in 2

    //Menu Bar
    MainMenu = new wxMenuBar();
    wxMenu *FileMenu = new wxMenu();
    wxMenu *EditMenu = new wxMenu();

    //Edit Menu functions
    EditMenu->Append(MENU_Pref, wxT("&Preferences\tCTRL+P"), wxT("Edit your preferences"));

    //File Menu functions
    FileMenu->Append(MENU_About, wxT("&About"), wxT("About youtube-wx"));
    FileMenu->Append(MENU_Quit, wxT("&Quit\tCTRL+Q"), wxT("Quit the program"));

    MainMenu->Append(FileMenu, wxT("&File"));
    MainMenu->Append(EditMenu, wxT("&Edit"));

    SetMenuBar(MainMenu);
    // create preferences window
    pref = new PrefWindow(this);

    CreateStatusBar();
    SetStatusText(_("youtube-wx, version 0.0.1")); //"youtube-wx version %s" % (_WXYT_VERSION))

}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(MENU_Quit, MainFrame::OnQuit)
    EVT_MENU(MENU_About, MainFrame::OnAbout)
    EVT_MENU(MENU_Pref, MainFrame::OnPref)
    EVT_BUTTON(BUTTON_Go, MainFrame::OnSearch)
    EVT_CHOICE(ID_COMBOBOX, MainFrame::OnSearch)
    EVT_RADIOBUTTON(ID_RADIOBUTTON1, MainFrame::OnAbout)
    EVT_LIST_ITEM_SELECTED(LIST_Video_list, MainFrame::OnVideoSelect)
    EVT_LIST_ITEM_ACTIVATED(LIST_Video_list, MainFrame::OnVideoWatch)
    EVT_TEXT_ENTER(TEXT_Search, MainFrame::OnSearch)
    
END_EVENT_TABLE()



void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_("Small utility to download youtube videos, based off of youtube-dl"),
        _("About Youtube-wx"),
        wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnPref(wxCommandEvent& WXUNUSED(event))
{
        pref->Show(true); 	// show the preferences window
        return;
}

void MainFrame::OnSearch(wxCommandEvent& WXUNUSED(event))
{
    wxString search_value  = search_box->GetValue(); //get search string
    SearchURL search_url(getSearchType(), search_value); //make URL with search string
    //get the search results
    XMLFeed feed(&search_url);

    // delete the the VideoInfo allocated on the heap
    if(listed_videos -> size()){
	std::vector<VideoInfo*>::iterator it = listed_videos -> begin();
	for(it; it != listed_videos -> end(); ++it) delete (*it);
	listed_videos -> clear();
	VideosDeleteEvent event;
	event_manager -> FireVideosDeleteEvent(&event);

    }

    if (feed.fetchFeed()) //fetch youtube xml feed
    {
	switch(search_url.getSearchType())
	{
        case VIDEO_SEARCH:
        case USER_VIDEO_SEARCH:
	    Parser::parseVideoFeed(listed_videos, feed.getXMLFeed());
	    break;

        case PLAY_LIST_SEARCH:
	    Parser::parsePlaylistFeed(listed_videos, feed.getXMLFeed());
	    break;
	}
    }
    else //something went wrong
    {
      switch (feed.getErrorCode()){ // recovering the curl exit code
                case 6:
                        wxMessageBox(_("Could not resolve host"), _("Error"), wxOK | wxICON_INFORMATION);
                        break;
                case 7:
                        wxMessageBox(_("Could not connect"),      _("Error"), wxOK | wxICON_INFORMATION);
                        break;
                case 28:
                        wxMessageBox(_("Operation timed out"),    _("Error"), wxOK | wxICON_INFORMATION);
                        break;
                case 52:
                        wxMessageBox(_("Got nothing"),            _("Error"), wxOK | wxICON_INFORMATION);
                        break;
                case 56:
                        wxMessageBox(_("Recieve error"),          _("Error"), wxOK | wxICON_INFORMATION);
                        break;
                case NO_SEARCH_RESULT:
                        break;
                default:
                        wxMessageBox(_("Undocumented error"),     _("Error"), wxOK | wxICON_INFORMATION);
                        break;
                        }
      return;    // abbort the function
        }

    // delete listed_videos;
    video_list -> DeleteAllItems(); //prepare list for new entry stream

    //vector iterator
    std::vector<VideoInfo*>::iterator p = listed_videos -> begin();
    // add the items one by one
    // if the vector is empty, there will be no problem
    for(p; p != listed_videos -> end(); ++p){
        video_list -> AddVideo(*p);

    }

}

void MainFrame::OnVideoSelect(wxListEvent& event)
{
    
  long video_item_index = video_list -> GetFirstSelected () ; //get selected video
  if (video_item_index != -1) //if found
  {

    VideoEntry* item = video_list -> GetVideoEntry(video_item_index); //get it's video entry object
    VideoInfo*  info = item -> getVideoData();
    VideoSelectEvent evt(info);
    event_manager -> FireVideoSelectEvent(&evt);
  }
    event.Skip();

}

SearchType MainFrame::getSearchType()
{
  int index = combo_box -> GetCurrentSelection(); //get the index from the search 
                                                  //criteria selection box

    switch (index)
    {
        case 0:
            return VIDEO_SEARCH;
            break;
        case 1:
            return USER_VIDEO_SEARCH;
            break;
        case 2:
	    return PLAY_LIST_SEARCH;
            break;
    }

}

void MainFrame::OnVideoWatch(wxListEvent& event)
{
  long video_item_index = video_list -> GetFirstSelected () ; //get selected video
  if (video_item_index != -1){ //if found

    VideoEntry* item = video_list -> GetVideoEntry(video_item_index); //get it's video entry object
    // open the browser to the video entry link
    wxLaunchDefaultBrowser(wxString(item -> getVideoData() -> getLink().c_str(), wxConvUTF8));

    }
    event.Skip();
}

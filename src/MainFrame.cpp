#include "MainFrame.hpp"
#ifdef MAINFRAME_H


MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, -1, title, pos, size)
{


    //Here we will define our controls


    //Combo box options
    wxArrayString choice_string;
    choice_string.Add(wxT("Search"));
    choice_string.Add(wxT("User videos"));
    choice_string.Add(wxT("Playlist"));

    splitter_win = new wxSplitterWindow(this);
    splitter_win -> SetMinimumPaneSize(1);
    upper_panel = new wxPanel(splitter_win);
    lower_panel = new wxPanel(splitter_win);

    //Combo box, (option box), to give the user a more specific search
    combo_box = new wxComboBox(upper_panel, ID_COMBOBOX, wxT("Search"), wxDefaultPosition, wxDefaultSize, choice_string, wxCB_READONLY);

    //Search box, this is where the user types in the url, user name, or other relevant info.
    search_box = new wxTextCtrl(upper_panel, TEXT_Search, wxT("Enter a valid url or search query"), wxDefaultPosition, wxSize(-1,-1),
				    wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);

    //Go Button, this initiates the search
    go_button = new wxButton(upper_panel, BUTTON_Go, _T("Go"), wxDefaultPosition, wxDefaultSize);

    //List control, this contains the video information
    video_list = new VideoListCtrl(upper_panel);
    //List control initial items

    video_descr = new wxTextCtrl(lower_panel, wxID_ANY, wxT("Video description.") ,wxDefaultPosition, wxSize(-1, -1),
            wxTE_READONLY | wxBORDER_SUNKEN | wxTE_RICH | wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);


    box_sizer = new wxBoxSizer(wxHORIZONTAL);

    box_sizer->Add(search_box,
			     1,
			     wxEXPAND |
			     wxALL,
			     0);


    box_sizer->Add(combo_box,
			    0, 		//make vertically strechable
			    wxEXPAND  | //make horizontally stretchable
			    wxALL, 	// and make border all around
			    0);		//set border width to 0


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

    lower_sizer-> Add( video_descr,
                    1,
                    wxEXPAND | wxALL,
                    0);

    lower_panel -> SetSizerAndFit(lower_sizer);

    splitter_win -> SplitHorizontally(upper_panel, lower_panel, -1);

    //Menu Bar
    MainMenu = new wxMenuBar();
    wxMenu *FileMenu = new wxMenu();
    wxMenu *EditMenu = new wxMenu();


    //Edit Menu functions
    EditMenu->Append(MENU_Pref, wxT("&Preferences..."), wxT("Edit your preferences"));

    //File Menu functions
    FileMenu->Append(MENU_About, wxT("&About..."), wxT("About youtube-wx"));
    FileMenu->Append(MENU_Quit, wxT("&Quit"), wxT("Quit the program"));



    MainMenu->Append(FileMenu, wxT("&File"));
    MainMenu->Append(EditMenu, wxT("&Edit"));

    SetMenuBar(MainMenu);
    CreateStatusBar();
    SetStatusText(_("youtube-wx, version 0.0.1"));




}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(MENU_Pref, MainFrame::OnSearch)
    EVT_MENU(MENU_Quit, MainFrame::OnQuit)
    EVT_MENU(MENU_About, MainFrame::OnAbout)
    EVT_MENU(MENU_Pref, MainFrame::OnPref)
    EVT_BUTTON(BUTTON_Go, MainFrame::OnSearch)
    EVT_CHOICE(ID_COMBOBOX, MainFrame::OnSearch)
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

}

void MainFrame::OnSearch(wxCommandEvent& WXUNUSED(event))
{
    wxString search_value;
    search_value = search_box->GetValue();

    //get the sealrh results
    listed_videos = get_search_result(search_value);

    if(listed_videos == NULL){
        //notify user of failure
        wxMessageBox(_("Curl has had an error, are you connected to the internet?"),_("Error"), wxOK | wxICON_INFORMATION);

        return;
    }

    //Prepare the list for new population
    //We must delete the old entries to add the new ones.
    video_list -> DeleteAllItems();

    //vector iterator
    std::vector<VideoInfo*>::iterator p = listed_videos -> begin();


    //add the items one by one
    for(p; p != listed_videos -> end(); ++p){
        video_list -> AddVideo(*p);

    }


}

#endif

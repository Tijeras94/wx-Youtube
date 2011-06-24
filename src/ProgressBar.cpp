#include "ProgressBar.hpp"
#include <iostream>

ProgressBar::ProgressBar(wxWindow* parent, wxWindowID id, int range, const wxString& name, const wxPoint& pos ,
			 const wxSize& size , long style,  const wxValidator& validator):

    wxPanel(parent), h_sizer(0), cancel_bt(0), gauge(0)
{
    self = this;
    h_sizer = new wxBoxSizer(wxHORIZONTAL);
    cancel_bt = new wxButton(this, ID_CANCEL_BUTTON, wxT("Cancel"));
    gauge = new wxGauge(this, id, range, pos, size, style, validator, name );


    h_sizer -> Add(gauge, 2, wxEXPAND | wxALL, 0);
    h_sizer -> Add(cancel_bt, 1, wxEXPAND | wxALL, 0);
   
    SetSizerAndFit(h_sizer);
}


void ProgressBar::Update(int val)
{

    gauge -> SetValue(val);
    wxWakeUpIdle();

}

int ProgressBar::CurlProgressCallback(void* ptr, double total_dl, double dled_now , double total_upl  , double upled_now)
{
    if(self){
	if (total_dl > 0) 		// avoid division by zero
	{
	    int percentage = (dled_now / total_dl) * 100; // calculate %
      
	    self -> Update(percentage);
	    if(total_dl == dled_now)
	    {
		self -> Destroy();
		self = 0;
	    }
	    // we should notify the user here that the downlod is complete.
	}
    }
    return curl_callback_code;
}

void ProgressBar::OnCancel(wxCommandEvent& event)
{
    curl_callback_code = 1; 	// abort curl download
    self -> Destroy();		// destroy progress bar and remove it from status bar

}

ProgressBar* ProgressBar::self = 0; // default
int ProgressBar::curl_callback_code = 0; 

BEGIN_EVENT_TABLE(ProgressBar, wxPanel)
    EVT_BUTTON(ID_CANCEL_BUTTON, ProgressBar::OnCancel)
END_EVENT_TABLE()

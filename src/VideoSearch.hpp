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

#ifndef VIDEO_SEARCH_H
#define VIDEO_SEARCH_H

#include <wx/wx.h>
#include <cstdlib>
#include <string>
#include <vector>
#include "curl/curl.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "VideoInfo.hpp"
#include "SearchURL.hpp"
#include "Parser.hpp"

class VideoSearch
{
private:
    struct request_information
    {
	std::string encoding;
	std::string version;
    };

//Write all expected data in here
    static std::string buffer;

//This is the writer callback function called by curl
    static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);

//creates the video info vector
    void dealWithResult();
//search URL used by the youtube API
    const SearchURL* m_search_url;
    int m_curl_result;
    std::vector<VideoInfo*>* m_search_results;

public:
    VideoSearch(const SearchURL* search_url);
    ~VideoSearch();
    // fetches the youtube xml feed
    bool doSearch();
    //returns search video info entries.
    std::vector<VideoInfo*>* getSearchResults();
    int getCurlCode() const;
};

#endif

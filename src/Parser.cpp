#include "Parser.hpp"

void Parser::parsePlaylistFeed(std::vector<VideoInfo*>* buffer, rapidxml::xml_document<>* feed)
{
    if(buffer == 0)
    {
	std::cerr << "parsePlaylistFeed received 0 buffer.";
	return;
    }
    if(feed == 0)
    {
	std::cerr << "parsePlaylistFeed received 0 feed.";
	return;
    }
    rapidxml::xml_node<>* feed_node = feed -> first_node("feed");
    if(feed_node == 0)
    {
	std::cerr << "Bad feed_node pointer in parsePlaylistFeed.";
	return;
	
    }
    rapidxml::xml_node<>* cur_node = feed_node -> first_node("entry"); //Setup our initial node

    while (cur_node != 0)
    {
	buffer -> push_back( new VideoInfo(cur_node -> first_node("title") -> value(),
					   "N/A",
					   "N/A",
					   cur_node -> first_node("link")-> next_sibling("link") -> first_attribute("href") -> value(),
					   std::string("Entries: " + std::string(cur_node -> first_node("yt:countHint")-> value())+
						       "\n" + std::string(cur_node -> first_node("summary") -> value())),
					   cur_node -> first_node("author") -> first_node("name") -> value(),
					   cur_node -> first_node("yt:playlistId") -> value(),
					   "N/A"));
        cur_node = cur_node->next_sibling("entry"); // Iterate to the next entry sibling
    }
}

void Parser::parseVideoFeed(std::vector<VideoInfo*>* buffer, rapidxml::xml_document<>* feed)
{
    if(buffer == 0)
    {
	std::cerr << "parseVideoFeed received 0 buffer.";
	return;
    }
    if(feed == 0)
    {
	std::cerr << "parseVideoFeed received 0 feed.";
	return;
    }
    rapidxml::xml_node<>* feed_node = feed -> first_node("feed");

    if(feed_node == 0)
    {
	std::cerr << "Bad feed_node pointer in parseVideoFeed.";
	return;
	
    }
    rapidxml::xml_node<>* cur_node = feed_node -> first_node("entry"); //Setup our initial node

    //creating video info object for each found video.
    while (cur_node != 0)
    {
	buffer -> push_back( new VideoInfo(cur_node -> first_node("title") -> value(),
					   cur_node -> first_node("gd:rating")-> first_attribute("average") -> value(),
					   cur_node -> first_node("yt:statistics")-> first_attribute("viewCount") -> value(),
					   cur_node -> first_node("link") -> first_attribute("href") -> value(),
					   cur_node-> first_node("media:group") -> first_node("media:description") -> value(),
					   cur_node -> first_node("author") -> first_node("name") -> value(),
					   std::string( cur_node -> first_node("id") -> value()).substr(42, std::string::npos),
					   cur_node -> first_node("media:group") -> first_node("media:thumbnail") -> first_attribute("url") -> value()));

        cur_node = cur_node->next_sibling("entry"); // Iterate to the next entry sibling
    }
}

void Parser::parseCommentsFeed(std::vector<CommentInfo*>* buffer, rapidxml::xml_document<>* feed)
{
    if(buffer == 0)
    {
	std::cerr << "parseCommentFeed received 0 buffer.";
	return;
    }
    if(feed == 0)
    {
	std::cerr << "parseCommentFeed received 0 feed.";
	return;
    }
    rapidxml::xml_node<>* feed_node = feed -> first_node("feed");

    if(feed_node == 0)
    {
	std::cerr << "Bad feed_node pointer in parseCommentFeed.";
	return;
	
    }
    rapidxml::xml_node<>* cur_node = feed_node -> first_node("entry"); //Setup our initial node

    while (cur_node != 0)
    {
	buffer -> push_back( new CommentInfo(cur_node -> first_node("title") -> value(),
					     cur_node -> first_node("author") -> first_node("name") -> value(),
					     cur_node -> first_node("content") -> value()
				 ));
        cur_node = cur_node->next_sibling("entry"); // Iterate to the next entry sibling
    }
}

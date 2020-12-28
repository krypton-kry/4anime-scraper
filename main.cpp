#include <bits/c++config.h>
#include <cpr/cpr.h>
#include <cpr/response.h>
#include <htmlcxx/html/Node.h>
#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <regex>

std::vector<std::string> search(cpr::Response response);
void scrape_direct_links(std::vector<std::string> urls);
std::vector<std::string> scrape_episode_links(std::string url);
std::vector<std::string> search_regex(std::vector<std::string> responses);

int main(int argc, char** argv){

  if(sizeof(argv)/sizeof(argv[0]) != 1){
    std::cout << "Please enter Anime Name" << std::endl;
    return 1;
  }
 // get search results 
  cpr::Response resp = cpr::Post(
      cpr::Url{"https://4anime.to/wp-admin/admin-ajax.php"},
      cpr::Payload{
      {"action","ajaxsearchlite_search"},
      {"aslp",argv[1]},
      {"asid","2"},
      {"options",
      "qtranslate_lang=0&set_intitle=None&customset%5B%5D=anime"                  }
  });  
  
//check status and do whatever you do
 if(resp.status_code == 200){
   std::vector<std::string> anime_links;
   std::vector<std::string> episode_links;

   anime_links = search(resp);
   
   for(int i = 0; i < anime_links.size(); i++){
     std::cout << i << ". " << anime_links[i] << "\n"<< std::endl;
   }
   int user_inp;
   std::cout << "Please Select a anime : ";
   std::cin >> user_inp; 

   if(user_inp > anime_links.size()){
      std::cout << "Please enter a number within range!" << std::endl;
   } else {
      std::cout << "\nPlease Wait!! scraping links!!\n" << std::endl;
      episode_links = scrape_episode_links(anime_links[user_inp]);
      
      scrape_direct_links(episode_links);   
   }
 } else {
   std::cout 
     << "\n[ERROR] Status Code : " 
     << resp.status_code << "Whlie searching for results!" << std::endl;
 }
  
  return 0;
}

std::vector<std::string> search(cpr::Response response){
  
   std::vector<std::string> anime_loc;

   htmlcxx::HTML::ParserDom parser;
   tree<htmlcxx::HTML::Node> dom = parser.parseTree(response.text);

   tree<htmlcxx::HTML::Node>::iterator iter = dom.begin();
   tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    
   for(; iter!=end; ++iter){
    
    if(iter->tagName() == "a"){
     
      iter->parseAttributes();
      
      std::string a_href = iter->attribute("href").second;
   
      if(a_href.find("https://4anime.to/") != std::string::npos){
          
          anime_loc.push_back(a_href);
      }
    }
  }
   return anime_loc; 
}

std::vector<std::string> scrape_episode_links(std::string url){
  std::vector<std::string> anime_links;

  //to implement - get episode to down from user
  cpr::Response resp = cpr::Get(cpr::Url(url));
  

  htmlcxx::HTML::ParserDom parser;
  tree<htmlcxx::HTML::Node> dom = parser.parseTree(resp.text);

  tree<htmlcxx::HTML::Node>::iterator iter = dom.begin();
  tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    
  for(; iter!=end; ++iter){
    
    if(iter->tagName() == "a"){
     
      iter->parseAttributes();
      
      std::string a_href = iter->attribute("href").second;

        if(a_href.find("/?id=") != std::string::npos){
          anime_links.push_back(a_href);   
        }
      }
    }
  return anime_links;
}

void scrape_direct_links(std::vector<std::string> urls){
  std::vector<std::string> direct_links;
  std::vector<std::string> resp_str;

  for(int i=0; i<urls.size(); ++i){
    cpr::Response resp = cpr::Get(cpr::Url(urls[i]));
    resp_str.push_back(resp.text);
  }

 direct_links =  search_regex(resp_str); 
  for(int i=0;i<direct_links.size();++i){
    std::cout << direct_links[i] << std::endl;
  }
}

std::vector<std::string> search_regex(std::vector<std::string> responses){
  std::vector<std::string> result;
  std::regex re("https.*?mp4");
  
  for(int i=0; i<responses.size(); ++i){
    std::sregex_iterator next(responses[i].begin(), responses[i].end(), re);
    std::sregex_iterator end;
    
    while (next != end) {
      std::smatch match = *next;
     result.push_back(match.str());  
      next++;
    }
  }
 sort( result.begin(), result.end() );
 result.erase( unique( result.begin(), result.end() ), result.end() ); 
 
 return result;
}

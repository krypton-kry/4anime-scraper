#include <bits/c++config.h>
#include <cpr/cpr.h>
#include <cpr/response.h>
#include <htmlcxx/html/Node.h>
#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include <utility>
#include <vector>


std::vector<std::string> search(cpr::Response response);

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

   anime_links = search(resp);
    
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
          
          std::cout << a_href << std::endl;
      }
    }
  }
   return anime_loc; 
}

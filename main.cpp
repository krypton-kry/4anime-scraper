#include <cpr/cpr.h>
#include <htmlcxx/html/Node.h>
#include <iostream>
#include <htmlcxx/html/ParserDom.h>

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
  

 if(resp.status_code == 200){
   htmlcxx::HTML::ParserDom parser;
   tree<htmlcxx::HTML::Node> dom = parser.parseTree(resp.text);

   tree<htmlcxx::HTML::Node>::iterator iter = dom.begin();
   tree<htmlcxx::HTML::Node>::iterator end = dom.end(); 
   for(; iter!=end; ++iter){
    if(iter->tagName() == "img"){
      iter->parseAttributes(); 
      std::cout << "https://4anime.to" <<iter->attribute("src").second << std::endl;
    }
   }

 }else{
   std::cout 
     << "\n[ERROR] Status Code : " 
     << resp.status_code << "Whlie searching for results!" << std::endl;
 }
  
  return 0;
}

cmake_minimum_required(VERSION 3.14.0)
project(4anime-scraper)

set(LIBRARY_LOCATION /usr/local/lib)

find_library(CPR_LIBRARY 
  NAMES cpr 
  HINTS "${LIBRARY_LOCATION}")

find_library(HTMLCXX_LIBRARY 
  NAMES htmlcxx 
  HINTS "${LIBRARY_LOCATION}")

add_executable(4anime-scraper main.cpp)
target_link_libraries(4anime-scraper PRIVATE "${CPR_LIBRARY}")
target_link_libraries(4anime-scraper PRIVATE "${HTMLCXX_LIBRARY}")

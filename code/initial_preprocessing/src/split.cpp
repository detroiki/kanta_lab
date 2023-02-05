#include "header.h"

template <typename Out>

/*/
    Splits a string based on a given delimiter. Copied from https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
*/
void split(const std::string &s, 
           const char *delim, 
           Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, *delim)) {
        *result++ = item;
    }
}

/*/
    Splits a string based on a given delimiter. Copied from https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
*/
std::vector<std::string> split(const std::string &s, 
                                const char *delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
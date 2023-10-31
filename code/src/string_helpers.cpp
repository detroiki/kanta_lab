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


std::string concat_string(const std::vector<std::string> &elems, std::string sep) {    
    std::stringstream ss;
    long unsigned int count = 0;
    for(auto elem: elems) {
        ss << elem;
        if(count < elems.size()-1)
            ss << sep;
        count++;
    }
    std::string final = ss.str();
    return(final);
} 

std::string concat_string(const std::unordered_set<string> &elems, std::string sep) {    
    std::stringstream ss;
    long unsigned int count = 0;
    for(auto elem: elems) {
        ss << elem;
        if(count < elems.size()-1)
            ss << sep;
        count++;
    }
    std::string final = ss.str();
    return(final);
} 

//     std::string concat = std::accumulate(std::make_move_iterator(v.begin()),
//                                          std::make_move_iterator(v.end()),
//                                          std::string());
//  https://en.cppreference.com/w/cpp/iterator/move_iterator

// Transform a string to lower case
std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return(str);
}

std::string remove_chars(std::string str,
                         char remove_char) {
    str.erase(std::remove_if(str.begin(), str.end(), [remove_char](char c) {
        return c == remove_char;}), str.end());
    return(str);
}

std::string get_omop_identifier(std::string lab_id,
                                std::string lab_abbreviation,
                                std::string lab_unit,
                                std::string sep = std::string(" ")) {

    // Currently identifying the OMOP concept by the lab ID and abbreviation.
    std::vector<std::string> omop_identifier_vec = {lab_id, lab_abbreviation, lab_unit};
    std::string omop_identifier = concat_string(omop_identifier_vec, sep);

    return(omop_identifier);
} 

/** 
 * @brief Splits a string based on a given delimiter
 * 
 * @param input string to split
 * @param delimiter delimiter to split on
 * 
 * @return vector of strings
 * 
 * @details
 * Splits a string based on a given delimiter. Produced by chatGPT. 
 * Splits string and ignored delimiter inside "". 
 */
std::vector<std::string> splitString(const std::string &input, 
                                     char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream ss(input);
  std::string token;
  bool inQuotedString = false;
  
  for (char c : input) {
    if (c == '"') {
      inQuotedString = !inQuotedString; // Toggle quoted string state
    } else if ((c == delimiter && !inQuotedString)) {
      tokens.push_back(token);
      token.clear();
    } else {
      token += c;
    }
  }
  tokens.push_back(token); // Add the last token
  
  return tokens;
}
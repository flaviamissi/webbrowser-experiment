//
// converts HTML tags to ANSI escape sequences
//
#include <iostream>
#include <map>

std::map<std::string,std::string> html_tags = {
    {"<!DOCTYPE", ""},
    {"<html", "</html>"},
    {"<head", "</head>"},
    {"<title", "</title>"},
    {"<meta", "</meta>"},
    {"<body", "</body>"},
    {"<h1", "</h1>"},
    {"<a", "</a>"},
    {"<p", "</p>"},
    {"<img", "</img>"},
    {"<div", "</div>"},
    {"<span", "</span>"},
    {"<strong", "</strong>"},
    {"<em", "</em>"},
    {"<link", ""},
    {"<br", ""}
};

std::map<std::string,std::string> no_contents_map = {
    {"<script", "</script>"},
    {"<style", "</style>"},
};

std::map<std::string,std::string> conversion_map = {
    {"<title", "\33\[47m\33\[30m\n"},
    {"<a", "\33\[34m"},
    {"<div", "\033\[37m\t\t"},
    {"<span", " "},
    {"<strong", "\033\[1m"},
    {"<em", "\033\[7m"},
    // {"<link", ""},
    // {"<br", ""}
};

std::string removeTagContent(std::string html) {
    std::map<std::string,std::string>::iterator it;
    std::string opening_tag;
    std::string closing_tag;
    std::size_t pos;
    std::size_t close_pos;

    for(it = no_contents_map.begin(); it != no_contents_map.end(); it++) {
        opening_tag = it->first;
        closing_tag = it->second;
        while((pos = html.find(opening_tag)) != std::string::npos) {
            close_pos = html.find(closing_tag, pos);
            html.replace(pos, (close_pos - pos) + 1, "");
        }
    }
    return html;
}

//
// loops through all tags on the document.
// replaces them with their matching ANSI escape sequence.
//
std::string HTMLToANSI(std::string html) {
    std::map<std::string,std::string>::iterator it;
    std::size_t pos;
    std::size_t close_pos;
    std::string close_entity = ">";
    int shift = close_entity.size();
    std::string opening_tag;
    std::string closing_tag;

    html = removeTagContent(html);

    for(it = html_tags.begin(); it != html_tags.end(); it++) {
        opening_tag = it->first;
        while ((pos = html.find(opening_tag)) != std::string::npos) {
            close_pos = html.find(close_entity, pos);

            if (close_pos != std::string::npos)
                html.replace(pos, (close_pos - pos) + shift, conversion_map[opening_tag]);
            else
                html.replace(pos, opening_tag.size() + close_entity.size(), conversion_map[opening_tag]);

            closing_tag = it->second;
            pos = html.find(closing_tag);

            if (pos == std::string::npos)
                continue;

            html.replace(pos, closing_tag.size(), "");
        }
    }
    return html;
}

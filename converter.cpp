//
// converts HTML tags to ANSI escape sequences
//
#include <iostream>
#include <map>

std::map<std::string,std::string> html_tags = {
    {"<!DOCTYPE", ""},
    {"<!doctype", ""},
    {"<html", "</html>"},
    {"<head", "</head>"},
    {"<title", "</title>"},
    {"<meta", "</meta>"},
    {"<body", "</body>"},
    {"<h1", "</h1>"},
    {"<h2", "</h2>"},
    {"<center", "</center>"},
    {"<a", "</a>"},
    {"<p", "</p>"},
    {"<ol", "</ol>"},
    {"<ul", "</ul>"},
    {"<li", "</li>"},
    {"<table", "</table>"},
    {"<td", "</td>"},
    {"<tr", "</tr>"},
    {"<img", "</img>"},
    {"<svg", "</svg>"},
    {"<div", "</div>"},
    {"<form", "</form>"},
    {"<input", ""},
    {"<nav", "</nav>"},
    {"<header", "</header>"},
    {"<footer", "</footer>"},
    {"<span", "</span>"},
    {"<strong", "</strong>"},
    {"<em", "</em>"},
    {"<link", ""},
    {"<br", ""}
};

//
// tags that should have their content removed
//
std::map<std::string,std::string> no_contents_map = {
    {"<script", "</script>"},
    {"<style", "</style>"},
};

//
// HTML tags to ANSI escaping sequences
//
std::map<std::string,std::string> conversion_map = {
    {"<title", "\33\[37m\n"},
    {"<a", "\33\[34m"},
    {"<header", ""},
    {"<footer", ""},
    {"<div", "\033\[37m\n"},
    {"<span", " "},
    {"<li", " | "},
    {"<p", "\n"},
    {"<strong", "\033\[1m"},
    {"<em", "\033\[7m"},
    {"<h1", ""},
    {"<h2", ""},
    {"<ul", ""},
    {"<li", ""},
    {"<svg", ""},
    // {"<link", ""},
    // {"<br", ""}
};

//
// Removes specific tags content and the tags themselves.
// Uses tags defined in the `no_contents_map`
//
std::string removeTagsContent(std::string html) {
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
            html.replace(pos, (close_pos - pos) + closing_tag.size(), "");
        }
    }
    return html;
}

std::string improveFormatting(std::string html) {
    std::size_t pos;
    std::string token = "  ";
    while ((pos = html.find(token)) != std::string::npos) {
        html.replace(pos, token.size(), " ");
    }

    token = "\n \n";
    while ((pos = html.find(token)) != std::string::npos) {
        html.erase(pos, token.size());
    }

    token = "\n\n ";
    while ((pos = html.find(token)) != std::string::npos) {
        html.erase(pos, token.size());
    }

    return html;
}

//
// Loops through all tags on the document.
// Replaces them with their matching ANSI escape sequence.
//
std::string HTMLToANSI(std::string html) {
    std::map<std::string,std::string>::iterator it;
    std::size_t pos;
    std::size_t close_pos;
    std::string close_entity = ">";
    int shift = close_entity.size();
    std::string opening_tag;
    std::string closing_tag;

    html = removeTagsContent(html);

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

    return improveFormatting(html);
}

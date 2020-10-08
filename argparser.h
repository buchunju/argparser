/*****************************************************
** File    : argparser.h
** Project : argparser
** Date    : 08-10-20
** Author  : Kenneth Buchunju
** Email   : buchunjukenneth@gmail.com
**
** Copyright 2020 Kenneth Buchunju <buchunjukenneth@gmail.com>
**
******************************************************/


#ifndef _ARGPARSER_H
#define _ARGPARSER_H

#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <list>
#include <map>

const int LINE_LENGTH = 100; // Length of the line in help menu.

// Argument types that can be parsed.
enum arg_type{
    A_INT    = 0,
    A_FLOAT  = 1,
    A_DOUBLE = 2,
    A_STRING = 3,
    A_CHAR   = 4,
    A_BOOL   = 5,
    A_NONE   = 6

};

// Structure for holding parsed data.
struct option_t
{
    bool is_required;         // is argument required.
    std::string opt_name;     // Option name. (long)
    std::string opt_alt_name; // Option name. (short)
    bool* is_parsed;          // Has argument been parsed.
    arg_type opt_type;        // Type of argument parsed.
    void *opt_ref;            // Reference pointing to where to store parsed values.
    std::string opt_desc;     // Option description
};

class argparser{
public:
    /**
     * @brief argparser constructer
     * @param argc int argument count.
     * @param argv char** argument list.
     */
    argparser(int argc, char **argv);

    /**
      * @brief ~argparser destructer.
      */
    ~argparser();
    
    /**
     * @brief add_option_group
     * @param opt_group_dec const char* name of a group of options.
     */
    void add_option_group(const char* opt_group_dec = "");

    /**
     * @brief add_option add options that will be used for parsing.
     * @param a_name const char* long option name.
     * @param alt_name const char* short option name.
     * @param is_parsed bool true if argument has been parsed.
     * @param desc const char* option description.
     * @param is_required bool true if argument is required.
     */
    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc, bool is_required);

    /**
     * @brief add_option add options that will be used for parsing.
     * @param a_name const char* long option name.
     * @param alt_name const char* short option name.
     * @param is_parsed bool true if argument has been parsed.
     * @param type arg_type type of argument value to parse.
     * @param ref void* ref to where to store value.
     * @param desc const char* option description.
     * @param is_required bool true if argument is required.
     */
    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    arg_type type, void* ref, const char* desc,  bool is_required);

    /**
     * @brief parse parsing the arguments passed to the constructer.
     * @return int 0 if parsed successfully.
     */
    int parse();

    /**
     * @brief set_usage_info
     * @param info std::string the usage information.
     */
    void set_usage_info(std::string info);

    /**
     * @brief set_header_info set header data.
     * @param info std::string header information.
     */
    void set_header_info(std::string info);

    /**
     * @brief set_footer_info set footer data.
     * @param info std::string footer information.
     */
    void set_footer_info(std::string info);

private:
    /**
     * @brief The opt_check struct used for navigating through my_options variable.
     */
    struct opt_check{
        
        void operator=(opt_check &tmp)
        {
            this->group = tmp.group;
            this->index = tmp.index;
        }

        std::string group;
        int index;
    } ;

    /**
     * @brief show_help
     * @param message std::string message to show with the help menu.
     */
    void show_help(std::string message = "");

    /**
     * @brief add_help_option add the help long and short options if not defined by user.
     */
    void add_help_option();

    /**
     * @brief check_option
     * @param opt const char* the option o find in my_options.
     * @return opt_check the group and index of an argument.
     */
    opt_check check_option(const char* opt);

    /**
     * @brief check_duplicate
     * @param a_name const char* long option name to check.
     * @param alt_name const char* short option name to check.
     * @return int 0 if no duplicate found.
     */
    int check_duplicate(const char* a_name, const char* alt_name);

    /**
     * @brief required_options
     * @return int number of required options that are missing from passed arguments.
     */
    int required_options();

    /**
     * @brief index_accessed
     * @param a_name const char* the long name of the argument.
     * @return int 0 if argument is not yet parsed.
     */
    int index_accessed(const char* a_name);

    /**
     * @brief f_output_line formats the short, long and description of options in a line.
     * @param n_opt std::string short option.
     * @param a_opt std::string long option.
     * @param data std::string option description.
     * @return std::string the formated line.
     */
    std::string f_output_line(std::string n_opt, std::string a_opt, std::string data);

    /**
     * @brief padding adding spaces.
     * @param data std::string data to prepend before adding spaces.
     * @param num  int number of spaces to add.
     * @return std::string spaces as a string.
     */
    std::string padding(std::string data, int num);

    /**
     * @brief get_opt_len calculate the length of the longest option.
     */
    void get_opt_len();

    /**
     * @brief format_description format option description if too long.
     * @param data std::string option description.
     * @return int 0 if succeded
     */
    int format_description(std::string data);

    /**
     * @brief current_group current option group name.
     */
    std::string current_group;

    /**
     * @brief my_options store the options in a map using the goup name as key.
     */
    std::map<std::string, std::vector<option_t>> my_options;

    /**
     * @brief accessed_index list of options already parsed.
     */
    std::list<std::string> accessed_index;

    int argc; // Argument count.

    char** argv; // argument list.

    std::string help_opt[2]; // hold the short and long options.

    std::string header; // Header information.

    std::string footer; // Footer information.

    std::string usage_info; // How to use app information

    size_t max_opt_len = 0;

    size_t max_alt_opt_len = 0;

    size_t opt_len = 0;
};


#endif /* _ARGPARSER_H */

#ifndef _ARGPARSER_H
#define _ARGPARSER_H

#include <string>
#include <iostream>
#include <string.h>
#include <vector>

const int LINE_LENGTH = 90;


enum arg_type{
    A_INT    = 0,
    A_FLOAT  = 1,
    A_DOUBLE = 2,
    A_STRING = 3,
    A_CHAR   = 4,
    A_BOOL   = 5,
    A_NONE   = 6

};

struct option_t
{
        std::string opt_name;
        std::string opt_alt_name;
        bool* is_parsed;
        arg_type opt_type;
        void *opt_ref;
        std::string opt_desc;
};

class argparser{
public:
    argparser(int argc, char **argv, const char* header_desc  = "", const char* footer_desc = "");

    ~argparser();
    
    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc);

    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    arg_type type, void* ref, const char* desc);

    int parse();

    void set_usage_info(std::string info);

private:
    void show_help(std::string message = "");

    void add_help_option();

    int check_option(const char* opt);

    int index_accessed(int index);

    std::string f_output_line(std::string n_opt, std::string a_opt, std::string data);

    std::string padding(std::string data, int num);

    void get_opt_len();

    std::vector<option_t> options;

    std::vector<int> accessed_index;

    int argc;

    char** argv;

    std::string header;

    std::string footer;

    std::string usage_info;

    int max_opt_len = 0;

    int max_alt_opt_len = 0;

    int opt_len = 0;
};


#endif /* _ARGPARSER_H */

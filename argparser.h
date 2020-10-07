#ifndef _ARGPARSER_H
#define _ARGPARSER_H

#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <list>
#include <map>

const int LINE_LENGTH = 100;


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
    bool is_required;
    std::string opt_name;
    std::string opt_alt_name;
    bool* is_parsed;
    arg_type opt_type;
    void *opt_ref;
    std::string opt_desc;
};

class argparser{
public:
    argparser(int argc, char **argv);

    ~argparser();
    
    void add_option_group(const char* opt_group_dec = "");

    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc, bool is_required);

    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    arg_type type, void* ref, const char* desc,  bool is_required);

    int parse();

    void set_usage_info(std::string info);

    void set_header_info(std::string info);

    void set_footer_info(std::string info);

private:
    struct opt_check{
        
        void operator=(opt_check &tmp)
        {
            this->group = tmp.group;
            this->index = tmp.index;
        }

        std::string group;
        int index;
    } ;

    void show_help(std::string message = "");

    void add_help_option();

    opt_check check_option(const char* opt);

    int check_duplicate(const char* a_name, const char* alt_name);

    int required_options();

    int index_accessed(const char* a_name);

    std::string f_output_line(std::string n_opt, std::string a_opt, std::string data);

    std::string padding(std::string data, int num);

    void get_opt_len();

    int format_description(std::string data);

    std::string current_group;

    std::map<std::string, std::vector<option_t>> my_options;

    std::list<std::string> accessed_index;

    int argc;

    char** argv;

    std::string help_opt[2];

    std::string header;

    std::string footer;

    std::string usage_info;

    size_t max_opt_len = 0;

    size_t max_alt_opt_len = 0;

    size_t opt_len = 0;
};


#endif /* _ARGPARSER_H */

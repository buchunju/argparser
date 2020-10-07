#include "argparser.h"

argparser::argparser(int argc, char **argv)
{
    this->argc   = argc;
    this->argv   = argv;

    this->current_group = "General Options.";
    this->usage_info    = "";
    this->header        = "Help menu.";
    this->footer        = "(c) 2020 ";
}

argparser::~argparser()
{

}


int argparser::check_duplicate(const char* a_name, const char* alt_name)
{
    bool duplicate = false;

    typedef std::map<std::string, std::vector<option_t> > opt_map;

    for(opt_map::iterator sn = this->my_options.begin() ; sn != this->my_options.end() ; sn++)
    {
        for(std::vector<option_t>::iterator itr = sn->second.begin() ;
        itr != sn->second.end() ; itr++)
        {
            if((itr->opt_alt_name == alt_name) || (itr->opt_name == a_name) )
            {
                duplicate = true;
                break;
            }
        }
    }
    
    if(duplicate)
    {
        std::cerr << "[W] Duplicate option added." << std::endl;
        return 1;
    }

    return 0;
}
void argparser::add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc , bool is_required)
{
    if(this->my_options.size() == 0) add_option_group();

    if(check_duplicate(a_name, alt_name))
        return;

    //if h is used and option is not help
    if(!strcmp(alt_name, "-h") && strcmp(a_name, "--help")) 
    { 
        std::cerr << "Short option -h is reserved for help." << std::endl;
        return;
    }
    
    option_t opt;
    opt.opt_name     = a_name;
    opt.opt_alt_name = alt_name;
    opt.is_parsed    = is_parsed;
    opt.opt_desc     = desc;
    opt.opt_type     = A_NONE;
    opt.opt_ref      = NULL;
    opt.is_required  = is_required;

    this->my_options.at(this->current_group).push_back(opt);
}

void argparser::add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                arg_type type, void* ref, const char* desc , bool is_required)
{
    if(this->my_options.size() == 0) add_option_group();

    if(check_duplicate(a_name, alt_name))
        return;

    //if h is used and option is not help
    if(!strcmp(alt_name, "-h") && strcmp(a_name, "--help")) 
    { 
        std::cerr << "Short option -h is reserved for help." << std::endl;
        return;
    }

    option_t opt;
    opt.opt_name     = a_name;
    opt.opt_alt_name = alt_name;
    opt.is_parsed    = is_parsed;
    opt.opt_desc     = desc;
    opt.opt_type     = type;
    opt.opt_ref      = ref;
    opt.is_required  = is_required;

    this->my_options.at(this->current_group).push_back(opt);
}


int argparser::required_options()
{
    int num = 0;

    typedef std::map<std::string, std::vector<option_t> > opt_map;

    for(opt_map::iterator sn = this->my_options.begin() ; sn != this->my_options.end() ; sn++)
    {
        for(std::vector<option_t>::iterator itr = sn->second.begin() ;
        itr != sn->second.end() ; itr++)
        {
            if(itr->is_required)
            {
                num += 1;
                int check = num;
                std::string tmp = "-" + itr->opt_alt_name;
                std::string tmp1 = "--" + itr->opt_name;

                for(int n = 1; n < this->argc;n++)
                {
                    if(!strcmp(tmp.c_str(), this->argv[n]) || 
                       !strcmp(tmp1.c_str(), this->argv[n]))
                    {
                        num -= 1;
                        break;
                    }
                }

                if(check == num)
                    std::cerr << "[E] Missing argument: --" << itr->opt_name <<  " or -" << itr->opt_alt_name
                              << " .\n";
            }
        }
    }

    return num;
}

int argparser::parse()
{
    add_help_option();

    get_opt_len();

    if(argc > 1)
    {
        for(int n = 1; n < argc; n++)
        {
            if(!strcmp(argv[n],help_opt[0].c_str()) || !strcmp(argv[n], help_opt[1].c_str()))
            {
                show_help();
            }
        }
        for(int count = 1 ; count < argc ; count++)
        {
            if(argv[count][0] == '-')
            {
                opt_check ret = check_option(argv[count]);

                if(ret.group != "")
                {
                    if(index_accessed(ret.group.c_str()))
                    {
                        show_help("Already parsed option: " + std::string(argv[count]));
                    }

                    *this->my_options.at(ret.group).at(ret.index).is_parsed = true;
                    if(this->my_options.at(ret.group).at(ret.index).opt_type != A_NONE)
                    {
                        ++count;
                        if(count != argc)
                        {
                            std::string possible_boolean[2][6] = {"Y","y", "1","true","True","TRUE",
                                                                  "N","n","0","false","False","FALSE"};

                            int a;
                            double b;
                            float c;
                            std::string d;
                            bool e;
                            char f;

                            switch (this->my_options.at(ret.group).at(ret.index).opt_type)
                            {
                            case A_INT:
                                a = std::stoi(argv[count]);
                                memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &a, sizeof(a));
                                break;
                            case A_FLOAT:
                                c = std::stof(argv[count]);
                                memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &c, sizeof(c));;
                                break;
                            case A_DOUBLE:
                                b = std::stod(argv[count]);
                                memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &b, sizeof(b));
                                break;
                            case A_CHAR:
                                f = argv[count][0];
                                memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &f, sizeof(f));
                                break;
                            case A_STRING:
                                d = std::string(argv[count]);
                                memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &d, sizeof(d));
                                break;
                            case A_BOOL:
                                if(possible_boolean[0]->find(argv[count]) != std::string::npos)
                                {
                                    e = true;
                                    memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &e, sizeof(e));
                                }
                                else if(possible_boolean[1]->find(argv[count]) != std::string::npos)
                                {
                                    e = false;
                                    memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &e, sizeof(e));
                                }
                                else
                                {
                                    show_help("Unkwown boolean value: " + std::string(argv[count]));
                                }
                                break; 
                            case A_NONE:
                                /* unlikely to reach here */
                                break; 
                            default:
                                a = std::stoi(argv[count]);
                                memcpy(this->my_options.at(ret.group).at(ret.index).opt_ref , &a, sizeof(a));
                                break;
                            }
                        }
                        else
                        {
                            show_help("Missing argument for option: " + std::string(argv[count - 1]));
                        }
                    }
                }
                else
                {
                    show_help("Unknown option: " + std::string(argv[count]));
                }
            }
            else
            {
                show_help("Unknown option: " + std::string(argv[count]));
            }
        }
    }
    else if(required_options())
    {
        show_help("You must include the missing arguments.");
    }
    else
    {
        show_help();
    }
    return 0;
    
}


void argparser::show_help(std::string message)
{
    std::string appname = this->argv[0];
    std::string help_buff;

    if(message.length() != 0)
    {
        help_buff = message + "\n\n";
    }
    
    help_buff.append("Usage: " + appname + " [options] ...\n\n"
                     + usage_info + "\n\n");


    if(this->header.length() != 0)
    {
        help_buff.append(this->header + "\n");
    }

    if(this->my_options.at(this->current_group).size() != 0)
    {
        typedef std::map<std::string, std::vector<option_t> > opt_map;

        for(opt_map::iterator sn = this->my_options.begin() ; sn != this->my_options.end() ; sn++)
        {
            help_buff.append( "\n" + sn->first + " \n");

            for(std::vector<option_t>::iterator itr = sn->second.begin() ;
            itr != sn->second.end() ; itr++)
            {
                help_buff.append(f_output_line(itr->opt_name, itr->opt_alt_name, itr->opt_desc));
            }
        }
    }

    if(this->footer.length() != 0)
    {
        help_buff.append( "\n\n" + this->footer + "\n");
    }

    std::cout << help_buff << std::endl;

    exit(1);

}

argparser::opt_check argparser::check_option(const char* opt)
{
    opt_check temp = {
        .group = "",
        .index = 0
    };
    
    typedef std::map<std::string, std::vector<option_t> > opt_map;

    for(opt_map::iterator sn = this->my_options.begin() ; sn != this->my_options.end() ; sn++)
    {
        temp.group = sn->first;
        temp.index = 0;

        for(std::vector<option_t>::iterator itr = sn->second.begin() ;
        itr != sn->second.end() ; itr++)
        {
            if((("--" + itr->opt_name) == opt) || (("-" + itr->opt_alt_name) == opt))
            {
                return temp;
            }
            temp.index += 1;
        }
    }

    temp.group = "";
    temp.index = 0;
    return temp;
}

int argparser::index_accessed(const char* a_name)
{
    for(std::list<std::string>::iterator itr = accessed_index.begin(); itr != accessed_index.end(); itr++)
    {
        if(*itr == a_name)
        {
            return 1;
        }
    }
    accessed_index.push_back(a_name);
    return 0;
}

void argparser::get_opt_len()
{
    typedef std::map<std::string, std::vector<option_t> > opt_map;

    for(opt_map::iterator sn = this->my_options.begin() ; sn != this->my_options.end() ; sn++)
    {
        for(std::vector<option_t>::iterator itr = sn->second.begin() ;
        itr != sn->second.end() ; itr++)
        {
            if(itr->opt_alt_name.length() > max_alt_opt_len)
		    {
			    max_alt_opt_len = (int)itr->opt_alt_name.length();
		    }
		    if(itr->opt_name.length() > max_alt_opt_len)
		    {
		    	max_opt_len =  (int)itr->opt_name.length();
		    }
        }
    }

    opt_len = 6 + max_opt_len + 3 + max_alt_opt_len + 4;
}

std::string argparser::f_output_line(std::string n_opt, std::string a_opt, std::string data)
{
	std::string tmp;
	tmp = padding("",3);
    tmp.append("-");
	tmp.append(padding(a_opt, max_alt_opt_len - a_opt.length() + 3));
    tmp.append("--");
	tmp.append(padding(n_opt, (max_opt_len + 4) - n_opt.length()));

    if((opt_len + data.length()) < LINE_LENGTH)
    {
        tmp.append(data);
        tmp.append("\n");
    }
    else
    {
        size_t data_len = LINE_LENGTH - opt_len;

        while(1)
        {
            if(data.length() > data_len)
            {
                for (size_t i = data_len; i > 0 ; i--)
                {
                    if(data[i] == ' ')
                    {
                        tmp.append(data.substr(0, i));
                        data = data.substr(i,data.size());
                        tmp.append(padding("\n",opt_len));
                        break;
                    }
                }
            }
            else
            {
                tmp.append(data);
                break;
            }
                    
        }
    }

    return tmp;
}

std::string argparser::padding(std::string data, int num)
{
	std::string tmp = data;
	for(int n = 0; n < num; n++ )
		tmp.append(" ");
	return tmp;
}

void argparser::add_help_option()
{
    if(!check_duplicate("help","h")) // Help option not defined.
    {
        option_t opt;
        opt.opt_alt_name = "h";        
        opt.opt_name = "help";
        opt.opt_desc = "Print the help menu.";
        opt.opt_type = A_NONE;
        opt.is_required = false;

        this->help_opt[0] = "-"  + opt.opt_alt_name;
        this->help_opt[1] = "--" + opt.opt_name;

        this->my_options.begin()->second.push_back(opt);
    }
}

void argparser::add_option_group(const char* opt_group_dec)
{
    if(strlen(opt_group_dec) != 0) 
        this->current_group = opt_group_dec;

    if(this->my_options.count(opt_group_dec) == 0)
    {
        std::pair<std::string, std::vector<option_t>> m_pair;
        
        std::vector<option_t> n_options;

        m_pair.first  = opt_group_dec;
        m_pair.second = n_options;

        this->my_options.insert(m_pair);
    }

}

void argparser::set_usage_info(std::string info)
{
    this->usage_info = info;
}
void argparser::set_header_info(std::string info)
{
    this->header = info;
}
void argparser::set_footer_info(std::string info)
{
    this->footer = info;
}
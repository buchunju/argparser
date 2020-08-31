#include "argparser.h"

argparser::argparser(int argc, char **argv, const char* header_desc, const char* footer_desc)
{
    this->argc   = argc;
    this->argv   = argv;
    this->header = header_desc;
    this->footer = footer_desc;
}

argparser::~argparser()
{

}

void argparser::add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc)
{
    bool duplicate = false;
    for(std::vector<option_t>::iterator itr = options.begin() ; itr != options.end() ; itr++)
    {
        if((itr->opt_alt_name == alt_name) || (itr->opt_name == a_name) )
        {
            duplicate = true;
            break;
        }
    }
    if(duplicate)
    {
        std::cout << "Duplicate option added." << std::endl;
        exit(1);
    }
    option_t opt;
    opt.opt_name     = a_name;
    opt.opt_alt_name = alt_name;
    opt.is_parsed    = is_parsed;
    opt.opt_desc     = desc;
    opt.opt_type     = A_NONE;
    opt.opt_ref      = NULL;

    options.push_back(opt);
}

void argparser::add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                arg_type type, void* ref, const char* desc)
{
    option_t opt;
    opt.opt_name     = a_name;
    opt.opt_alt_name = alt_name;
    opt.is_parsed    = is_parsed;
    opt.opt_desc     = desc;
    opt.opt_type     = type;
    opt.opt_ref      = ref;

    options.push_back(opt);
}

int argparser::parse()
{
    add_help_option();
    get_opt_len();
    if(argc > 1)
    {
        for(int n = 1; n < argc; n++)
        {
            if(!strcmp(argv[n],"-h") || !strcmp(argv[n], "--help"))
            {
                show_help();
            }
        }
        for(int count = 1 ; count < argc ; count++)
        {
            if(argv[count][0] == '-')
            {
                int ret = check_option(argv[count]);
                if(ret)
                {
                    --ret;
                    if(index_accessed(ret))
                    {
                        show_help("Already parsed option: " + std::string(argv[count]));
                    }
                    *options.at(ret).is_parsed = true;
                    if(options.at(ret).opt_type != A_NONE)
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

                            switch (options.at(ret).opt_type)
                            {
                            case A_INT:
                                a = std::stoi(argv[count]);
                                memcpy(options.at(ret).opt_ref , &a, sizeof(a));
                                break;
                            case A_FLOAT:
                                c = std::stof(argv[count]);
                                memcpy(options.at(ret).opt_ref , &c, sizeof(c));;
                                break;
                            case A_DOUBLE:
                                b = std::stod(argv[count]);
                                memcpy(options.at(ret).opt_ref , &b, sizeof(b));
                                break;
                            case A_CHAR:
                                f = argv[count][0];
                                memcpy(options.at(ret).opt_ref , &f, sizeof(f));
                                break;
                            case A_STRING:
                                d = std::string(argv[count]);
                                memcpy(options.at(ret).opt_ref , &d, sizeof(d));
                                break;
                            case A_BOOL:
                                if(possible_boolean[0]->find(argv[count]) != std::string::npos)
                                {
                                    e = true;
                                    memcpy(options.at(ret).opt_ref , &e, sizeof(e));
                                }
                                else if(possible_boolean[1]->find(argv[count]) != std::string::npos)
                                {
                                    e = false;
                                    memcpy(options.at(ret).opt_ref , &e, sizeof(e));
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
                                memcpy(options.at(ret).opt_ref , &a, sizeof(a));
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
        help_buff = message + '\n';
    }
    if(usage_info.length() == 0)
    {
        help_buff.append("Usage: " + appname + " [options] ...\n\n");
    }
    else
    {
        help_buff.append(usage_info + "\n");
    }
    

    if(this->header.length() != 0)
    {
        help_buff.append(this->header + "\n");
    }

    if(options.size() != 0)
    {
        help_buff.append("Options: \n");
        for(std::vector<option_t>::iterator itr = options.begin() ; itr != options.end() ; itr++)
        {
            help_buff.append(f_output_line(itr->opt_name, itr->opt_alt_name, itr->opt_desc));
        }
    }

    if(this->footer.length() != 0)
    {
        help_buff.append( "\n" + this->footer + "\n");
    }
    std::cout << help_buff << std::endl;

    exit(1);

}

int argparser::check_option(const char* opt)
{
    int index = 0;
    for(std::vector<option_t>::iterator itr = options.begin(); itr != options.end() ; itr++)
    {
        if((("--" + itr->opt_name) == opt) || (("-" + itr->opt_alt_name) == opt))
        {
            return index + 1;
        }
        index += 1;
    }
    return 0;
}

int argparser::index_accessed(int index)
{
    for(std::vector<int>::iterator itr = accessed_index.begin(); itr != accessed_index.end(); itr++)
    {
        if(*itr == index)
        {
            return 1;
        }
    }
    accessed_index.push_back(index);
    return 0;
}

void argparser::get_opt_len()
{
	for(std::vector<option_t>::iterator itr = options.begin() ; itr != options.end(); itr++)
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
        int extra = (opt_len + data.length()) - LINE_LENGTH;
        int base = 0;
        int d_len = LINE_LENGTH - opt_len;
        while(1)
        {
            if(extra <= 0)
            {
                if(base != 0)
                    tmp.append(padding(" ",opt_len - 1));
                tmp.append(data.substr(base));
                tmp.append("\n");
                break;
            }
            else
            {
                if(base != 0)
                    tmp.append(padding(" ",opt_len - 1));
                tmp.append(data.substr(base, d_len));
                tmp.append("\n");
                base = base + d_len;
                extra = extra - d_len;
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
    bool found = false;
    for(std::vector<option_t>::iterator itr = options.begin() ; itr != options.end() ; itr++)
    {
        if((itr->opt_alt_name == "h") || (itr->opt_name == "help"))
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        option_t opt;
        opt.opt_alt_name = "h";
        opt.opt_name = "help";
        opt.opt_desc = "Print the help menu.";
        opt.opt_type = A_NONE;

        options.push_back(opt);
    }
}
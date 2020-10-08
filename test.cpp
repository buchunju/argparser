#include "argparser.h"

int main(int argc, char* argv[])
{
    bool hello_parsed = false;
    bool age_parsed = false;
    bool long_parsed = false;
    bool ken_parsed = false;

    int m_age;

    argparser m_argparse(argc, argv);

    m_argparse.set_usage_info("./test [option] <value> ..");

    m_argparse.set_header_info("Simple program v0.0.1.");
    m_argparse.set_footer_info("Developed by kenneth buchunju\n"
                               "(C) 2020 <buchunjukenneth@gmail.com>.");

    m_argparse.add_option_group("General options.");
    m_argparse.add_option("hello","hl",&hello_parsed, "A simple hello greeting", false);

    m_argparse.add_option_group("Haha options.");
    m_argparse.add_option("age", "a", &age_parsed, A_INT, (void*)&m_age, "Age of the user",true); 
    m_argparse.add_option("long","l",&long_parsed,
                        "This is a very long line to test the length of the option description. "
                        "Just for testing purposes. It can be as long as possible",false);


    m_argparse.add_option_group("Hehe options.");
    m_argparse.add_option("ken","k", &ken_parsed, "Doing something.", false);

    m_argparse.parse();

    if(hello_parsed)
    {
        /* Say hello */
        std::cout << "Hello" << std::endl;
    }

    if(age_parsed)
    {
	    std::cout << "Your age is: " << m_age << std::endl;
    }

    if(ken_parsed)
    {
        std::cout << "Ken was here." <<  std::endl;
    }

    if(long_parsed)
    {
        std::cout << "mmmmh. Am speechless" << std::endl;
    }

    return 0;
}

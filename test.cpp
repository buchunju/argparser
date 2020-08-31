#include "argparser.h"

int main(int argc, char* argv[])
{
    bool hello_parsed = false;
    bool age_parsed = false;
    bool long_parsed = false;

    int m_age;

    argparser m_argparse(argc, argv,"","Simple Hello v0.0.1");

    m_argparse.add_option("hello","hl",&hello_parsed, "A simple hello greeting");
    m_argparse.add_option("age", "a", &age_parsed, A_INT, (void*)&m_age, "Age of the user"); 
    m_argparse.add_option("long","l",&long_parsed,"This is a very long line to test the length of the option description. Just for testing purposes. It can be as long as possible");

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

    return 0;
}

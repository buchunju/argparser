argparser v0.0.1
================

A simple and small c++ argument parser.

API Summary
-----------

### Constructor ###
Takes argc and argv as arguments.
```c++
argparser::argparser(int argc, char** argv);
```
### Argument Value type ###

```c++
enum arg_type{
    A_INT    = 0,
    A_FLOAT  = 1,
    A_DOUBLE = 2,
    A_STRING = 3,
    A_CHAR   = 4,
    A_BOOL   = 5,
    A_NONE   = 6

};
```

### Add_option ###
Takes a_name as long name, alt_name as short name, desc as option description
is_parsed reference to where to store a boolean true if the argument has been 
passed though cmd, 
is_required is true if option is required.
```c++
    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc, bool is_required);
```
Argument type for the type of value parsed. 
Argument ref reference to where to store the value.
```c++
        void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    arg_type type, void* ref, const char* desc,  bool is_required);
```

Testing.
-------

### Compiling the test file ###
    make 


Example.
--------
```c++
    bool age_parsed   = false;
    bool hello_parsed = false;
    
    int m_age;
    
    argparser m_argparse(argc, argv);

    m_argparse.set_header_info("Simple program v0.0.1.");
    
    m_argparse.set_footer_info("Developed by kenneth buchunju\n"
                               "(C) 2020 <buchunjukenneth@gmail.com>.");

    m_argparse.add_option_group("General options.");
    m_argparse.add_option("hello","hl",&hello_parsed, "A simple hello greeting", false);

    m_argparse.add_option_group("Misc options.");
    m_argparse.add_option("age", "a", &age_parsed, A_INT, (void*)&m_age, "Age of the user",true); 


    m_argparse.parse();
    
    if(age_parsed)
    {
        std::cout << "Your age is: " << m_age << std::endl;
    }
    
    if(hello_parsed)
    {
        std::cout << "Hello there" << std::endl;
    }

```

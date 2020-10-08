argparser v0.0.1
================

> A simple and small c++ argument parser.

***

API Summary
-----------

### Constructor ###
Takes ___argc___ and ___argv___ as arguments.
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
Takes ___a_name___ as _long name_, ___alt_name___ as _short name_, ___desc___ as _option description_, 
___is_parsed___ reference to where to _store a boolean_ true if the argument has been 
passed though cmd, ___is_required___ is true if _option is required_.
```c++
    void add_option(const char* a_name, const char* alt_name, bool *is_parsed, 
                    const char* desc, bool is_required);
```
Argument ___type___ for the type of value parsed. 
Argument ___ref___ reference to where to store the value.
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

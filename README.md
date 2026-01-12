# argparser

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)

A lightweight, header-based C++ argument parser designed for simplicity and ease of use.

---

## Requirements

- **C++17 or later**: This library uses template features (`if constexpr` and `std::is_same_v`) introduced in C++17.

---

## Features

- **Simple API**: Easy to integrate and use.
- **Type Safety**: Uses C++ templates for type-safe argument parsing.
- **Short & Long Options**: Supports both `--long-name` and `-s` (short) formats.
- **Option Groups**: Organize options for better help message layout.
- **Required Options**: Mark specific arguments as mandatory.
- **Custom Help**: Automatically generates a clean help menu with customizable header and footer.

---

## Quick Start

### Basic Usage

```cpp
#include "argparser.h"
#include <iostream>

int main(int argc, char** argv) {
    bool hello_parsed = false;
    int age = 0;

    argparser parser(argc, argv);
    parser.set_header_info("My Awesome App v1.0");

    // Add a flag (no value)
    parser.add_option("hello", "h", &hello_parsed, "Print a greeting");

    // Add a value option
    parser.add_option("age", "a", &age, "user age", true);

    if (parser.parse() != 0) return 1;

    if (hello_parsed) std::cout << "Hello!" << std::endl;
    std::cout << "Age: " << age << std::endl;

    return 0;
}
```

---

## Detailed Type Support

The `add_option` template supports the following types and parsing rules:

| Type | Parsing Logic |
| :--- | :--- |
| `int`, `float`, `double` | Parsed using `std::stoi`, `std::stof`, or `std::stod`. |
| `std::string` | Taken as-is. |
| `char` | Takes the first character of the provided value. |
| `bool` | Parsed as `true` if the value is: `true`, `True`, `1`, `y`, or `Y`. |

---

## Advanced Customization

### Option Groups

You can group related options together to make the help menu more readable:

```cpp
parser.add_option_group("File Options");
parser.add_option("input", "i", &input_file, "Path to input file");
parser.add_option("output", "o", &output_file, "Path to output file");

parser.add_option_group("Debug Options");
parser.add_option("verbose", "v", &is_verbose, "Enable verbose logging");
```

### Help Text

Customize the info displayed when `--help` or `-h` is called:

```cpp
parser.set_header_info("Program Name v1.0.0 - A brief description.");
parser.set_usage_info("Usage: my_app [options] <arguments>");
parser.set_footer_info("For more information, visit: https://github.com/your/repo");
```

---

## API Summary

### Constructor
```cpp
argparser::argparser(int argc, char** argv);
```

### Adding Options

#### Flags (Options without values)
```cpp
void add_option(const char* long_name, 
                const char* short_name, 
                bool* is_parsed, 
                const char *description, 
                bool is_required = false);
```

#### Value Options
```cpp
template<typename T>
void add_option(const char* long_name, 
                const char* short_name, 
                bool* is_parsed, 
                T* reference, 
                const char* description, 
                bool is_required = false);
```

### Parsing
```cpp
int parse(); // Returns 0 on success, prints errors to cerr and exits on failure.
```

---

## Building and Testing

The project includes a `Makefile` for easy compilation of the test utility.

```bash
make
./test --help
```

---

## License

This project is licensed under the MIT License - see the source files for details.
Copyright (c) 2020 Kenneth Buchunju.


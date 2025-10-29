#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // For system function
#include <cstdio>
#include <cstring>
#include <regex>    // For regex operations

using namespace std;

int main(int argc, char **argv) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input_filename> <output_filename_1> <output_filename_2>" << endl;
        return 1;
    }

    ifstream src_ifs(argv[1]);
    if (!src_ifs.is_open()) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }

    ostringstream input_stream;
    string line;
    while (getline(src_ifs, line)) {
        input_stream << line << '\n';
    }

    string input = input_stream.str();

    // Remove newline before `else` keyword
    regex else_pattern(R"(\}\s*\n\s*else)");
    input = regex_replace(input, else_pattern, "} else");

    // Remove newline before semicolon after struct definition
    regex struct_pattern(R"(\}\s*\n\s*;)");
    input = regex_replace(input, struct_pattern, "};");

    // Regex pattern to find labels
    regex label_pattern(R"((label_\w+) :)");
    string modified_input;
    smatch match;

    // Iterating through the matches
    auto search_start = input.cbegin();
    while (regex_search(search_start, input.cend(), match, label_pattern)) {
        // Append the text before the match
        modified_input.append(search_start, match[0].first);

        // Append the matched label and the goto statement
        string label_name = match[1].str();
        modified_input.append(match[0].first, match[0].second);
        modified_input += "    if false { goto " + label_name + "; }\n";

        // Move the search start forward
        search_start = match[0].second;
    }
    // Append the remaining part of the input
    modified_input.append(search_start, input.cend());

    // The set_var function definition to be inserted
    string set_var = "func set_var(gi int, gf float64){\n"
                     "    global_int = gi;\n"
                     "    global_float = gf;\n"
                     "}\n";

    // The call to set_var to be inserted before return 0;
    string call_set_var = "    set_var(local_int, local_float);\n";

    // The equivalent Go main function to be added
    string main_tail_source = "func main(){\n"
                              "    func1();\n"
                              "}";

    // Insert the set_var function before func func1()
    size_t pos_func1 = modified_input.find("func func1");
    if (pos_func1 != string::npos) {
        modified_input.insert(pos_func1, set_var + "\n");
    }

    // Insert the call to set_var before return 0; in func func1()
    size_t pos_return = modified_input.find("return", pos_func1);
    if (pos_return != string::npos) {
        modified_input.insert(pos_return, call_set_var);
    }

    ofstream checked_src(argv[2]);
    if (!checked_src.is_open()) {
        cerr << "Error: Could not open file " << argv[2] << endl;
        return 1;
    }

    checked_src << modified_input + "\n" + main_tail_source;
    checked_src.close();

    // Add the equivalent Go main function with output and write it to the third file
    string main_tail = "func main(){\n"
                       "    func1();\n"
                       "    fmt.Printf(\"%d:%.2f\\n\", global_int, global_float);\n"
                       "}";

    // Find the position of the "package main;" line and insert the import statement after it
    size_t pos_package = modified_input.find("package main ;");
    if (pos_package != string::npos) {
        pos_package += strlen("package main ;");
        modified_input.insert(pos_package, "\nimport \"fmt\"\n");
    }

    ofstream main_src(argv[3]);
    if (!main_src.is_open()) {
        cerr << "Error: Could not open file " << argv[3] << endl;
        return 1;
    }

    main_src << modified_input + "\n" + main_tail;
    main_src.close();

    string timeout_duration = "30";
    string build_command = "timeout " + timeout_duration + " go build " + string(argv[3]) + " > /dev/null 2>&1";
    int build_result = system(build_command.c_str());

    // Check if the build was successful
    if (build_result != 0) {
        cout << "Build Fails" << endl;
        return 1; // Return 1 if the build failed
    }

    return 0;
}

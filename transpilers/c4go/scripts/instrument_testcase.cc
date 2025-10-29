#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> // Include for system function
#include <cstdio>
#include <cstring>

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
    
    string set_var = "void set_var(int gi, float gf, char gc){\n"
                     "    global_int = gi;\n"
                     "    global_float = gf;\n"
                     "    global_char = gc;\n"
                     "}\n";

    string call_set_var = "    set_var(local_int, local_float, local_char);\n";

    size_t func_pos = input.find("int func1");
    if (func_pos != string::npos) {
        input.insert(func_pos, set_var + "\n");
    }


    size_t return_pos = input.find("return", func_pos);
    if (return_pos != string::npos) {
        input.insert(return_pos, call_set_var);
    }

    ofstream checked_src(argv[2]);
    if (!checked_src.is_open()) {
        cerr << "Error: Could not open file " << argv[2] << endl;
        return 1;
    }

    string main_tail_source = "int main(){\n"
                       "    func1();\n"
                       "    return 0;\n"
                       "}";

    checked_src << input + "\n"+ main_tail_source;
    checked_src.close(); 

    string main_tail = "int main(){\n"
                       "    func1();\n"
                       "    printf(\"%d:%.2f:%d\\n\", global_int, global_float, (int)global_char);\n"
                       "    return 0;\n"
                       "}";

    input += "\n" + main_tail;

    ofstream main_src(argv[3]);
    if (!main_src.is_open()) {
        cerr << "Error: Could not open file " << argv[3] << endl;
        return 1;
    }

    string header = "#include <stdio.h>\n";

    main_src << header << input;
    main_src.close(); // Ensure the file is closed

    // Debug: Print the final content of the generated C code
    //cout << "Generated C code:" << endl;
    //cout << header << input << endl;

     // Compile the generated C code
	string output_filename = string(argv[3]);
    size_t last_dot = output_filename.find_last_of(".");
    string base_filename = output_filename.substr(0, last_dot);
    string output_executable = base_filename + ".out";

    string timeout_duration = "30";

    string compile_command = "timeout "+ timeout_duration +" clang -fsanitize=undefined " + string(argv[3]) + " -o " + output_executable  + " > /dev/null 2>&1";
    int compile_result = system(compile_command.c_str());

    // Check if the compilation was successful
    if (compile_result != 0) {
        //cerr << "==>>Compilation failed." << endl;
        return 1;
    }  


    
    string run_command = "timeout " + timeout_duration+ " ./"+output_executable + " 2>&1 "; // Redirect stderr to stdout
    //cout<<run_command<<endl;
        FILE* pipe = popen(run_command.c_str(), "r");
        if (!pipe) {
            //cerr << "Failed to run command." << endl;
            return 1;
        }

 char buffer[128];
    bool runtime_error_found = false;

    // Read the output of the command   strstr(buffer, "stack-overflow")
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        if ((strstr(buffer, "runtime error:") != nullptr) || (strstr(buffer, "stack-overflow") != nullptr)) {
            runtime_error_found = true;
            break; // Stop reading further if "runtime error:" is found
        }
    }

    // Get the return status of the process after it finishes or times out
    int return_status = pclose(pipe);

    // If the return status indicates a timeout (usually 124 for timeout command)
    if (WEXITSTATUS(return_status) == 124) {
        //cerr << "Execution failed: process timed out." << endl;
        return 1;
    }

    // Check if "runtime error:" was found in the output
    if (runtime_error_found) {
        //cerr << "Execution failed: runtime/stack-overflow error detected." << endl;
        return 1;
    }



    return 0;
}

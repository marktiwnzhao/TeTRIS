#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <mutated.hx> <Runner.hx> " << endl;
        return 1;
    }

    string mutatedFile = argv[1];
    string runnerFile = argv[2];

    // Read the mutated.hx file
    ifstream inFile(mutatedFile);
    if (!inFile) {
        cerr << "Error opening " << mutatedFile << endl;
        return 1;
    }

    string line;
    string modifiedCode;
    bool setVarDefined = false;
    bool inTestClass = false;
    bool inFunc1 = false;  // Track whether we're inside func1
    int openBraces = 0;    // To track nested braces inside func1
    string testClassContent;

    while (getline(inFile, line)) {
        // Look for the class Runner definition to start processing
        if (line.find("class Runner") != string::npos) {
            inTestClass = true;
        }

        if (inTestClass) {
            // Look for the start of func1
            if (line.find("function func1") != string::npos) {
                inFunc1 = true;
            }

            if (inFunc1) {
                // Count opening and closing braces to detect the end of func1
                if (line.find('{') != string::npos) openBraces++;
                if (line.find('}') != string::npos) openBraces--;

                // Look for the return statement and insert set_var before it
                if (line.find("return") != string::npos) {
                    testClassContent += "    set_var(local_int, local_float);\n";
                }

                // When func1's braces are balanced (openBraces == 0), we exit func1
                if (openBraces == 0) {
                    inFunc1 = false;
                }
            }

            testClassContent += line + "\n";
        } else {
            // For lines outside the test class, copy them directly
            modifiedCode += line + "\n";
        }

        // Define the set_var function globally if not already added
        if (!setVarDefined && line.find("class Runner") != string::npos) {
            modifiedCode += "function set_var(local_int:Int, local_float:Float) {\n";
            modifiedCode += "    global_int = local_int;\n";
            modifiedCode += "    global_float = local_float;\n";
            modifiedCode += "}\n\n";
            setVarDefined = true;
        }
    }
    inFile.close();

    // Insert the main function before the last closing brace of the Test class
    size_t lastBracePos = testClassContent.find_last_of('}');

    if (lastBracePos != string::npos) {
        testClassContent.insert(lastBracePos, "    public static function main() {\n        func1();\n    }\n");
    }

    // Write the modified content to runner.hx
    ofstream runnerFileOut(runnerFile);
    if (!runnerFileOut) {
        cerr << "Error creating or writing to " << runnerFile << endl;
        return 1;
    }
    runnerFileOut << modifiedCode + testClassContent;
    runnerFileOut.close();

    string timeout_duration = "40";

    string compile_command = "timeout "+ timeout_duration +" haxe --main Runner --interp " + " > /dev/null 2>&1";
    int compile_result = system(compile_command.c_str());

    // Check if the compilation was successful
    if (compile_result != 0) {
        //cerr << "==>>Compilation failed." << endl;
        string remove_command = "rm Runner.hx";
        system(remove_command.c_str());
        return 1;
    }  

    return 0;
}

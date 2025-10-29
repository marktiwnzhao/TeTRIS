#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Get the input file from the command line arguments
    char *input_file = argv[1];

    // Construct the command to run 'haxelib run go2hx <input_file>'
    char command[1024];
    snprintf(command, sizeof(command), "haxelib run go2hx %s", input_file);

    // Execute the command
    int result = system(command);

    // Return the result of the system command
    return result;
}

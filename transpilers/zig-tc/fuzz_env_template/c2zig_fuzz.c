#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Prepare the command to execute zig translate-c
    char command[256];
    snprintf(command, sizeof(command), "zig translate-c %s > runner.zig", argv[1]);

    // Execute the command
    int result = system(command);

    // Check if the command was successful
    if (result != 0) {
        fprintf(stderr, "Failed to run zig translate-c on %s\n", argv[1]);
        return 1;
    }

    return 0;
}

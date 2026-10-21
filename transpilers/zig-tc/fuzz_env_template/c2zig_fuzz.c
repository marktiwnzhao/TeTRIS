#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    /* Keep the translator in the AFL/QEMU process lineage.  The old
       implementation used system(), which made the wrapper the observed
       target and hid input-dependent translate-c coverage. */
    int output_fd = open("runner.zig", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (output_fd < 0) {
        perror("runner.zig");
        return 1;
    }
    if (dup2(output_fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(output_fd);
        return 1;
    }
    close(output_fd);

    char *const translate_argv[] = {
        (char *)"zig", (char *)"translate-c", argv[1],
        (char *)"-I", (char *)"/usr/include",
        (char *)"-I", (char *)"/usr/include/x86_64-linux-gnu",
        NULL
    };
    execvp("zig", translate_argv);
    perror("zig translate-c");
    return 1;
}

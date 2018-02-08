#pragma once

#define PANIC(msg) {\
            fprintf(stderr,\
                "----- PANIC -----| file: %s | line: %d | msg: %s\n", __FILE__, __LINE__,  msg);\
            getchar();\
            exit(EXIT_FAILURE);}\

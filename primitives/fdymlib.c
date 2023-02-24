#pragma GCC optimize("O0")

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <alloca.h>

enum {
    SIZEDOUBLE = 8,
    SIZESTR = 4,
    SIZEINT = 4,
};

int main(int argc, char *argv[]) {
    const char *libname = argv[1];
    const char *funcname = argv[2];
    const char *sign = argv[3];

    void *lib = dlopen(libname, RTLD_LAZY);
    if (lib == NULL) {
        fprintf(stderr, "cant open lib %s: %s", libname, dlerror());
        return EXIT_FAILURE;
    }
    void *func = dlsym(lib, funcname);
    if (func == NULL) {
        fprintf(stderr, "cant load func %s: %s", funcname, dlerror());
        return EXIT_FAILURE;
    }

    size_t signlen = strlen(sign);
    size_t signbytes = 0;
    for (size_t i = 1; i < signlen; ++i) {
        if (sign[i] == 'd') {
            signbytes += SIZEDOUBLE;
        } else if (sign[i] == 'i') {
            signbytes += SIZEINT;
        } else if (sign[i] == 's') {
            signbytes += SIZESTR;
        } else {
            fprintf(stderr, "undefined char in sign[%zu]: %c\n", i, sign[i]);
            return EXIT_FAILURE;
        }
    }

    char *stackbuf = (char *)alloca(signbytes);
    size_t stackpointer = 0;
    for (size_t i = 1; i < signlen; ++i) {
        if (sign[i] == 'd') {
            double curval;
            if (sscanf(argv[3 + i], "%lf", &curval) != 1) {
                fprintf(stderr, "cant scanf double: sign[%zu]\n", i);
                return EXIT_FAILURE;
            }
            *(double *)(stackbuf + stackpointer) = curval;
            stackpointer += SIZEDOUBLE;
        } else if (sign[i] == 'i') {
            int curval;
            if (sscanf(argv[3 + i], "%d", &curval) != 1) {
                fprintf(stderr, "cant scanf int: sign[%zu]\n", i);
                return EXIT_FAILURE;
            }
            *(int *)(stackbuf + stackpointer) = curval;
            stackpointer += SIZEINT;
        } else if (sign[i] == 's') {
            *(char **)(stackbuf + stackpointer) = argv[3 + i];
            stackpointer += SIZESTR;
        } else {
            fprintf(stderr, "undefined char in sign[%zu]: %c\n", i, sign[i]);
            return EXIT_FAILURE;
        }
    }

    char rettype = sign[0];
    if (rettype == 'v') {
        void (*funcprot)() = func;
        funcprot();
    } else if (rettype == 'i') {
        int (*funcprot)() = func;
        printf("%d\n", funcprot());
    } else if (rettype == 'd') {
        double (*funcprot)() = func;
        printf("%.10g\n", funcprot());
    } else if (rettype == 's') {
        char *(*funcprot)() = func;
        printf("%s\n", funcprot());
    } else {
        fprintf(stderr, "undefined char in sign[0]: %c\n", rettype);
        return EXIT_FAILURE;
    }
    return 0;
}
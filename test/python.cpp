//python.cpp
#include <stdio.h>
#include <stdlib.h>


int main( ){
        char *cmd = "ls";
        char buf[BUFSIZ];
        FILE *ptr;

        if ((ptr = popen(cmd, "r")) != NULL)
            while (fgets(buf, BUFSIZ, ptr) != NULL)
                        (void) printf("%s", buf);
                (void) pclose(ptr);
        return 0;
}
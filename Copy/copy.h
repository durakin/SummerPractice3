#ifndef COPY__COPY_H_
#define COPY__COPY_H_

//int copy_regular_file(int sourcefd, int destfd, int size);

#include <stdbool.h>
int copy(char *source_name, char *dest_name, bool rec, bool verbose);
#endif //COPY__COPY_H_

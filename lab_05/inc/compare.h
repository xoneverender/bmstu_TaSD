#ifndef COMPARE_H
#define COMPARE_H

#include <stdio.h>
#include <sys/time.h>
#include "errors.h"
#include "queue.h"

#define MEASURES_COUNT QUEUE_CAPACITY
#define STEP 100

err_code_e compare(void);

#endif /* COMPARE_H */

#include "ogs_internal.h"

#include <stdio.h>

FILE *_OGS_LOG_SINK = NULL;

void ogsSetLogSink(FILE *sink) {
    _OGS_LOG_SINK = sink;
}

void _ogsInitLog(void) {
    if (!_OGS_LOG_SINK) {
        _OGS_LOG_SINK = stdout;
    }
}

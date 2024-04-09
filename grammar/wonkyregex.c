#include "wonkyregex.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

int match(const char *pattern, const char *string) {

    /* for pcre2_compile */
    pcre2_code *re;
    PCRE2_SIZE erroffset;
    int errcode;
    PCRE2_UCHAR8 buffer[128];

    /* for pcre2_match */
    int rc;
    PCRE2_SIZE* ovector;

    size_t subject_size = strlen(string);
    uint32_t options = 0;

    pcre2_match_data *match_data;
    uint32_t ovecsize = 128;

    size_t pattern_size = strlen(pattern);
    re = pcre2_compile(pattern, pattern_size, options, &errcode, &erroffset, NULL);
    if (re == NULL) {
        pcre2_get_error_message(errcode, buffer, 120);
        fprintf(stderr,"%d\t%s\n", errcode, buffer);
        return 1;
    }

    match_data = pcre2_match_data_create(ovecsize, NULL);
    rc = pcre2_match(re, string, subject_size, 0, options, match_data, NULL);

    if(rc == 0) {
        fprintf(stderr,"offset vector too small: %d",rc);
        return 0;

    } else {
        return rc > 0;

    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
}

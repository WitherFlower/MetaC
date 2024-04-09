#ifndef WONKYREGEX_H
#define WONKYREGEX_H

/*
 * Returns 1 if the string could be matched by the regex
 * Compile using -lpcre2-8
 * Doing this cause regex.h didn't work properly
 */
int match(const char *pattern, const char *string);

#endif

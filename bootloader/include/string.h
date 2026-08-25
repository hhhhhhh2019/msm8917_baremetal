#ifndef STRING_H_
#define STRING_H_

size_t strlen(const char*);

bool is_digit(char);

i32 atoi(const char*, const char** end_ptr);
i64 atol(const char*, const char** end_ptr);

#endif // STRING_H_

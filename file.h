#ifndef _FILE_H_
#define _FILE_H_

// Struct representing a file's contents.
typedef struct file
{
	void *buffer;
	int size;
} file_t;

// File read operations.
void file_null(file_t *out);
int file_load(const char *filename, file_t *out);
void file_destroy(file_t *file);

#endif // _FILE_H_

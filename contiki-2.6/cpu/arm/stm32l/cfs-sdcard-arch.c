#include <sys/process.h>
#include <sys/etimer.h>
#include <cfs/cfs.h>
#include <debug-uart.h>
#include <stdio.h>


int
cfs_open (const char *name, int flags)
{
  return -1;
}

void
cfs_close(int fd)
{

}

int
cfs_remove(const char *name)
{
  return -1;
}

int
cfs_read (int fd, void *buf, unsigned int len)
{
  return -1;
}

int
cfs_write (int fd, const void *buf, unsigned int len)
{
  return -1;
}

cfs_offset_t
cfs_seek (int fd, cfs_offset_t offset, int whence)
{
  return 0;
}


/* Cause a compile time error if expr is false */
#ifdef __GNUC__
#define COMPILE_TIME_CHECK(expr) \
(void) (__builtin_choose_expr ((expr), 0, ((void)0))+3)
#else
#define COMPILE_TIME_CHECK(expr)
#endif


int
cfs_opendir (struct cfs_dir *dirp, const char *name)
{
  return -1;
}

int
cfs_readdir (struct cfs_dir *dirp, struct cfs_dirent *dirent)
{
  return -1;
}

void
cfs_closedir (struct cfs_dir *dirp)
{
}

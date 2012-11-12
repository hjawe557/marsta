#include <debug-uart.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

caddr_t
_sbrk(int incr)
{
  extern char __heap_start__;           /* Defined by the linker */
  extern char __heap_end__;             /* Defined by the linker */
  static char *heap_end = &__heap_start__;
  char *prev_heap_end;
 
  prev_heap_end = heap_end;
  if (heap_end + incr > &__heap_end__) {
    printf("Heap full (requested %d, available %d)\n",
            incr, (int)(&__heap_end__ - heap_end));
    errno = ENOMEM;
    return (caddr_t)-1;
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

void _exit (int n)
{
  /* FIXME: return code is thrown away.  */
  while(1);
}



int _kill (int n, int m)
{
   return -1;
}
int _getpid     (int n)
{
   return -1;
}
int _open (const char * path,
       int          flags,
       ...)
{
  return -1;
}
int _fflush_r(struct _reent *r, FILE *f)
{
  return 0;
}


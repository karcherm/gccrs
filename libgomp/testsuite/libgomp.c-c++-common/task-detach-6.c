/* { dg-do run } */

/* { dg-additional-sources "../lib/on_device_arch.c" } */
extern int on_device_arch_nvptx ();

#include <omp.h>
#include <assert.h>

/* Test tasks with detach clause on an offload device.  Each device
   thread spawns off a chain of tasks, that can then be executed by
   any available thread.  */

int main (void)
{
  //TODO See '../libgomp.c/pr99555-1.c'.
  if (on_device_arch_nvptx ())
    __builtin_abort (); //TODO Until resolved, skip, with error status.

  int x = 0, y = 0, z = 0;
  int thread_count;
  omp_event_handle_t detach_event1, detach_event2;

  #pragma omp target map (tofrom: x, y, z) map (from: thread_count)
    #pragma omp parallel private (detach_event1, detach_event2)
      {
	#pragma omp single
	  thread_count = omp_get_num_threads ();

	#pragma omp task detach(detach_event1) untied
	  #pragma omp atomic update
	    x++;

	#pragma omp task detach(detach_event2) untied
	{
	  #pragma omp atomic update
	    y++;
	  omp_fulfill_event (detach_event1);
	}

	#pragma omp task untied
	{
	  #pragma omp atomic update
	    z++;
	  omp_fulfill_event (detach_event2);
	}
      }

  assert (x == thread_count);
  assert (y == thread_count);
  assert (z == thread_count);
}

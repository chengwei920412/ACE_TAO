#include "ace/Service_Config.h"
// $Id$

#include "ace/Synch.h"

#if defined (ACE_HAS_THREADS)

// Define a class that will be stored in thread-specific data.  Note
// that as far as this class is concerned it's just a regular C++
// class.  The ACE_TSS wrapper transparently ensures that
// objects of this class will be placed in thread-specific storage.
// All calls on ACE_TSS::operator->() are delegated to the
// appropriate method in the Errno class.

class Errno
{
public:
  int error (void) { return this->errno_; }
  void error (int i) { this->errno_ = i; }
  
  int line (void) { return this->lineno_; }
  void line (int l) { this->lineno_ = l; }

  // Errno::flags_ is a static variable, so we've got to protect it
  // with a mutex since it isn't kept in thread-specific storage.
  int flags (void) 
  { 
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Errno::lock_, -1);

    return Errno::flags_;
  }

  void flags (int f)
  {
    ACE_GUARD (ACE_Thread_Mutex, ace_mon, Errno::lock_);

    Errno::flags_ = f;
  }

private:
  // = errno_ and lineno_ will be thread-specific data so they don't
  // need a lock.  
  int errno_;
  int lineno_;

  static int flags_;
#if defined (ACE_HAS_THREADS)
  // flags_ needs a lock. 
  static ACE_Thread_Mutex lock_;
#endif /* ACE_HAS_THREADS */
};

// Static variables.
ACE_MT (ACE_Thread_Mutex Errno::lock_);
int Errno::flags_;

// This is our thread-specific error handler...
static ACE_TSS<Errno> TSS_Error;

#if defined (ACE_HAS_THREADS)
// Serializes output via cout.
static ACE_Thread_Mutex lock;

typedef ACE_TSS_Guard<ACE_Thread_Mutex> GUARD;
#else
// Serializes output via cout.
static ACE_Null_Mutex lock;

typedef ACE_Guard<ACE_Null_Mutex> GUARD;
#endif /* ACE_HAS_THREADS */

static void 
cleanup (void *ptr)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) in cleanup, ptr = %x\n", ptr));

  delete ptr;
}

// This worker function is the entry point for each thread.

static void *
worker (void *c)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  int count = int (c);

  ACE_thread_key_t key = 0;
  int *ip = 0;

  // Make one key that will be available when the thread exits so that
  // we'll have something to cleanup!

  if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

  ip = new int;

  if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

  for (int i = 0; i < count; i++)
    {
      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

      ip = new int;

      ACE_DEBUG ((LM_DEBUG, "(%t) in worker 1, key = %d, ip = %x\n", key, ip));

      if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_getspecific (key, (void **) &ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_setspecific (key, (void *) 0) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      delete ip;

      if (ACE_OS::thr_keyfree (key) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keyfree"));

      // Cause an error.
      ACE_OS::read (ACE_INVALID_HANDLE, 0, 0);

      // The following two lines set the thread-specific state.
      TSS_Error->error (errno);
      TSS_Error->line (__LINE__);

      // This sets the static state (note how C++ makes it easy to do
      // both).
      TSS_Error->flags (count);

      {
	// Use the guard to serialize access to cout...
	ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, lock, 0);

	cout << "(" << ACE_Thread::self ()
	     << ") errno = "  << TSS_Error->error () 
	     << ", lineno = " << TSS_Error->line ()
	     << ", flags = " << TSS_Error->flags () 
	     << endl;
      }
      key = 0;

      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

      ip = new int;

      ACE_DEBUG ((LM_DEBUG, "(%t) in worker 2, key = %d, ip = %x\n", key, ip));

      if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_getspecific (key, (void **) &ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_setspecific (key, (void *) 0) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      delete ip;

      if (ACE_OS::thr_keyfree (key) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keyfree"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) exiting\n"));
  return 0;
}

extern "C" void 
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "signal = %S\n", signum));
  ACE_Service_Config::thr_mgr ()->exit (0);
}

int 
main (int argc, char *argv[])
{
  // The Service_Config must be the first object defined in main...
  ACE_Service_Config daemon (argv[0]);
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  int threads = argc > 1 ? ACE_OS::atoi (argv[1]) : 4;
  int count = argc > 2 ? ACE_OS::atoi (argv[2]) : 10000;

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) (handler), SIGINT);
  ACE_UNUSED_ARG (sa);

#if defined (ACE_HAS_THREADS)
  if (ACE_Service_Config::thr_mgr ()->spawn_n (threads, 
					       ACE_THR_FUNC (&worker), 
					       (void *) count,
					       THR_BOUND | THR_DETACHED) == -1)
    ACE_OS::perror ("ACE_Thread_Manager::spawn_n");

  ACE_Service_Config::thr_mgr ()->wait ();
#else
  worker ((void *) count);
#endif /* ACE_HAS_THREADS */
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_TSS<Errno>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int 
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, 
		     "ACE doesn't support support threads on this platform (yet)\n"),
		    -1);
}
#endif /* ACE_HAS_THREADS */

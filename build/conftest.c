/* confdefs.h */
#define PACKAGE_NAME "Lustre"
#define PACKAGE_TARNAME "lustre"
#define PACKAGE_VERSION "2.14.51_211_g38508ed"
#define PACKAGE_STRING "Lustre 2.14.51_211_g38508ed"
#define PACKAGE_BUGREPORT "https://jira.whamcloud.com/"
#define PACKAGE_URL ""
#define PACKAGE "lustre"
#define VERSION "2.14.51_211_g38508ed"
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define STDC_HEADERS 1
#define HAVE_DLFCN_H 1
#define LT_OBJDIR ".libs/"
#define LUSTRE_MAJOR 2
#define LUSTRE_MINOR 14
#define LUSTRE_PATCH 51
#define LUSTRE_FIX 0
#define LUSTRE_VERSION_STRING "2.14.51_211_g38508ed"
#define SIZEOF_UNSIGNED_LONG_LONG 8
#define HAVE_PYTHON "3.12"
/* end confdefs.h.  */

#include <linux/kernel.h>
#include <linux/module.h>

#if defined(NEED_LOCKDEP_IS_HELD_DISCARD_CONST)  && defined(CONFIG_LOCKDEP)  && defined(lockdep_is_held)
#undef lockdep_is_held
	#define lockdep_is_held(lock) 		lock_is_held((struct lockdep_map *)&(lock)->dep_map)
#endif


int
main (void)
{

  ;
  return 0;
};
MODULE_LICENSE("GPL");

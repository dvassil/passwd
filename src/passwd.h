/**************************************************
 * Created : 2011-09-11 04:43:11
 * Author  : dva
 * Project : passwd
 * Version : 1.0.0
 * History :
 **************************************************/
#ifndef __PASSWD_H__
#define __PASSWD_H__ 1

#ifndef _WIN32
#include <stdint.h>
#endif
#include <stdlib.h>

#ifdef ZTS
#include "TSRM.h"
#endif

#ifndef BOOL
#define BOOL	short
#endif //BOOL
#ifndef FALSE
#define FALSE	(0)
#endif
#ifndef TRUE
#define TRUE	(1)
#endif

#include "ext/standard/php_random.h"


#define PHP_PASSWD_WORLD_VERSION "1.1.0"
#define PHP_PASSWD_WORLD_EXTNAME "passwd"

#define BASE62	62

PHP_FUNCTION(passwd_version);
PHP_FUNCTION(passwd_create);
PHP_FUNCTION(tobase62);
PHP_FUNCTION(frombase62);

PHP_MINIT_FUNCTION(passwd);
PHP_MINFO_FUNCTION(passwd);

extern zend_module_entry passwd_module_entry;
#define phpext_passwd_ptr &passwd_module_entry

#endif //__PASSWD_H__

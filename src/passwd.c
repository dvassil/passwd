/**************************************************
 * Created : 2011-09-11 04:43:11
 * Author  : dva
 * Project : passwd
 * Version : 1.0.0
 * History :
 **************************************************/

#define PHP_EXPORTS 1

#include "php.h"
//#ifdef _WIN32
#include "ext/standard/info.h"
//#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "passwd.h"

#ifndef LONG
typedef long LONG;
#endif


/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_passwd_version, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_passwd_create, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, len, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_tobase62, 0)
	ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_frombase62, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

static zend_function_entry passwd_functions[] = {
	PHP_FE(passwd_version, arginfo_passwd_version)
	PHP_FE(passwd_create,  arginfo_passwd_create)
	PHP_FE(tobase62,       arginfo_tobase62)
	PHP_FE(frombase62,     arginfo_frombase62)
	{NULL, NULL, NULL}
};


zend_module_entry passwd_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_PASSWD_WORLD_EXTNAME,
	passwd_functions,
	PHP_MINIT(passwd),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(passwd),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_PASSWD_WORLD_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_PASSWD
ZEND_GET_MODULE(passwd)
#endif


char* digits = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char* passwordChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}\\|,<.>/?~;:";
int passwordCharsLen = 0;


static void ZEND_MODULE_GLOBALS_CTOR_N(passwd)(void *passwd_globals)
{
	//php_srand( time(NULL) );
}

PHP_MINIT_FUNCTION(passwd)
{
	passwordCharsLen = strlen(passwordChars);
	return SUCCESS;
}

PHP_MINFO_FUNCTION(passwd)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "passwd", "passwd Functions");
	php_info_print_table_row(2, "passwd_create", "return a random password string");
	php_info_print_table_row(2, "tobase62", "convert long to base62 string");
	php_info_print_table_row(2, "frombase62", "convert base62 string to long");
	php_info_print_table_end();
}

PHP_FUNCTION(passwd_version)
{
	RETURN_STRING(PHP_PASSWD_WORLD_VERSION);
}

PHP_FUNCTION(passwd_create)
{
    long i, len, rnd_idx;
    char* tmp;

    if (ZEND_NUM_ARGS() != 1) {
        WRONG_PARAM_COUNT;
    }

    len = 8;
    //if ((zend_parse_parameters(ZEND_NUM_ARGS(), "l", &len) == FAILURE) ||
    //    (len < 4) || (len > 128))
    //{
    //    RETURN_STRING("len should be an integer between 4 and 128");
    //}

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(len);
    ZEND_PARSE_PARAMETERS_END();

    if ((len < 4) || (len > 128))
    {
        RETURN_STRING("len should be an integer between 4 and 128");
    }

    tmp = emalloc(len+1);
    for (i=0; i<len; i++)
    {
	php_random_int_throw(0, passwordCharsLen-1, &rnd_idx);
        tmp[i] = passwordChars[rnd_idx];
    }
    tmp[i]='\0';

    RETURN_STRING(tmp);
}


PHPAPI unsigned long _php_math_base62tolong(const char* s, int len)
{
	unsigned long num = 0;
	double fnum = 0;
	int i;
	int mode = 0;
	char c;

	for (i = len; i > 0; i--) {
		c = *s++;

		/* might not work for EBCDIC */
		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 36;
		else continue;

		if (c >= BASE62)
			continue;
		
		num = num * BASE62 + c;
	}

	return num;
}

PHPAPI char * _php_math_longtobase62(unsigned long value)
{
	char buf[(sizeof(unsigned long) << 3) + 1];
	char *ptr, *end;

	end = ptr = buf + sizeof(buf) - 1;
	*ptr = '\0';

	do
	{
		*--ptr = digits[value % BASE62];
		value /= BASE62;
	}
	while (ptr > buf && value);

	return estrndup(ptr, end-ptr);
}

PHP_FUNCTION(tobase62)
{
	unsigned long number;
	char *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &number) == FAILURE) {
		return;
	}

	result = _php_math_longtobase62(number);

	RETURN_STRING(result);
}

PHP_FUNCTION(frombase62)
{
	long ret = 0;
	int len = 0;
	char* str;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &str, &len) == FAILURE)
	{
		RETURN_LONG(0);
	}

	ret = _php_math_base62tolong(str, len);
	RETURN_LONG(ret);
}

/* cereal extension for PHP (c) 2020 Sammy Kaye Powers <sammyk@php.net> */

#ifndef PHP_CEREAL_H
# define PHP_CEREAL_H

extern zend_module_entry cereal_module_entry;
# define phpext_cereal_ptr &cereal_module_entry

# define PHP_CEREAL_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_CEREAL)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

ZEND_BEGIN_MODULE_GLOBALS(cereal)
	int use_arena;
ZEND_END_MODULE_GLOBALS(cereal)

#ifdef ZTS
#define CEREAL_G(v) TSRMG(cereal_globals_id, zend_cereal_globals *, v)
#else
#define CEREAL_G(v) (cereal_globals.v)
#endif

#endif	/* PHP_CEREAL_H */

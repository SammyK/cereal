/* cereal extension for PHP (c) 2020 Sammy Kaye Powers <sammyk@php.net> */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_cereal.h"
#include "cereal_arginfo.h"

#define CEREAL_ARENA_SIZE 1024 * 32 // TODO Figure out how much we need

ZEND_DECLARE_MODULE_GLOBALS(cereal)

typedef struct cereal_obj {
	zend_object *obj;
	struct cereal_obj *prev;
} cereal_obj;

ZEND_TLS cereal_obj *cereals;
ZEND_TLS zend_arena *cereal_arena;

zend_class_entry *cereal_ce_ptr;

static zend_object *_cereal_obj_create(zend_class_entry *ce)
{
	zend_object *obj;
	cereal_obj *c;

	if (CEREAL_G(use_arena) == 1) {
		obj = zend_arena_calloc(&cereal_arena, 1, sizeof(zend_object) + zend_object_properties_size(ce));
		zend_object_std_init(obj, ce);
		object_properties_init(obj, ce);
		obj->handlers = &std_object_handlers;
		c = zend_arena_calloc(&cereal_arena, 1, sizeof(cereal_obj));
	} else {
		obj = zend_objects_new(ce);
		object_properties_init(obj, ce);
		c = (cereal_obj *) ecalloc(1, sizeof(cereal_obj));
	}

	c->obj = obj;
	c->prev = cereals;
	cereals = c;

	GC_ADDREF(obj);

	return obj;
}

static void php_cereal_init_globals(zend_cereal_globals *cereal_globals)
{
	cereal_globals->use_arena = 0;
}

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_FUNCTION(get_cereal)
{
	ZEND_PARSE_PARAMETERS_NONE();

	ZVAL_OBJ(return_value, cereal_ce_ptr->create_object(cereal_ce_ptr));
}

PHP_INI_BEGIN()
	STD_PHP_INI_BOOLEAN("cereal.use_arena", "0", PHP_INI_SYSTEM, OnUpdateBool, use_arena, zend_cereal_globals, cereal_globals)
PHP_INI_END()

static PHP_MINIT_FUNCTION(cereal)
{
#if defined(ZTS) && defined(COMPILE_DL_CEREAL)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	ZEND_INIT_MODULE_GLOBALS(cereal, php_cereal_init_globals, NULL);
	REGISTER_INI_ENTRIES();

	zend_class_entry _ce;
	INIT_CLASS_ENTRY(_ce, "Cereal", class_Cereal_methods);
	_ce.create_object = _cereal_obj_create;
	cereal_ce_ptr = zend_register_internal_class_ex(&_ce, NULL);
	cereal_ce_ptr->ce_flags |= ZEND_ACC_FINAL;
	zend_declare_property_string(cereal_ce_ptr, ZEND_STRL("name"), "", ZEND_ACC_PUBLIC);
	zend_declare_property_string(cereal_ce_ptr, ZEND_STRL("milk"), "", ZEND_ACC_PUBLIC);

	return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(cereal)
{
	return SUCCESS;
}

static PHP_RINIT_FUNCTION(cereal)
{
	cereals = NULL;
	if (CEREAL_G(use_arena) == 1) {
		cereal_arena = zend_arena_create(CEREAL_ARENA_SIZE);
	}

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(cereal)
{
	if (CEREAL_G(use_arena) == 1) {
#if 0
		ptrdiff_t bytes = 0;
		zend_arena *a = cereal_arena;
		while (a) {
			bytes += ((char *) a->end) - ((char *) a->ptr);
			a = a->prev;
		}
		php_printf("Arena size: %td bytes\n", bytes);
#endif
		php_printf("Freeing arena (%p)\n", cereal_arena);
		zend_arena_destroy(cereal_arena);
		cereal_arena = NULL;
	} else {
		cereal_obj *c = cereals;
		cereal_obj *tmp;
		
		if (c) {
			php_printf("Freeing cereals one at a time (%p)\n", c);
		}

		while (c) {
			tmp = c->prev;
			//php_printf("Freeing cereal #%lu (%p)\n", (unsigned long) c->obj->handle, c);
			GC_DELREF(c->obj);
			zend_object_std_dtor(c->obj);
			efree(c);
			c = tmp;
		}
		cereals = NULL;
	}

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(cereal)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "cereal support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

zend_module_entry cereal_module_entry = {
	STANDARD_MODULE_HEADER,
	"cereal",				/* Extension name */
	ext_functions,			/* zend_function_entry */
	PHP_MINIT(cereal),		/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(cereal),	/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(cereal),		/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(cereal),	/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(cereal),		/* PHP_MINFO - Module info */
	PHP_CEREAL_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CEREAL
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(cereal)
#endif

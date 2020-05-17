PHP_ARG_ENABLE([cereal],
  [whether to enable cereal support],
  [AS_HELP_STRING([--enable-cereal],
    [Enable cereal support])],
  [no])

if test "$PHP_CEREAL" != "no"; then
  AC_DEFINE(HAVE_CEREAL, 1, [ Have cereal support ])
  PHP_NEW_EXTENSION(cereal, cereal.c, $ext_shared)
fi

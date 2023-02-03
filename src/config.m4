PHP_ARG_ENABLE(passwd, whether to enable passwd support, [ --enable-passwd Enable passwd support])

if test "$PHP_PASSWD" = "yes"; then
  AC_DEFINE(HAVE_PASSWD, 1, [Whether you have passwd])
  PHP_NEW_EXTENSION(passwd, passwd.c, $ext_shared)
fi

INC_CHECK_DIRS="/usr /usr/local"

for i in $INC_CHECK_DIRS ; do
  if test -f $i/include/php/ext/standard/php_rand.h; then
    PHP_ADD_INCLUDE($i/include/php/ext/standard)
    break
  fi
done

SUMMARY = "Provide limited super user privileges to specific users"
DESCRIPTION = "Sudo (superuser do) allows a system administrator to give certain users (or groups of users) the ability to run some (or all) commands as root while logging all commands and arguments."
HOMEPAGE = "http://www.sudo.ws"
BUGTRACKER = "http://www.sudo.ws/bugs/"
SECTION = "admin"
LICENSE = "ISC & BSD & Zlib"
LIC_FILES_CHKSUM = "file://doc/LICENSE;md5=6c76b73603ac7763ab0516ebfbe67b42 \
                    file://plugins/sudoers/redblack.c;beginline=1;endline=46;md5=4a162fc04b86b03f5632180fe6076cda \
                    file://lib/util/reallocarray.c;beginline=3;endline=15;md5=b47f1f85a12f05a0744cd8b1b6f41a0d \
                    file://lib/util/fnmatch.c;beginline=3;endline=27;md5=67f83ee9bd456557397082f8f1be0efd \
                    file://lib/util/getcwd.c;beginline=2;endline=27;md5=09068a19b4f6b6f0a0958655bfe98b63 \
                    file://lib/util/glob.c;beginline=2;endline=31;md5=1f2f771c35fb0658d567a7824007e56d \
                    file://lib/util/snprintf.c;beginline=3;endline=33;md5=63e48e1b992bce749a19dd9b2256e9a0 \
                    file://include/sudo_queue.h;beginline=2;endline=27;md5=082b138b72ba3e568a13a25c3bf254dc \
                    file://lib/util/inet_pton.c;beginline=3;endline=17;md5=3970ab0518ab79cbd0bafb697f10b33a \
                    file://lib/util/arc4random.c;beginline=3;endline=20;md5=15bdc89c1b003fa4d7353e6296ebfd68 \
                    file://lib/util/arc4random_uniform.c;beginline=3;endline=17;md5=31e630ac814d692fd0ab7a942659b46f \
                    file://lib/util/getentropy.c;beginline=1;endline=19;md5=9f1a275ecd44cc264a2a4d5e06a75292 \
                    "

inherit autotools

PACKAGECONFIG ??= ""
PACKAGECONFIG[zlib] = "--enable-zlib,--disable-zlib,zlib"
PACKAGECONFIG[pam-wheel] = ",,,pam-plugin-wheel"

CONFFILES_${PN} = "${sysconfdir}/sudoers"

EXTRA_OECONF = "--with-editor=/bin/vi --with-env-editor"

EXTRA_OECONF_append_libc-musl = " --disable-hardening "

# mksigname/mksiglist are used on build host to generate source files
do_compile_prepend () {
	# Remove build host references from sudo_usage.h
	sed -i  \
	    -e 's,--with-libtool-sysroot=${STAGING_DIR_TARGET},,g' \
	    -e 's,--build=${BUILD_SYS},,g' \
	    -e 's,--host=${HOST_SYS},,g' \
	    ${B}/src/sudo_usage.h
	oe_runmake SSP_CFLAGS="" SSP_LDFLAGS="" CC="$BUILD_CC" CFLAGS="$BUILD_CFLAGS" CPPFLAGS="$BUILD_CPPFLAGS -I${S}/include -I${S} -I${B}"  -C lib/util mksigname mksiglist
}

# Explicitly create ${localstatedir}/lib before do_install to ensure
# the directory is accessible by all users. Otherwise the mkinstalldirs
# script (from sudo) will recursively create ${localstatedir}/lib/sudo
# and then chmod each directory with 0700 permissions, which isn't what
# we want (i.e, users would not be able to access /var/lib).
do_install_prepend (){
	mkdir -p ${D}/${localstatedir}/lib
}

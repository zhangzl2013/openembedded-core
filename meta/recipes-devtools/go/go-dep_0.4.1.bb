SUMMARY = "Dependency management tool for Golang"
HOMEPAGE = "https://github.com/golang/dep"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://src/${GO_IMPORT}/LICENSE;md5=1bad315647751fab0007812f70d42c0d"

GO_IMPORT = "github.com/golang/dep"
SRC_URI = "git://${GO_IMPORT} \
           file://0001-Add-support-for-mips-mips64.patch;patchdir=src/github.com/golang/dep \
          "

# Points to 0.4.1 tag
SRCREV = "37d9ea0ac16f0e0a05afc3b60e1ac8c364b6c329"

inherit go

GO_INSTALL = "${GO_IMPORT}/cmd/dep"

RDEPENDS_${PN}-dev += "bash"

BBCLASSEXTEND = "native nativesdk"

# for x86 ends with textrel in ${PN}
# http://errors.yoctoproject.org/Errors/Details/185630/
# ERROR: QA Issue: ELF binary '/work/i586-oe-linux/go-dep/0.4.1-r0/packages-split/go-dep/usr/bin/dep' has relocations in .text [textrel]
INSANE_SKIP_${PN} += "textrel"

# for aarch64 ends with textrel in ${PN}-ptest
# http://errors.yoctoproject.org/Errors/Details/185632/
# ERROR: QA Issue: ELF binary '/work/aarch64-oe-linux/go-dep/0.4.1-r0/packages-split/go-dep-ptest/usr/lib/go-dep/ptest/github.com/golang/dep/cmd/dep/dep.test' has relocations in .text [textrel]  
INSANE_SKIP_${PN}-ptest += "textrel"

#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\x2b\x3a\x21\x7f"
	"proc_create\0"
	"\x18\x00\x00\x00\x6e\xa2\x66\x3f"
	"register_kprobe\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\x27\x56\xb6\x2a"
	"remove_proc_entry\0\0\0"
	"\x14\x00\x00\x00\xb7\x6f\x70\xe6"
	"single_open\0"
	"\x14\x00\x00\x00\x69\xb6\x98\xa4"
	"seq_printf\0\0"
	"\x1c\x00\x00\x00\x1d\xe6\x10\xbb"
	"unregister_kprobe\0\0\0"
	"\x14\x00\x00\x00\x0e\xd8\xd5\xd9"
	"seq_read\0\0\0\0"
	"\x14\x00\x00\x00\xe5\x41\x2a\xae"
	"seq_lseek\0\0\0"
	"\x18\x00\x00\x00\x0a\x8d\x36\x7f"
	"single_release\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x18\x00\x00\x00\x64\xbd\x8f\xba"
	"_raw_spin_lock\0\0"
	"\x1c\x00\x00\x00\x34\x4b\xb5\xb5"
	"_raw_spin_unlock\0\0\0\0"
	"\x28\x00\x00\x00\xb3\x1c\xa2\x87"
	"__ubsan_handle_out_of_bounds\0\0\0\0"
	"\x18\x00\x00\x00\xde\x9f\x8a\x25"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DD3516740E815C84655D17B");

// File: syscall_counter.c

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Duuudeeeeeel");
MODULE_DESCRIPTION("Zählt alle Systemaufrufe via Kprobe direkt auf do_syscall_64");

#define MAX_SYSCALLS 512
static unsigned long syscall_counts[MAX_SYSCALLS];

/* Pre-Handler: läuft VOR jedem Systemaufruf */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    unsigned long nr;

#if defined(__x86_64__)
    nr = regs->orig_ax;
#elif defined(__aarch64__)
    nr = regs->orig_x0;
#else
# error "Architektur nicht unterstützt"
#endif

    if (nr < MAX_SYSCALLS)
        syscall_counts[nr]++;
    return 0;  /* weiter mit der echten do_syscall_64 */
}

/* Kprobe direkt auf die exakte Adresse von do_syscall_64 */
static struct kprobe kp = {
    .address     = (kprobe_opcode_t *)0xffffffff8be911f0,
    .pre_handler = handler_pre,
};

static int __init sc_init(void)
{
    int i, ret;

    /* Zähler initialisieren */
    for (i = 0; i < MAX_SYSCALLS; i++)
        syscall_counts[i] = 0;

    pr_info("syscall_counter: versuche Kprobe auf address %p\n", kp.address);
    ret = register_kprobe(&kp);
    pr_info("syscall_counter: register_kprobe() returned %d\n", ret);
    if (ret < 0) {
        pr_err("syscall_counter: Kprobe-Registration fehlgeschlagen: %d\n", ret);
        return ret;
    }
    pr_info("syscall_counter: Kprobe erfolgreich registriert\n");
    return 0;
}

static void __exit sc_exit(void)
{
    int i;
    unregister_kprobe(&kp);

    pr_info("syscall_counter: Kprobe entfernt, Syscalls gezählt:\n");
    for (i = 0; i < MAX_SYSCALLS; i++) {
        if (syscall_counts[i])
            pr_info("  Syscall %-4u = %10lu Aufrufe\n", i, syscall_counts[i]);
    }
}

module_init(sc_init);
module_exit(sc_exit);



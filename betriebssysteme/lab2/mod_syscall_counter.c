// mod_syscall_counter.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kprobes.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <linux/syscalls.h>

#define PROC_NAME       "syscall_counter"
#define MAX_SYSCALLS    450
#define WANT_SYSCALL_NAMES

#ifdef WANT_SYSCALL_NAMES
# include "syscall_names.h"
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Duuuuuuudeeeel");
MODULE_DESCRIPTION("Counts almostttttttt all syscalls and exposes them via /proc");

/* Zähler und Synchronisation */
static unsigned long syscall_counters[MAX_SYSCALLS];
static DEFINE_SPINLOCK(counter_lock);

/* /proc-Eintrag */
static struct proc_dir_entry *proc_entry;
/* Kprobe-Objekt */
static struct kprobe kp;

/* Mögliche Einstiegspunkte */
#if defined(__x86_64__)
static const char *possible_syscall_entries[] = {
    "syscall_trace_enter",
    "do_syscall_64",
    NULL
};
#elif defined(__aarch64__)
static const char *possible_syscall_entries[] = {
    "syscall_trace_enter",
    "invoke_syscall",
    NULL
};
#else
static const char *possible_syscall_entries[] = {
    "syscall_trace_enter",
    NULL
};
#endif

/* Handler vor jedem Syscall */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    unsigned long nr;
#if defined(__x86_64__)
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0)
        nr = regs->orig_ax;
    #else
        nr = regs->ax;
    #endif
#elif defined(__aarch64__)
    nr = regs->regs[1];
#else
    return 0;
#endif

    if (nr < MAX_SYSCALLS) {
        spin_lock(&counter_lock);
        syscall_counters[nr]++;
        spin_unlock(&counter_lock);
    }
    return 0;
}

/* /proc-Ausgabe */
static int syscall_counter_show(struct seq_file *m, void *v)
{
    int i;
    spin_lock(&counter_lock);
    for (i = 0; i < MAX_SYSCALLS; i++) {
        if (syscall_counters[i] > 0) {
            #ifdef WANT_SYSCALL_NAMES
                seq_printf(m, "%3d: %-30s => %lu\n",
                           i, syscall_names[i], syscall_counters[i]);
            #else
                seq_printf(m, "%3d => %lu\n",
                           i, syscall_counters[i]);
            #endif
        }
    }
    spin_unlock(&counter_lock);
    return 0;
}

static int syscall_counter_open(struct inode *inode, struct file *file)
{
    return single_open(file, syscall_counter_show, NULL);
}

static const struct proc_ops syscall_counter_fops = {
    .proc_open    = syscall_counter_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/* Modul-Init */
static int __init mod_syscall_counter_init(void)
{
    int i, ret;
    bool registered = false;

    /* /proc anlegen */
    proc_entry = proc_create(PROC_NAME, 0444, NULL, &syscall_counter_fops);
    if (!proc_entry)
        return -ENOMEM;

    /* kprobe registrieren */
    kp.pre_handler = handler_pre;
    for (i = 0; possible_syscall_entries[i]; i++) {
        kp.symbol_name = possible_syscall_entries[i];
        ret = register_kprobe(&kp);
        if (ret == 0) {
            printk(KERN_INFO "kprobe at %s registered\n",
                   possible_syscall_entries[i]);
            registered = true;
            break;
        }
    }
    if (!registered) {
        remove_proc_entry(PROC_NAME, NULL);
        return -EINVAL;
    }

    printk(KERN_INFO "syscall_counter: Module loaded\n");
    return 0;
}

/* Modul-Exit */
static void __exit mod_syscall_counter_exit(void)
{
    unregister_kprobe(&kp);
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "syscall_counter: Module unloaded\n");
}

module_init(mod_syscall_counter_init);
module_exit(mod_syscall_counter_exit);
sudo bpftrace -e '
  tracepoint:syscalls:sys_enter_openat,
  tracepoint:syscalls:sys_enter_read,
  tracepoint:syscalls:sys_enter_write,
  tracepoint:syscalls:sys_enter_close
  {
    @[probe]++; 
  }
  END {
    print(@);
  }
'


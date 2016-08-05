/***************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 ***************************************************************************/

#ifndef API_SYSCALLS_H_
#define API_SYSCALLS_H_

#ifndef __ASSEMBLY__
#define SVC_NUM_SYS_CALL_EXIT  0
#define SYS_CALL_EXIT(exitCode)  asm volatile("mov r1, %[arg]\r\n mov r0, %[svcnum]\r\n svc #0\r\n"::[arg] "r" (exitCode), [svcnum] "r" (SVC_NUM_SYS_CALL_EXIT): "r0", "r1")
#endif

#define SYS_restart_syscall 0
#define SYS_exit    1
#define SYS_fork    2
#define SYS_read    3
#define SYS_write   4
#define SYS_open    5
#define SYS_close   6
#define SYS_creat   8
#define SYS_link    9
#define SYS_unlink  10
#define SYS_execve  11
#define SYS_chdir   12
#define SYS_mknod   14
#define SYS_chmod   15
#define SYS_lchown  16
#define SYS_lseek   19
#define SYS_getpid  20
#define SYS_mount   21
#define SYS_setuid  23
#define SYS_getuid  24
#define SYS_ptrace  26
#define SYS_pause   29
#define SYS_access  33
#define SYS_nice    34
#define SYS_sync    36
#define SYS_kill    37
#define SYS_rename  38
#define SYS_mkdir   39
#define SYS_rmdir   40
#define SYS_dup 41
#define SYS_pipe    42
#define SYS_times   43
#define SYS_brk 45
#define SYS_setgid  46
#define SYS_getgid  47
#define SYS_geteuid 49
#define SYS_getegid 50
#define SYS_acct    51
#define SYS_umount2 52
#define SYS_ioctl   54
#define SYS_fcntl   55
#define SYS_setpgid 57
#define SYS_umask   60
#define SYS_chroot  61
#define SYS_ustat   62
#define SYS_dup2    63
#define SYS_getppid 64
#define SYS_getpgrp 65
#define SYS_setsid  66
#define SYS_sigaction   67
#define SYS_setreuid    70
#define SYS_setregid    71
#define SYS_sigsuspend  72
#define SYS_sigpending  73
#define SYS_sethostname 74
#define SYS_setrlimit   75
#define SYS_getrusage   77
#define SYS_gettimeofday    78
#define SYS_settimeofday    79
#define SYS_getgroups   80
#define SYS_setgroups   81
#define SYS_symlink 83
#define SYS_readlink    85
#define SYS_uselib  86
#define SYS_swapon  87
#define SYS_reboot  88
#define SYS_munmap  91
#define SYS_truncate    92
#define SYS_ftruncate   93
#define SYS_fchmod  94
#define SYS_fchown  95
#define SYS_getpriority 96
#define SYS_setpriority 97
#define SYS_statfs  99
#define SYS_fstatfs 100
#define SYS_syslog  103
#define SYS_setitimer   104
#define SYS_getitimer   105
#define SYS_stat    106
#define SYS_lstat   107
#define SYS_fstat   108
#define SYS_vhangup 111
#define SYS_wait4   114
#define SYS_swapoff 115
#define SYS_sysinfo 116
#define SYS_fsync   118
#define SYS_sigreturn   119
#define SYS_clone   120
#define SYS_setdomainname   121
#define SYS_uname   122
#define SYS_adjtimex    124
#define SYS_mprotect    125
#define SYS_sigprocmask 126
#define SYS_init_module 128
#define SYS_delete_module   129
#define SYS_quotactl    131
#define SYS_getpgid 132
#define SYS_fchdir  133
#define SYS_bdflush 134
#define SYS_sysfs   135
#define SYS_personality 136
#define SYS_setfsuid    138
#define SYS_setfsgid    139
#define SYS__llseek 140
#define SYS_getdents    141
#define SYS__newselect  142
#define SYS_flock   143
#define SYS_msync   144
#define SYS_readv   145
#define SYS_writev  146
#define SYS_getsid  147
#define SYS_fdatasync   148
#define SYS__sysctl 149
#define SYS_mlock   150
#define SYS_munlock 151
#define SYS_mlockall    152
#define SYS_munlockall  153
#define SYS_sched_setparam  154
#define SYS_sched_getparam  155
#define SYS_sched_setscheduler  156
#define SYS_sched_getscheduler  157
#define SYS_sched_yield 158
#define SYS_sched_get_priority_max  159
#define SYS_sched_get_priority_min  160
#define SYS_sched_rr_get_interval   161
#define SYS_nanosleep   162
#define SYS_mremap  163
#define SYS_setresuid   164
#define SYS_getresuid   165
#define SYS_poll    168
#define SYS_nfsservctl  169
#define SYS_setresgid   170
#define SYS_getresgid   171
#define SYS_prctl   172
#define SYS_rt_sigreturn    173
#define SYS_rt_sigaction    174
#define SYS_rt_sigprocmask  175
#define SYS_rt_sigpending   176
#define SYS_rt_sigtimedwait 177
#define SYS_rt_sigqueueinfo 178
#define SYS_rt_sigsuspend   179
#define SYS_pread64 180
#define SYS_pwrite64    181
#define SYS_chown   182
#define SYS_getcwd  183
#define SYS_capget  184
#define SYS_capset  185
#define SYS_sigaltstack 186
#define SYS_sendfile    187
#define SYS_vfork   190
#define SYS_ugetrlimit  191
#define SYS_mmap2   192
#define SYS_truncate64  193
#define SYS_ftruncate64 194
#define SYS_stat64  195
#define SYS_lstat64 196
#define SYS_fstat64 197
#define SYS_lchown32    198
#define SYS_getuid32    199
#define SYS_getgid32    200
#define SYS_geteuid32   201
#define SYS_getegid32   202
#define SYS_setreuid32  203
#define SYS_setregid32  204
#define SYS_getgroups32 205
#define SYS_setgroups32 206
#define SYS_fchown32    207
#define SYS_setresuid32 208
#define SYS_getresuid32 209
#define SYS_setresgid32 210
#define SYS_getresgid32 211
#define SYS_chown32 212
#define SYS_setuid32    213
#define SYS_setgid32    214
#define SYS_setfsuid32  215
#define SYS_setfsgid32  216
#define SYS_getdents64  217
#define SYS_pivot_root  218
#define SYS_mincore 219
#define SYS_madvise 220
#define SYS_fcntl64 221
#define SYS_gettid  224
#define SYS_readahead   225
#define SYS_setxattr    226
#define SYS_lsetxattr   227
#define SYS_fsetxattr   228
#define SYS_getxattr    229
#define SYS_lgetxattr   230
#define SYS_fgetxattr   231
#define SYS_listxattr   232
#define SYS_llistxattr  233
#define SYS_flistxattr  234
#define SYS_removexattr 235
#define SYS_lremovexattr    236
#define SYS_fremovexattr    237
#define SYS_tkill   238
#define SYS_sendfile64  239
#define SYS_futex   240
#define SYS_sched_setaffinity   241
#define SYS_sched_getaffinity   242
#define SYS_io_setup    243
#define SYS_io_destroy  244
#define SYS_io_getevents    245
#define SYS_io_submit   246
#define SYS_io_cancel   247
#define SYS_exit_group  248
#define SYS_lookup_dcookie  249
#define SYS_epoll_create    250
#define SYS_epoll_ctl   251
#define SYS_epoll_wait  252
#define SYS_remap_file_pages    253
#define SYS_set_tid_address 256
#define SYS_timer_create    257
#define SYS_timer_settime   258
#define SYS_timer_gettime   259
#define SYS_timer_getoverrun    260
#define SYS_timer_delete    261
#define SYS_clock_settime   262
#define SYS_clock_gettime   263
#define SYS_clock_getres    264
#define SYS_clock_nanosleep 265
#define SYS_statfs64    266
#define SYS_fstatfs64   267
#define SYS_tgkill  268
#define SYS_utimes  269
#define SYS_fadvise64_64    270
#define SYS_pciconfig_iobase    271
#define SYS_pciconfig_read  272
#define SYS_pciconfig_write 273
#define SYS_mq_open 274
#define SYS_mq_unlink   275
#define SYS_mq_timedsend    276
#define SYS_mq_timedreceive 277
#define SYS_mq_notify   278
#define SYS_mq_getsetattr   279
#define SYS_waitid  280
#define SYS_socket  281
#define SYS_bind    282
#define SYS_connect 283
#define SYS_listen  284
#define SYS_accept  285
#define SYS_getsockname 286
#define SYS_getpeername 287
#define SYS_socketpair  288
#define SYS_send    289
#define SYS_sendto  290
#define SYS_recv    291
#define SYS_recvfrom    292
#define SYS_shutdown    293
#define SYS_setsockopt  294
#define SYS_getsockopt  295
#define SYS_sendmsg 296
#define SYS_recvmsg 297
#define SYS_semop   298
#define SYS_semget  299
#define SYS_semctl  300
#define SYS_msgsnd  301
#define SYS_msgrcv  302
#define SYS_msgget  303
#define SYS_msgctl  304
#define SYS_shmat   305
#define SYS_shmdt   306
#define SYS_shmget  307
#define SYS_shmctl  308
#define SYS_add_key 309
#define SYS_request_key 310
#define SYS_keyctl  311
#define SYS_semtimedop  312
#define SYS_vserver 313
#define SYS_ioprio_set  314
#define SYS_ioprio_get  315
#define SYS_inotify_init    316
#define SYS_inotify_add_watch   317
#define SYS_inotify_rm_watch    318
#define SYS_mbind   319
#define SYS_get_mempolicy   320
#define SYS_set_mempolicy   321


#define SYS_openat  322
#define SYS_mkdirat 323
#define SYS_mknodat 324
#define SYS_fchownat    325
#define SYS_futimesat   326
#define SYS_fstatat64   327
#define SYS_unlinkat    328
#define SYS_renameat    329
#define SYS_linkat  330
#define SYS_symlinkat   331
#define SYS_readlinkat  332
#define SYS_fchmodat    333
#define SYS_faccessat   334
#define SYS_pselect6    335
#define SYS_ppoll   336
#define SYS_unshare 337
#define SYS_set_robust_list 338
#define SYS_get_robust_list 339
#define SYS_splice  340
#define SYS_sync_file_range2    341
#define SYS_tee 342
#define SYS_vmsplice    343
#define SYS_move_pages  344
#define SYS_getcpu  345
#define SYS_epoll_pwait 346
#define SYS_kexec_load  347
#define SYS_utimensat   348
#define SYS_signalfd    349
#define SYS_timerfd_create  350
#define SYS_eventfd 351
#define SYS_fallocate   352
#define SYS_timerfd_settime 353
#define SYS_timerfd_gettime 354
#define SYS_signalfd4   355
#define SYS_eventfd2    356
#define SYS_epoll_create1   357
#define SYS_dup3    358
#define SYS_pipe2   359
#define SYS_inotify_init1   360
#define SYS_preadv  361
#define SYS_pwritev 362
#define SYS_rt_tgsigqueueinfo   363
#define SYS_perf_event_open 364
#define SYS_recvmmsg    365
#define SYS_accept4 366
#define SYS_fanotify_init   367
#define SYS_fanotify_mark   368
#define SYS_prlimit64   369
#define SYS_name_to_handle_at   370
#define SYS_open_by_handle_at   371
#define SYS_clock_adjtime   372 /* TODO */
#define SYS_syncfs  373     /* Not implemented: Currently we only support RAM based filesystems */
#define SYS_sendmmsg    374 /* Not implemented: We do not have a network stack. */
#define SYS_setns   375     /* Not implemented: We do not support namespaces */
#define SYS_process_vm_readv    376 /* Not Implemented: hard to implement in secure manner. Use posix message queues. */
#define SYS_process_vm_writev   377 /* Not Implemented: hard to implement in secure manner. */
#define SYS_kcmp        378     /* Not implemented: Non posix and not exported by libc */
#define SYS_finit_module    379 /* Not implemented: We do not support loadable kernel modules */
#define SYS_sched_setattr   380 /* Not implemented: Use SYS_nice to change priority. */
#define SYS_sched_getattr   381 /* Not implemented: Use SYS_nice to change priority. */
#define SYS_renameat2   382     /* TODO */
#define SYS_seccomp 383
#define SYS_getrandom   384
#define SYS_memfd_create    385     /* Not implemented: This function call relies on presence of the Linux procfs. Procfs is not supported here. */

#define SYS_CALL_LAST       386
#define NUM_SYS_CALLS       (SYS_CALL_LAST)

#define EXT_SYS_CALL_BASE       0xf0000

#define EXT_get_curr_cpu        0xf0000
#define EXT_gen_sgi             0xf0001
#define EXT_cache_inval         0xf0002
#define EXT_cache_clean         0xf0003
#define EXT_set_thread_area     0xf0004
#define EXT_get_thread_area     0xf0005
#define EXT_tracelog_inval      0xf0006
#define EXT_tracelog_start      0xf0007
#define EXT_tracelog_stop       0xf0008
#define EXT_tracelog_add        0xf0009

#define EXT_SYS_CALL_LAST       0xf000a
#define NUM_EXT_SYS_CALLS       (EXT_SYS_CALL_LAST - EXT_SYS_CALL_BASE)

#endif /* API_SYSCALLS_H_ */

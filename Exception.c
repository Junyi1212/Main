#include "Exception.h"
#include "ShareLib.h"

#ifndef REG_RIP
#define REG_RIP 16 /** @warning 16 = REG_RIP only in 64bit machines */
#endif
#ifndef REG_EIP
#define REG_EIP 14  /* should be defined in ucontext.h */
#endif

#define EXCEPTION_MAX_LEN_INFO_TEXT   150
#define EXCEPTION_MAX_BACKTRACE_SIZE 100

static ucontext_t *ucont = 0;

/* Second argument to trace.  */
typedef struct backtrace_arg
{
    void **buffer;
    int    size;
    int    frame_num;
}backtrace_arg;

/* Help function for backtrace.  */
static _Unwind_Reason_Code trace(struct _Unwind_Context *context, void *arg)
{
    backtrace_arg *bt_arg = (backtrace_arg *)arg;

    /* Ignore the top frame.  */
    if (bt_arg->frame_num != -1)
    {
        bt_arg->buffer[bt_arg->frame_num] = (void *)_Unwind_GetIP(context);
    }

    if (++bt_arg->frame_num == bt_arg->size)
    {
        return _URC_END_OF_STACK;
    }

    return _URC_NO_REASON;
}

/* Similar to glibc's backtrace.  Return the list of pc in BUFFER
   where BUFFER is allocated to contain at most SIZE number of
   pointers.  */
int backtrace(void **buffer, int size)
{
    struct backtrace_arg bt_arg = { buffer, size, -1 };
    _Unwind_Backtrace(trace, &bt_arg);
    return bt_arg.frame_num;
}

static void ExceptionDeal(const char *const description)
{
    printf("\nDescription    : %s\n", description);
    printf("Pid (OS)     : %u\n", getpid());

    /* Generate calltrace */
    printf("Calltrace:\n");

    void    *array[EXCEPTION_MAX_BACKTRACE_SIZE];
    Dl_info  dlinfo;
    size_t   size;
    int      i;
    void    *pc = NULL;

    if (ucont != 0)
    {
#ifdef __powerpc__
        pc = (void *)ucont->uc_mcontext.regs->nip;
#elif defined(__arm__)
        pc = (void *)ucont->uc_mcontext.arm_pc;
#else
        pc = (void *)ucont->uc_mcontext.gregs[REG_EIP];
#endif

        if (dladdr(pc, &dlinfo))
        {
            if (dlinfo.dli_sname)
            {
                printf("#0 0x%08lx in %s+%#lx from %s+%#lx\n",
                               (unsigned long)pc,
                               dlinfo.dli_sname,
                               (unsigned long)((char *) pc - (char *)dlinfo.dli_saddr),
                               dlinfo.dli_fname,
                               (unsigned long)((char *) pc - (char *)dlinfo.dli_fbase));

            }
            else
            {
                printf("#0 0x%08lx from %s+%#lx\n",
                               (unsigned long)pc,
                               dlinfo.dli_fname,
                               (unsigned long)((char *) pc - (char *)dlinfo.dli_fbase));
            }
        }
        else
        {
            printf("#0 0x%08lx\n", (unsigned long)pc);
        }
    }

    size = backtrace(array, EXCEPTION_MAX_BACKTRACE_SIZE);

    for (i = 0; i < size; i++)
    {
        if (dladdr(array[i], &dlinfo))
        {
            if (dlinfo.dli_sname)
            {
                printf("#%d 0x%08lx in %s+%#lx from %s+%#lx\n",
                               i + 1,
                               (unsigned long)array[i],
                               dlinfo.dli_sname,
                               (unsigned long)((char *)array[i] - (char *)dlinfo.dli_saddr),
                               dlinfo.dli_fname,
                               (unsigned long)((char *)array[i] - (char *)dlinfo.dli_fbase));
            }
            else
            {
                printf("#%d 0x%08lx from %s+%#lx\n",
                               i + 1,
                               (unsigned long)array[i],
                               dlinfo.dli_fname,
                               (unsigned long)((char *)array[i] - (char *)dlinfo.dli_fbase));
            }
        }
        else
        {
            printf("#%d 0x%08lx\n",
                           i + 1,
                           (unsigned long)array[i]);
        }
    }
    printf("%s: Out\n", __FUNCTION__);
}

static void HandleInvalidAddress(const siginfo_t *const info,const ucontext_t *const uc)
{
    char  infoText[EXCEPTION_MAX_LEN_INFO_TEXT];
    const char *detector;
    const char *reason;
    printf("%s\n", __FUNCTION__);
    detector = (info->si_code > 0 ? "kernel" : "user");

    switch (info->si_code)
    {
        case SEGV_MAPERR:
            reason = "address not mapped to object";
            break;

        case SEGV_ACCERR:
            reason = "invalid permissions for mapped object";
            break;

        default:
            reason = "unknown";
            break;
    }

#if defined(__arm__)
    (void)uc;

    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Invalid memory reference (reason: %i '%s') detected by %s at address %p from ?",
             info->si_code,
             reason,
             detector,
             info->si_addr);
#else
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Invalid memory reference (reason: %i '%s') detected by %s at address %p from %p",
             info->si_code,
             reason,
             detector,
             info->si_addr,
             (void *)uc->uc_mcontext.gregs[REG_RIP]);
#endif

    ExceptionDeal(infoText);
}

static void HandleInvalidInstruction(const siginfo_t *const info, const ucontext_t *const uc)
{
    char  infoText[EXCEPTION_MAX_LEN_INFO_TEXT];
    const char *detector;
    const char *reason;
    printf("%s\n", __FUNCTION__);
    detector = (info->si_code > 0 ? "kernel" : "user");

    switch (info->si_code)
    {
        case ILL_ILLOPC:
            reason = "illegal operation code";
            break;

        case ILL_ILLOPN:
            reason = "illegal operand";
            break;

        case ILL_ILLADR:
            reason = "illegal addressing mode";
            break;

        case ILL_ILLTRP:
            reason = "illegal trap";
            break;

        case ILL_PRVOPC:
            reason = "priviledged operation code";
            break;

        case ILL_PRVREG:
            reason = "priviledged register";
            break;

        case ILL_COPROC:
            reason = "coprocessor error";
            break;

        case ILL_BADSTK:
            reason = "internal stack error";
            break;

        default:
            reason = "unknown";
            break;
    }

#if defined(__arm__)
    (void)uc;
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Invalid instruction (reason: %i '%s') detected by %s at address %p from ?",
             info->si_code,
             reason,
             detector,
             info->si_addr);
#else
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Invalid instruction (reason: %i '%s') detected by %s at address %p from %p",
             info->si_code,
             reason,
             detector,
             info->si_addr,
             (void *)uc->uc_mcontext.gregs[REG_RIP]);
#endif

    ExceptionDeal(infoText);
}

static void HandleFloatingPointError(const siginfo_t *const info, const ucontext_t *const uc)
{
    char  infoText[EXCEPTION_MAX_LEN_INFO_TEXT];
    const char *detector;
    const char *reason;
    printf("%s\n", __FUNCTION__);
    detector = (info->si_code > 0 ? "kernel" : "user");

    switch (info->si_code)
    {
        case FPE_INTDIV:
            reason = "integer divide by zero";
            break;

        default:
            reason = "unknown";
            break;
    }

#if defined(__arm__)
    (void)uc;
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Floating point error (reason: %i '%s') detected by %s at address %p from ?",
             info->si_code,
             reason,
             detector,
             info->si_addr);
#else
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Floating point error (reason: %i '%s') detected by %s at address %p from %p",
             info->si_code,
             reason,
             detector,
             info->si_addr,
             (void *)uc->uc_mcontext.gregs[REG_RIP]);
#endif

    ExceptionDeal(infoText);
}

static void HandleBusError(const siginfo_t *const info, const ucontext_t *const uc)
{
    char  infoText[EXCEPTION_MAX_LEN_INFO_TEXT];
    const char *detector;
    const char *reason;
    printf("%s\n", __FUNCTION__);
    detector = (info->si_code > 0 ? "kernel" : "user");

    switch (info->si_code)
    {
        case BUS_ADRALN:
            reason = "invalid address aligment";
            break;

        default:
            reason = "unknown";
            break;
    }

#if defined(__arm__)
    (void)uc;
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Bus error (reason: %i '%s') detected by %s at address %p from ?",
             info->si_code,
             reason,
             detector,
             info->si_addr);
#else
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Bus error (reason: %i '%s') detected by %s at address %p from %p",
             info->si_code,
             reason,
             detector,
             info->si_addr,
             (void *)uc->uc_mcontext.gregs[REG_RIP]);
#endif

    ExceptionDeal(infoText);
}

static void HandleAbort(const siginfo_t *const info, const ucontext_t *const uc)
{
    char  infoText[EXCEPTION_MAX_LEN_INFO_TEXT];
    const char *detector;
    printf("%s\n", __FUNCTION__);
    detector = (info->si_code > 0 ? "kernel" : "user");

#if defined(__arm__)
    (void)uc;
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Abort signal received - detected by %s at address %p from ?",
             detector,
             info->si_addr);
#else
    snprintf(infoText,
             EXCEPTION_MAX_LEN_INFO_TEXT,
             "Abort signal received - detected by %s at address %p from %p",
             detector,
             info->si_addr,
             (void *)uc->uc_mcontext.gregs[REG_RIP]);
#endif

    ExceptionDeal(infoText);
}

static void SelfSigHandler(int        signo,
                            siginfo_t *info,
                            void      *context)
{
    ucontext_t *uc = (ucontext_t *)context;
    ucont          = uc;
    printf("%s\n", __FUNCTION__);
    switch (signo)
    {
        case SIGSEGV:
            HandleInvalidAddress(info, uc);
            break;

        case SIGILL:
            HandleInvalidInstruction(info, uc);
            break;

        case SIGFPE:
            HandleFloatingPointError(info, uc);
            break;

        case SIGBUS:
            HandleBusError(info, uc);
            break;

        case SIGABRT:
            HandleAbort(info, uc);
            break;

        default:
            ExceptionDeal("Unknown POSIX signal");
            break;
    }

    raise(signo);
}


void ExceptionInit(void)
{
    struct sigaction  action;
    struct sigaction  action_ignore;

    /* Register signal handlers */
    action.sa_handler   = NULL;
    action.sa_sigaction = SelfSigHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags     = SA_RESTART | SA_SIGINFO | SA_ONESHOT;
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGILL, &action, NULL);
    sigaction(SIGFPE, &action, NULL);
    sigaction(SIGBUS, &action, NULL);
    sigaction(SIGABRT, &action, NULL);
/*
    action_ignore.sa_handler = SIG_IGN;
    action_ignore.sa_flags   = 0;
    sigemptyset(&action_ignore.sa_mask);
    sigaction(SIGPIPE, &action_ignore, NULL);
    */
}

int ExceptionStart(void)
{
	const char *dlib_path = "/var/fpwork/lyun/test/lib/libCal.so";
	//貌似只能是绝对路径，相对路径找不到文件

	void *handle = dlopen(dlib_path, RTLD_GLOBAL | RTLD_NOW);
	if (handle == NULL) 
	{
		fprintf(stderr, "%s\n", dlerror());
		return -1;
	} 
	else 
	{
		CalculatorFuncPointer divide_func = (CalculatorFuncPointer)dlsym(handle, "divide");
		int divide_ret = divide_func(10, 0);
		printf("divide function result : %d \n", divide_ret);
		dlclose(handle);
	}
	return 0;
}

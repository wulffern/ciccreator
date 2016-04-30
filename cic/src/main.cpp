//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-4-19
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================


#include "cic-core.h"
#include <iostream>
#include <QDebug>
#include <QString>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

/* This structure mirrors the one found in /usr/include/asm/ucontext.h */
typedef struct _sig_ucontext {
    unsigned long     uc_flags;
    struct ucontext   *uc_link;
    stack_t           uc_stack;
    struct sigcontext uc_mcontext;
    sigset_t          uc_sigmask;
} sig_ucontext_t;

void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
{
    void *             array[50];
    void *             caller_address;
    char **            messages;
    int                size, i;
    sig_ucontext_t *   uc;

    uc = (sig_ucontext_t *)ucontext;

    /* Get the address at the time the signal was raised */
#if defined(__i386__) // gcc specific
    caller_address = (void *) uc->uc_mcontext.eip; // EIP: x86 specific
#elif defined(__x86_64__) // gcc specific
    caller_address = (void *) uc->uc_mcontext.rip; // RIP: x86_64 specific
#else
#error Unsupported architecture. // TODO: Add support for other arch.
#endif

    fprintf(stderr, "signal %d (%s), address is %p from %p\n",
            sig_num, strsignal(sig_num), info->si_addr,
            (void *)caller_address);

    size = backtrace(array, 50);

    /* overwrite sigaction with caller's address */
    array[1] = caller_address;

    messages = backtrace_symbols(array, size);

    /* skip first stack frame (points here) */
    for (i = 1; i < size && messages != NULL; ++i)
    {
        fprintf(stderr, "[bt]: (%d) %s\n", i, messages[i]);
    }

    free(messages);

    exit(EXIT_FAILURE);
}






int main(int argc, char *argv[])
{

    struct sigaction sigact;

    sigact.sa_sigaction = crit_err_hdlr;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;

    if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0)
    {
        fprintf(stderr, "error setting signal handler for %d (%s)\n",
                SIGSEGV, strsignal(SIGSEGV));

        exit(EXIT_FAILURE);
    }

    try
    {



        if(argc >=  3){

            QString file = argv[1];
            QString rules = argv[2];
            QString library = argv[3];

            if(library == ""){
                QRegularExpression re("/?([^\\/]+)\\.json");
                QRegularExpressionMatch m = re.match(file);
                library = m.captured(1);
            }

            //Load rules
            cIcCore::Rules::loadRules(rules);

            //Load design, this is where the magic happens
            cIcCore::Design * d = new cIcCore::Design();
            d->read(file);

            //Print SVG file
//        cIcPrinter::Svg * pr = new cIcPrinter::Svg("test");
            //       pr->print(d);

            //Minecraft JavaScript
            cIcPrinter::Minecraft * m = new cIcPrinter::Minecraft(library + ".js");
            m->print(d);

            //Print SPICE file
            cIcPrinter::Spice * spice = new cIcPrinter::Spice(library);
            spice->print(d);

            delete(spice);

            //Write GDS
            cIcCore::ConsoleOutput console;
            console.comment("Writing GDS");
            cIcPrinter::Gds * gd = new cIcPrinter::Gds(library);
            gd->print(d);
            delete(gd);

            //Write JSON
            d->writeJsonFile(library + ".json");

        }else{
            qWarning() << "Wrong number of arguments " << argc;
        }

    }catch(...){

        return -1;
    }


    exit(EXIT_SUCCESS);



}

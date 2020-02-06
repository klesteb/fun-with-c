!
! File: Makefile
! Date: 02-Sep-1999
! By  : Kevin Esteb
!
! This is the makefile to create the cclibs library.
!
!
CC = cc/inc=([-],[-.misc])'cflags'
LIBS = 
LINK = link'lflags'
OLBS = [-]vmslibs.olb
LIBR = libr/replace
!
OBJS = delay.obj,latio.obj,lbrio.obj,mbxio.obj,rmsio.obj,\
       subprocess.obj,symbol_subsitute.obj,vms_find_file.obj,vms_getdvi.obj,\
       vms_mail.obj,vms_submit.obj,vms_print.obj,vms_sndopr.obj,vms_trnlnm.obj,\
       vms_file_exists.obj,vms_filescan.obj,vms_bintim.obj,vms_numtim.obj,\
       vms_cmptim.obj,vms_asctim.obj,vms_fmtuic.obj,vms_asts.obj,vms_ef.obj,\
       vms_init_cli.obj,vms_crelnm.obj,vms_dellnm.obj,vms_chklnm.obj,\
       vms_sndopr_req.obj,vms_chkio.obj,vms_queio.obj,nspio.obj,\
       vms_tempfile.obj,vms_freespace.obj,gsc.obj
!
all: $(OBJS)
!
debug:
    cflags := /debug
    lflags := /debug
!
clean:
    delete *.lis;*,*.com;*
    purge *.exe,*.obj,*.c,*.h
!
!
delay.obj: delay.c
    $(CC) delay.c
    $(LIBR) $(OLBS) delay.obj 
!
accio.obj: accio.c accio.h
    $(CC) accio.c
    $(LIBR) $(OLBS) accio.obj
!
latio.obj: latio.c latio.h
    $(CC) latio.c
    $(LIBR) $(OLBS) latio.obj
!
lbrio.obj: lbrio.c lbrio.h
    $(CC) lbrio.c
    $(LIBR) $(OLBS) lbrio.obj
!
mbxio.obj: mbxio.c mbxio.h
    $(CC) mbxio.c
    $(LIBR) $(OLBS) mbxio.obj
!
rmsio.obj: rmsio.c rmsio.h
    $(CC) rmsio.c
    $(LIBR) $(OLBS) rmsio.obj
!
subprocess.obj: subprocess.c
    $(CC) subprocess.c
    $(LIBR) $(OLBS) subprocess.obj
!
symbol_subsitute.obj: symbol_subsitute.c
    $(CC) symbol_subsitute.c
    $(LIBR) $(OLBS) symbol_subsitute.obj
!
vms_init_cli.obj: vms_init_cli.c
    $(CC) vms_init_cli.c
    $(LIBR) $(OLBS) vms_init_cli.obj
!
vms_find_file.obj: vms_find_file.c
    $(CC) vms_find_file.c
    $(LIBR) $(OLBS) vms_find_file.obj
!
vms_getdvi.obj: vms_getdvi.c
    $(CC) vms_getdvi.c
    $(LIBR) $(OLBS) vms_getdvi.obj
!
vms_mail.obj: vms_mail.c
    $(CC) vms_mail.c
    $(LIBR) $(OLBS) vms_mail.obj
!
vms_submit.obj: vms_submit.c
    $(CC) vms_submit.c
    $(LIBR) $(OLBS) vms_submit.obj
!
vms_print.obj: vms_print.c
    $(CC) vms_print.c
    $(LIBR) $(OLBS) vms_print.obj
!
vms_sndopr.obj: vms_sndopr.c
    $(CC) vms_sndopr.c
    $(LIBR) $(OLBS) vms_sndopr.obj
!
vms_trnlnm.obj: vms_trnlnm.c
    $(CC) vms_trnlnm.c
    $(LIBR) $(OLBS) vms_trnlnm.obj
!
vms_file_exists.obj: vms_file_exists.c
    $(CC) vms_file_exists.c
    $(LIBR) $(OLBS) vms_file_exists.obj
!
vms_filescan.obj: vms_filescan.c
    $(CC) vms_filescan.c
    $(LIBR) $(OLBS) vms_filescan.obj
!
vms_bintim.obj: vms_bintim.c
    $(CC) vms_bintim.c
    $(LIBR) $(OLBS) vms_bintim.obj
!
vms_numtim.obj: vms_numtim.c
    $(CC) vms_numtim.c
    $(LIBR) $(OLBS) vms_numtim.obj
!
vms_cmptim.obj: vms_cmptim.c
    $(CC) vms_cmptim.c
    $(LIBR) $(OLBS) vms_cmptim.obj
!
vms_asctim.obj: vms_asctim.c
    $(CC) vms_asctim.c
    $(LIBR) $(OLBS) vms_asctim.obj
!
vms_fmtuic.obj: vms_fmtuic.c
    $(CC) vms_fmtuic.c
    $(LIBR) $(OLBS) vms_fmtuic.obj
!
vms_asts.obj: vms_asts.c
    $(CC) vms_asts.c
    $(LIBR) $(OLBS) vms_asts.obj
!
vms_ef.obj: vms_ef.c
    $(CC) vms_ef.c
    $(LIBR) $(OLBS) vms_ef.obj
!
vms_crelnm.obj: vms_crelnm.c
    $(CC) vms_crelnm.c
    $(LIBR) $(OLBS) vms_crelnm.obj
!
vms_dellnm.obj: vms_dellnm.c
    $(CC) vms_dellnm.c
    $(LIBR) $(OLBS) vms_dellnm.obj
!
vms_chklnm.obj: vms_chklnm.c
    $(CC) vms_chklnm.c
    $(LIBR) $(OLBS) vms_chklnm.obj
!
vms_sndopr_req.obj: vms_sndopr_req.c
    $(CC) vms_sndopr_req.c
    $(LIBR) $(OLBS) vms_sndopr_req.obj
!
vms_chkio.obj: vms_chkio.c
    $(CC) vms_chkio.c
    $(LIBR) $(OLBS) vms_chkio.obj
!
vms_queio.obj: vms_queio.c
    $(CC) vms_queio.c
    $(LIBR) $(OLBS) vms_queio.obj
!
nspio.obj: nspio.c
    $(CC) nspio.c
    $(LIBR) $(OLBS) nspio.obj
!
vms_tempfile.obj: vms_tempfile.c
    $(CC) vms_tempfile.c
    $(LIBR) $(OLBS) vms_tempfile.obj
!
vms_freespace.obj: vms_freespace.c
    $(CC) vms_freespace.c
    $(LIBR) $(OLBS) vms_freespace.obj
!
gsc.obj: gsc.c
    $(CC) gsc.c
    $(LIBR) $(OLBS) gsc.obj
!
! eof
!

/**** MODULE $SMBDEF ***
/*  Symbiont interface definitions
/* +
/*  Symbolic definitions for the symbiont to job controller interface.
/* 
/* 	Public definitions of message types, item codes, and
/* 	other constants utilied by the symbiont to job controller
/* 	interface facility.
/* 
/* -
/* 
/* 	Structure level
/* */
#define SMBMSG$K_STRUCTURE_LEVEL  1   /*  Current structure level	*/
#define SMBMSG$K_STRUCTURE_LEVEL_1  1 /*  Structure level 1		*/
#define SMBMSG$K_ARCH_MAX_STREAMS  32 /*  Architected maximum streams per symbiont	*/
/* 
/* 	Request header
/* */
#define SMBMSG$K_PAUSE_TASK  1        /*   - STOP /QUEUE		*/
#define SMBMSG$K_RESET_STREAM  2      /*   - STOP /QUEUE /RESET	*/
#define SMBMSG$K_RESUME_TASK  3       /*   - START /QUEUE (when paused)     */
#define SMBMSG$K_START_STREAM  4      /*   - START /QUEUE (when stopped)    */
#define SMBMSG$K_START_TASK  5        /*   - task available		*/
#define SMBMSG$K_STOP_STREAM  6       /*   - STOP /QUEUE /NEXT	*/
#define SMBMSG$K_STOP_TASK  7         /*   - STOP /QUEUE /ABORT or /REQUEUE */
#define SMBMSG$K_TASK_COMPLETE  8     /*   - stream is idle		*/
#define SMBMSG$K_TASK_STATUS  9       /*   - asynchronous status update     */
#define SMBMSG$K_START_SYMBIONT  10   /*   - symbiont creation confirmation */
#define SMBMSG$K_JOB_REQUEST  11      /*   - request a job to be executed   */
#define SMBMSG$K_MAX_REQUEST_CODE  12 /*  MUST BE LAST		*/
#define SMBMSG$S_REQUEST_HEADER  12

typedef struct {
	unsigned short SMBMSG$W_REQUEST_CODE;	/*  Request code	*/
/*   Define request codes	*/
unsigned char smbmsg$b_structure_level;	/*  Message structure level	*/
unsigned char smbmsg$b_stream_index;	/*  Stream index		*/
unsigned long smbmsg$l_symbiont_id ;	/*  Symbiont identification	*/
unsigned long smbmsg$l_job_id;		/*  Job number (or 0 if a queue-oriented request)	*/
	} smbmsg$request_header;
/* 
/* 	Item header
/* */
#define SMBMSG$K_ACCOUNTING_DATA  1   /*   - accounting information	*/
#define SMBMSG$K_ACCOUNT_NAME  2      /*   - account name		*/
#define SMBMSG$K_AFTER_TIME  3        /*   - /AFTER value	*/
#define SMBMSG$K_ALIGNMENT_PAGES  4   /*   - /ALIGN count	*/
#define SMBMSG$K_BOTTOM_MARGIN  5     /*   - trailing blank lines	*/
#define SMBMSG$K_CHARACTERISTICS  6   /*   - /CHARACTERISTICS value	*/
#define SMBMSG$K_CHECKPOINT_DATA  7   /*   - checkpoint information	*/
#define SMBMSG$K_CONDITION_VECTOR  8  /*   - task error messages	*/
#define SMBMSG$K_DEVICE_NAME  9       /*   - /ON value	*/
#define SMBMSG$K_DEVICE_STATUS  10    /*   - device status	*/
#define SMBMSG$K_ENTRY_NUMBER  11     /*   - job entry number	*/
#define SMBMSG$K_EXECUTOR_QUEUE  12   /*   - this output queue	*/
#define SMBMSG$K_FILE_COPIES  13      /*   - /COPIES value	*/
#define SMBMSG$K_FILE_COUNT  14       /*   - current file copy number	*/
#define SMBMSG$K_FILE_SETUP_MODULES  15 /*   - file setup module list	*/
#define SMBMSG$K_FIRST_PAGE  16       /*   - first page to print	*/
#define SMBMSG$K_FORM_LENGTH  17      /*   - lines per page	*/
#define SMBMSG$K_FORM_NAME  18        /*   - name of physical form	*/
#define SMBMSG$K_FORM_SETUP_MODULES  19 /*   - form setup module list	*/
#define SMBMSG$K_FORM_WIDTH  20       /*   - columns per line	*/
#define SMBMSG$K_FILE_IDENTIFICATION  21 /*   - device, fid, and did	*/
#define SMBMSG$K_FILE_SPECIFICATION  22 /*   - file name	*/
#define SMBMSG$K_JOB_COPIES  23       /*   - /JOB_COUNT value	*/
#define SMBMSG$K_JOB_COUNT  24        /*   - current job copy number	*/
#define SMBMSG$K_JOB_NAME  25         /*   - /NAME value	*/
#define SMBMSG$K_JOB_RESET_MODULES  26 /*   - job reset module list	*/
#define SMBMSG$K_LAST_PAGE  27        /*   - last page to print	*/
#define SMBMSG$K_LEFT_MARGIN  28      /*   - leading blank columns	*/
#define SMBMSG$K_LIBRARY_SPECIFICATION  29 /*   - library name	*/
#define SMBMSG$K_MAXIMUM_STREAMS  30  /*   - maximum supported symbiont	*/
#define SMBMSG$K_MESSAGE_VECTOR  31   /*   - error messages to print	*/
#define SMBMSG$K_NOTE  32             /*   - /NOTE value	*/
#define SMBMSG$K_PAGE_SETUP_MODULES  33 /*   - page setup module list	*/
#define SMBMSG$K_PARAMETER_1  34      /*   - user parameter 1	*/
#define SMBMSG$K_PARAMETER_2  35      /*   - user parameter 2	*/
#define SMBMSG$K_PARAMETER_3  36      /*   - user parameter 3	*/
#define SMBMSG$K_PARAMETER_4  37      /*   - user parameter 4	*/
#define SMBMSG$K_PARAMETER_5  38      /*   - user parameter 5	*/
#define SMBMSG$K_PARAMETER_6  39      /*   - user parameter 6	*/
#define SMBMSG$K_PARAMETER_7  40      /*   - user parameter 7	*/
#define SMBMSG$K_PARAMETER_8  41      /*   - user parameter 8	*/
#define SMBMSG$K_PRINT_CONTROL  42    /*   - printing control	*/
#define SMBMSG$K_PRIORITY  43         /*   - queue priority	*/
#define SMBMSG$K_QUEUE  44            /*   - generic queue name	*/
#define SMBMSG$K_REFUSE_REASON  45    /*   - reason task refused	*/
#define SMBMSG$K_RELATIVE_PAGE  46    /*   - /BACKWARD, /FORWARD values	*/
#define SMBMSG$K_REQUEST_CONTROL  47  /*   - request control	*/
#define SMBMSG$K_REQUEST_RESPONSE  48 /*   - request code being responded to	*/
#define SMBMSG$K_RIGHT_MARGIN  49     /*   - trailing blank columns	*/
#define SMBMSG$K_SEARCH_STRING  50    /*   - /SEARCH value	*/
#define SMBMSG$K_SEPARATION_CONTROL  51 /*   - separation control	*/
#define SMBMSG$K_STOP_CONDITION  52   /*   - reason for print abort	*/
#define SMBMSG$K_TIME_QUEUED  53      /*   - time queued	*/
#define SMBMSG$K_TOP_MARGIN  54       /*   - leading blank lines	*/
#define SMBMSG$K_UIC  55              /*   - UIC of submittor	*/
#define SMBMSG$K_USER_NAME  56        /*   - username	*/
#define SMBMSG$K_CHECKPOINT_FREQUENCY  57 /*   - pages per checkpoint	*/
#define SMBMSG$K_QUEUING_CONTROL  58  /*   - queuing control	*/
#define SMBMSG$K_RETRY_TIME  59       /*   - delta time at which symbiont should retry job	*/
#define SMBMSG$K_DEVICE_CONDITION  60 /*   - device error messages	*/
#define SMBMSG$K_MESSAGE_FILE  61     /*   - symbiont's message file	*/
#define SMBMSG$K_AGENT_PROFILE  62    /*   - security info	*/
#define SMBMSG$K_CPU_LIMIT  63
#define SMBMSG$K_FILE_SEPARATION  64
#define SMBMSG$K_LOG_QUEUE  65
#define SMBMSG$K_LOG_SPECIFICATION  66
#define SMBMSG$K_LOG_SPOOL  67
#define SMBMSG$K_OPERATOR_REQUEST  68
#define SMBMSG$K_WSDEFAULT  69
#define SMBMSG$K_WSEXTENT  70
#define SMBMSG$K_WSQUOTA  71
#define SMBMSG$K_FILE_ATTRIBUTES  72
#define SMBMSG$K_FILE_ATTRIBUTES_SIZE  73
#define SMBMSG$K_JOB_ATTRIBUTES  74
#define SMBMSG$K_JOB_ATTRIBUTES_SIZE  75
#define SMBMSG$K_QUEUE_ATTRIBUTES  76
#define SMBMSG$K_QUEUE_ATTRIBUTES_SIZE  77
#define SMBMSG$K_SUBMITTER_EPID  78
/* */
#define SMBMSG$K_MAX_ITEM_CODE  79    /*  MUST BE LAST	*/

#define SMBMSG$S_ITEM_HEADER 4
typedef struct
	{unsigned short
		smbmsg$w_item_size,	/*  Item size */
		smbmsg$w_item_code; 	/*  Item code */
	}	smbmsg$r_item_header;

/*   Define item codes
/* */
/* 	ACCOUNTING_DATA item
/* */
#define SMBMSG$S_ACCOUNTING_DATA 16
typedef struct
	{unsigned long
		smbmsg$l_pages_printed,		/*  Pages printed */
		smbmsg$l_qio_puts,		/*  Lines printed */
		smbmsg$l_rms_gets,		/*  File reads */
		smbmsg$l_cpu_time;		/*  Processor time */
	}	smbmsg$r_accounting_data;
/*  */
/* 	CHECKPOINT_DATA item
/*  */
#define SMBMSG$S_CHECKPOINT_DATA 24
typedef struct
	{unsigned char smbmsg$b_filler;		/*  Reserved */
	 unsigned char smbmsg$b_checkpoint_level; /*  Checkpoint structure level */
	 unsigned short smbmsg$w_offset;	/*  Offset into record */
	 unsigned long smbmsg$l_carcon;     	/*  Carriage control */
	 unsigned long smbmsg$l_page;		/*  Page number */
	 unsigned long smbmsg$l_record_number;	/*  Record number */
	 unsigned char smbmsg$q_user_key[8];	/*  User positioning key */
	} smbmsg$r_checkpoint_data;
#define SMBMSG$S_USER_KEY 8
/*  */
/* 	DEVICE_STATUS item
/*  */
#define SMBMSG$M_LOWERCASE 1
#define SMBMSG$M_PAUSE_TASK 2
#define SMBMSG$M_REMOTE 4
#define SMBMSG$M_SERVER 8
#define SMBMSG$M_STALLED 16
#define SMBMSG$M_STOP_STREAM 32
#define SMBMSG$M_TERMINAL 64
#define SMBMSG$M_UNAVAILABLE 128
#define SMBMSG$M_SYM_NOTIFIES 256
#define SMBMSG$M_SYM_REQUESTS_OPER 512
#define SMBMSG$M_SYM_COPIES_FILE 1024
#define SMBMSG$M_SYM_COPIES_JOB 2048
#define SMBMSG$M_SYM_ACCEPTS_ALL_FORMS 4096
#define SMBMSG$M_SYM_NO_JOB_CHECKPOINT 8192
#define SMBMSG$M_SYM_ALL_CHARACTERISTIC  16384
#define SMBMSG$M_NOTIFY_ON_INTERRUPT  32768
#define SMBMSG$S_DEVICE_STATUS 4
#pragma nostandard
typedef
	union{
	unsigned smbmsg$l_device_flags;
	variant_struct {
		unsigned smbmsg$v_lowercase:1;	/*   - supports lowercase */
		unsigned smbmsg$v_pause_task:1;	/*   - symbiont initiated pause */
		unsigned smbmsg$v_remote:1;	/*   - device is remote */
		unsigned smbmsg$v_server:1;	/*   - server symbiont */
		unsigned smbmsg$v_stalled:1;	/*   - task stalled */
		unsigned smbmsg$v_stop_stream:1;/*   - symbiont requesting stop stream */
		unsigned smbmsg$v_terminal:1;	/*   - device is a terminal */
		unsigned smbmsg$v_unavailable:1;/*   - device unavailable */
		unsigned smbmsg$v_sym_notifies:1;	/*   - Symbiont notifies for job completions */
		unsigned smbmsg$v_sym_requests_oper:1;	/*   - Symbiont generates operator messages */
		unsigned smbmsg$v_sym_copies_file:1;	/*   - Symbiont generates multiple file copies */
		unsigned smbmsg$v_sym_copies_job:1;	/*   - Symbiont generates multiple job copies */
		unsigned smbmsg$v_sym_accepts_all_forms:1;	/*   - Symbiont processes all form types */
		unsigned smbmsg$v_sym_no_job_checkpoint:1;	/*   - Always reprocess full job on a restart */
		unsigned smbmsg$v_sym_all_characteristic:1;	/*   - Symbiont processes all characteristics	*/
		unsigned smbmsg$v_notify_on_interrupt:1;	/*   - Symbiont notifies user on job interrupt	*/
		} smbmsg$devbits;
	} smbmsg$r_device_status;
#pragma standard
/*  */
/* 	PRINT_CONTROL item
/*  */
#define SMBMSG$M_DOUBLE_SPACE 1
#define SMBMSG$M_PAGE_HEADER 2
#define SMBMSG$M_PAGINATE 4
#define SMBMSG$M_PASSALL 8
#define SMBMSG$M_SEQUENCED 16
#define SMBMSG$M_SHEET_FEED 32
#define SMBMSG$M_TRUNCATE 64
#define SMBMSG$M_WRAP 128
#define SMBMSG$M_RECORD_BLOCKING 256
#define SMBMSG$M_PAGE_FOOTER 512
#define SMBMSG$M_DELETE_FILE  1024
#define SMBMSG$M_LOWERCASE_EXPLICIT  2048
#define SMBMSG$M_NOTIFY  4096
#define SMBMSG$M_RESTART  8192
#define SMBMSG$M_JOB_RETAIN  16384
#define SMBMSG$M_JOB_ERROR_RETAIN  32768
#define SMBMSG$S_PRINT_CONTROL 4

#pragma nostandard
typedef union
	{unsigned SMBMSG$L_PRINT_FLAGS;	/*  Print flags */
	 variant_struct{
		unsigned  smbmsg$v_double_space:1;	/*   - double space */
		unsigned  smbmsg$v_page_header:1;	/*   - print page headers */
		unsigned  smbmsg$v_paginate:1;		/*   - insert <FF>'s */
		unsigned  smbmsg$v_passall:1;		/*   - binary print file */
		unsigned  smbmsg$v_sequenced:1;		/*   - print sequence numbers */
		unsigned  smbmsg$v_sheet_feed:1;	/*   - pause at every TOF */
		unsigned  smbmsg$v_truncate:1;		/*   - truncate on overflow */
		unsigned  smbmsg$v_wrap:1;		/*   - wrap on overflow */
		unsigned  smbmsg$v_record_blocking:1;	/*   - block records in output buffer */
		unsigned  smbmsg$v_page_footer:1;	/*   - print page footers */
		unsigned  smbmsg$v_delete_file:1;	/*   - delete file after printing	*/
		unsigned  smbmsg$v_lowercase_explicit:1;/*   - job for printer that supports lowercase	*/
		unsigned  smbmsg$v_notify:1;		/*   - notify user when job completes
		unsigned  smbmsg$v_restart:1;		/*   - restart job after crash or requeue	*/
		unsigned  smbmsg$v_job_retain:1;	/*   - per job retain	*/
		unsigned  smbmsg$v_job_error_retain:1;	/*   - per job retain on error	*/
		} smbmsg$printbits;
	} smbmsg$r_print_control;
#pragma standard
/*  */
/* 	REQUEST_CONTROL item
/*  */
#define SMBMSG$M_ALIGNMENT_MASK 1
#define SMBMSG$M_PAUSE_COMPLETE 2
#define SMBMSG$M_RESTARTING 4
#define SMBMSG$M_TOP_OF_FILE 8
#define SMBMSG$S_REQUEST 4
#pragma nostandard
typedef union{
	unsigned smbmsg$l_request_flags;	/*  Print flags */
	variant_struct {
		unsigned smbmsg$v_alignment_mask:1;	/*   - print A's and 9's */
		unsigned smbmsg$v_pause_complete:1;	/*   - pause when request complete */
		unsigned smbmsg$v_restarting:1;		/*   - job is restarting */
		unsigned smbmsg$v_top_of_file:1;	/*   - rewind before resume */
	} smbmsg$request_bits;
	}smbmsg$r_request;
#pragma standard
/*  */
/* 	SEPARATION_CONTROL item
/*  */
#define SMBMSG$M_FILE_BURST 1
#define SMBMSG$M_FILE_FLAG 2
#define SMBMSG$M_FILE_TRAILER 4
#define SMBMSG$M_FILE_TRAILER_ABORT 8
#define SMBMSG$M_JOB_FLAG 16
#define SMBMSG$M_JOB_BURST 32
#define SMBMSG$M_JOB_RESET 64
#define SMBMSG$M_JOB_RESET_ABORT 128
#define SMBMSG$M_JOB_TRAILER 256
#define SMBMSG$M_JOB_TRAILER_ABORT 512
#define SMBMSG$M_FIRST_FILE_OF_JOB 1024
#define SMBMSG$M_LAST_FILE_OF_JOB 2048
#define SMBMSG$S_SEPARATION_CONTROL 4
#pragma nostandard
typedef union{
	unsigned smbmsg$l_separation_flags;	/*  Print flags */
	variant_struct {
		unsigned smbmsg$v_file_burst:1;		/*   - print file burst page */
		unsigned smbmsg$v_file_flag:1;		/*   - print file flag page */
		unsigned smbmsg$v_file_trailer:1;	/*   - print file trailer page */
		unsigned smbmsg$v_file_trailer_abort:1;	/*   - print file trailer page */
		unsigned smbmsg$v_job_flag:1;		/*   - print job flag page */
		unsigned smbmsg$v_job_burst:1;		/*   - print job burst page */
		unsigned smbmsg$v_job_reset:1;		/*   - execute job reset sequence */
		unsigned smbmsg$v_job_reset_abort:1;	/*   - execute job reset sequence */
		unsigned smbmsg$v_job_trailer:1;	/*   - print job trailer page */
		unsigned smbmsg$v_job_trailer_abort:1;	/*   - print job trailer page */
		unsigned smbmsg$v_first_file_of_job:1;	/*   - this is the first file of the current job */
		unsigned smbmsg$v_last_file_of_job:1;	/*   - this is the last file of the current job */
		} smbmsg$separation_bits;
	} smbmsg$r_separation_control;
#pragma standard

/* 
/* 	QUEUING_CONTROL item
/**/
#define SMBMSG$M_RETAIN_JOB  1
#define SMBMSG$S_QUEUING_CONTROL  4
#pragma nostandard
typedef union {
	unsigned long smbmsg$l_queuing_flags;	/*  Queuing flags	*/
	variant_struct {
	unsigned smbmsg$v_retain_job:1;	/*   - symbiont retained job on "fixable" error	*/
	}	smbmsg$$queuing_control$$1;
	} smbmsg$r_queuing_control;
#pragma standard

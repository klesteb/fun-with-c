/* This header file was created by converting the $NFBDEF macro
  * and adding the comments taken from NETUSR.SDL
 * (VMS 4.6 microfiche page 334)
 */

/* you should define NFB_MAX_FIELD_ID_ENTRIES before including this module */
/* if you don't it will default to the following value */
#ifndef NFB_MAX_FIELD_ID_ENTRIES 
#define NFB_MAX_FIELD_ID_ENTRIES 10
#endif

struct nfbdef {
    unsigned char nfb$b_fct;
    unsigned char nfb$b_flags;
    unsigned char nfb$b_database;
    unsigned char nfb$b_oper;
    long int nfb$l_srch_key;
    long int nfb$l_srch2_key;
    unsigned char nfb$b_oper2;
    unsigned char nfb$b_mbz1;
    short int nfb$w_cell_size;
    long int nfb$l_fldid[NFB_MAX_FIELD_ID_ENTRIES];
};

struct nfbdef1 {
  union {
    long int nfb$l_param_id;
    struct {
      unsigned nfb$v_inx : 16;
      unsigned nfb$v_typ : 2;
      unsigned nfb$v_spare : 6;
      unsigned nfb$v_db : 8;
    } fields;
  } overlay;
};

struct nfbdef2 {
  union {
    long int nfb$l_lng_value;
    long int nfb$l_bit_value;
    struct {
      short int nfb$w_str_count;
      char nfb$b_str_text;
    } string;
  } overlay;
};

#define NFB$C_ENDOFLIST	0	/* used to terminate the field i.d. */
#define NFB$C_WILDCARD	1	/* field to use for "match all" database searches */
#define NFB$C_CTX_SIZE	64	/* length of context area in P2 buffer */

/* The following codes are passed in the second IOSB longword to qualify
 * an SS$_ILLCNTRFUNC error.
 *
 * The high order word of these error codes must be zero
 * so that they won't be confused with field i.d.s
 */
#define NFB$_ERR_FCT	1	/* unrecognized NFB$B_FCT value */
#define NFB$_ERR_DB	2	/* unrecognized NFB$B_DATABASE value */
#define NFB$_ERR_P1	3	/* The P1 buffer is invalid */
#define NFB$_ERR_P2	4	/* The P2 buffer is invalid */
#define NFB$_ERR_P3	5	/* The P3 buffer is invalid */
#define NFB$_ERR_P4	6	/* The P4 buffer is invalid */
#define NFB$_ERR_P5	7	/* The P5 buffer should not have been specified */
#define NFB$_ERR_P6	8	/* The P6 buffer should not have been specified */
#define NFB$_ERR_CELL	9	/* unrecognized NFB$B_CELL value */
#define NFB$_ERR_OPER	10	/* unrecognized NFB$B_OPER value */
#define NFB$_ERR_SRCH	11	/* unrecognized NFB$L_SRCH_KEY field i.d. */
#define NFB$_ERR_SRCH2	12	/* unrecognized NFB$L_SRCH2_KEY field i.d. */
#define NFB$_ERR_OPER2	13	/* unrecognized NFB$B_OPER2 value */
#define NFB$_ERR_FLAGS	14	/* undefined bits in NFB$B_FLAGS were not zero */

#define NFB$B_FCT	0	/* function codes */
#define NFB$C_DECLNAME	21	/* declare name */
#define NFB$C_DECLOBJ	22	/* declare object */
#define NFB$C_DECLSERV	23	/* declare server process available */
#define NFB$C_LOGEVENT	28	/* log a network event */
#define NFB$C_READEVENT	29	/* read current raw event queue (used by EVL only) */
#define NFB$C_FC_DELETE	33	/* remove an entry from the database */
#define NFB$C_FC_SHOW	34	/* return specified field values */
#define NFB$C_FC_SET	35	/* set/modify the field values */
#define NFB$C_FC_CLEAR	36	/* clear specified field values */
#define NFB$C_FC_ZERCOU	37	/* zero (and optionally read) counters */
#define NFB$C_FC_LOOP	38	/* loop (used only to PSI to loop an x.25 line) */
#define NFB$C_FC_MAX	38	/* maximum FCT value

#define NFB$B_FLAGS	1	/* miscellaneous control flags */
#define NFB$V_ERRUPD	0
#define NFB$V_MULT	1
#define NFB$V_NOCTX	2
#define NFB$M_ERRUPD	1	/* update position context, even on error */
#define NFB$M_MULT	2	/* process as many entries as can be fit into P4 */
#define NFB$M_NOCTX	4	/* Don't update position context, even if successful
                                 * (used to stay on an entry for a while) This flag
                                 * overrides the ERRUPD flag.
                                 */

#define NFB$B_DATABASE	2	/* code identifying the database */
#define NFB$C_DB_LNI	1	/* local node */
#define NFB$C_DB_NDI	2	/* common nodes */
#define NFB$C_DB_OBI	3	/* network objects */
#define NFB$C_DB_CRI	4	/* circuits */
#define NFB$C_DB_PLI	5	/* lines */
#define NFB$C_DB_EFI	6	/* event logging filters */
#define NFB$C_DB_ESI	7	/* event logging sinks */
#define NFB$C_DB_LLI	8	/* logical-links */
#define NFB$C_DB_XNI	9	/* X.25 networks */
#define NFB$C_DB_XGI	10	/* X.25 groups */
#define NFB$C_DB_XDI	11	/* X.25 DTEs */
#define NFB$C_DB_XS5	12	/* X.25 server */
#define NFB$C_DB_XD5	13	/* X.25 destinations */
#define NFB$C_DB_XS9	14	/* X.29 server */
#define NFB$C_DB_XD9	15	/* X.29 destinations */
#define NFB$C_DB_XTI	16	/* X.25 trace facility */
#define NFB$C_DB_XTT	17	/* X.25 tracepoints */
#define NFB$C_DB_SPI	18	/* server process */
#define NFB$C_DB_AJI	19	/* adjaceny information */
#define NFB$C_DB_ARI	20	/* area information */
#define NFB$C_DB_XDTE	21	/* PSI reserved database */
#define NFB$C_DB_PSI2	22	/* PSI reserved database */
#define NFB$C_DB_PSI3	23	/* PSI reserved database */
#define NFB$C_DB_PSI4	24	/* PSI reserved database */
#define NFB$C_DB_PSI5	25	/* PSI reserved database */
#define NFB$C_DB_SDI	26	/* service DLE information */
#define NFB$C_DB_XAI	27	/* X.25 access database */
#define NFB$C_DB_XXX	28	/* last database definition for NFB$C_DB_MAX calc. */
#define NFB$C_DB_MAX	27	/* Maximum database value */

#define NFB$B_OPER	3	/* specifies the sense of the database search operation */
#define NFB$C_OP_EQL	0	/* match if SRCH_KEY EQL database entry field */
#define NFB$C_OP_GTRU	1	/* match if SRCH_KEY GTRU database field entry */
#define NFB$C_OP_LSSU	2	/* match if SRCH_KEY LSSU database field entry */
#define NFB$C_OP_NEQ	3	/* match if SRCH_KEY NEQ database field entry */
/* the following may only be used internally by NETACP */
#define NFB$C_OP_FNDMIN	4	/* find entry with minimum key value */
#define NFB$C_OP_FNDMAX	5	/* find entry with maximum key value */
#define NFB$C_OP_FNDPOS	6	/* find entry position in database */
#define NFB$C_OP_MAXFCT	3	/* maximum operator function */
#define NFB$C_OP_MAXINT	6	/* maximum internal function */

#define NFB$L_SRCH_KEY	4
/* Search key field identifier specifying the key used
 * to locate the entry in the database. This search is controlloed
 * by the sense of the NFB$B_OPER field.
 *
 * If this field has the value NFB$C_WILDCARD then the very next
 * entry in the list is assumed to be the target of the search.
 *
 * If this field is not specified (zero), then it
 * is assumed to be NFB$C_WILDCARD (no search key)
 */

#define NFB$L_SRCH2_KEY	8
/* secondary search key field identifier specifying the key used
 * to locate the entry in the database. This search is controlled by
 * the sense of the NFB$B_OPER2 field.
 *
 * If both SRCH_KEY and SRCH2_KEY are specified, then only those
 * database entries matching both search keys will be processed.
 *
 * If this field is not specified (zero) then it is assumed to be
 * NFB$C_WILDCARD (no search key)
 */

#define NFB$B_OPER2	12	/* specifies the sense of the
				 * database search operation
				 * when comparing against the SRCH2_KEY
				 */

#define NFB$B_MBZ1	13	/* must be zero */

#define NFB$W_CELL_SIZE	14
/* some of the field values found in the P4 buffer are
 * counted strings. If the "cell" size is non-zero, it
 * indicates the number of bytes which each string in
 * the P4 buffer occupies. If it is zero then string
 * fields are stores as variable length fields.
 */

#define NFB$L_FLDID	16
/* cell containing the first field i.d. -- the list
 * of field i.d.s begins here and continues to the
 * end of the structure.
 *
 * The list may be terminated before the end of the
 * structure by placing the value NFB$C_ENDOFLIST
 * in the longword following the last field i.d.
 */

#define NFB$K_LENGTH	16
#define NFB$C_LENGTH	16

/* define the field i.d. format */
#define NFB$L_PARAM_ID	0
#define NFB$M_INX	65535	/* Index into semantic table */
#define NFB$S_INX	16
#define NFB$V_INX	0
#define NFB$M_TYP	196608	/* Field type (string, bit, etc. */
#define NFB$C_TYP_BIT	0	/* field type for bits */
#define NFB$C_TYP_V	0	/* field type for bits */
#define NFB$C_TYP_LNG	1	/* field type for longs */
#define NFB$C_TYP_L	1	/* field type for longs */
#define NFB$C_TYP_STR	2	/* field type for strings */
#define NFB$C_TYP_S	2	/* field type for strings */
#define NFB$S_TYP	2
#define NFB$V_TYP	16
#define NFB$M_SPARE	16515072	/* reserved, must be zero */
#define NFB$S_SPARE	6
#define NFB$V_SPARE	18
#define NFB$M_DB	-16777216	/* database i.d. */
#define NFB$S_DB	8
#define NFB$V_DB	24

#define NFB$L_LNG_VALUE	0	/* longword value */
#define NFB$L_BIT_VALUE	0	/* longword value - bit 0 */
#define NFB$W_STR_COUNT	0	/* string - count */
#define NFB$B_STR_TEXT	2	/* string - text */

/* database parameter i.d.s */

/* define the NDI database */
/* boolean params */
#define NFB$C_NDI_LCK	33554433	/* set if conditionally writable fields are not writeable */
#define NFB$C_NDI_LOO	33554434	/* set if CNF is for a "loopback" node */
#define NFB$C_NDI_REA	33554435	/* set if node is reachable */
/* longword parameters */
#define NFB$C_NDI_TAD	33619984	/* "transformed address" - uses local node address
					 * for the local NDI (instead of zero as does ADD)
					 */
#define NFB$C_NDI_CTA	33619985	/* absolute due time for logging counters */
#define NFB$C_NDI_ADD	33619986	/* address */
#define NFB$C_NDI_CTI	33619987	/* counter timer */
#define NFB$C_NDI_ACL	33619988	/* active links */
#define NFB$C_NDI_DEL	33619989	/* delay */
#define NFB$C_NDI_DTY	33619990	/* destination type */
#define NFB$C_NDI_DCO	33619991	/* destination cost */
#define NFB$C_NDI_DHO	33619992	/* destination hops */
#define NFB$C_NDI_SDV	33619993	/* service device */
#define NFB$C_NDI_CPU	33619994	/* CPU type */
#define NFB$C_NDI_STY	33619995	/* software type */
#define NFB$C_NDI_DAD	33619996	/* dump address */
#define NFB$C_NDI_DCT	33619997	/* dump count */
#define NFB$C_NDI_OHO	33619998	/* host */
#define NFB$C_NDI_IHO	33619999	/* host */
#define NFB$C_NDI_ACC	33620000	/* access switch (inbound, outbound, etc.) */
#define NFB$C_NDI_PRX	33620001	/* ** obsolete ** (node proxy parameter) */
#define NFB$C_NDI_NND	33620002	/* next node address */
#define NFB$C_NDI_SNV	33620003	/* service node version */
#define NFB$C_NDI_INB	33620004	/* async line - inbound node type */
/* string parameters */
#define NFB$C_NDI_COL  	33685568	/* collating sequnce */
#define NFB$C_NDI_HAC	33685569	/* node address/loop linename combination */
#define NFB$C_NDI_CNT	33685570	/* counters */
#define NFB$C_NDI_NNA	33685571	/* name */
#define NFB$C_NDI_SLI	33685572	/* service line */
#define NFB$C_NDI_SPA	33685573	/* service password */
#define NFB$C_NDI_LOA	33685574	/* load file */
#define NFB$C_NDI_SLO	33685575	/* secondary loader */
#define NFB$C_NDI_TLO	33685576	/* tertiary loader */
#define NFB$C_NDI_SID	33685577	/* software i.d. */
#define NFB$C_NDI_DUM	33685578	/* dump file */
#define NFB$C_NDI_SDU	33685579	/* secondary dumper */
#define NFB$C_NDI_NLI	33685580	/* loopback line */
#define NFB$C_NDI_DLI	33685581	/* destination line */
#define NFB$C_NDI_PUS	33685582	/* privileged user id */
#define NFB$C_NDI_PAC	33685583	/* privileged account */
#define NFB$C_NDI_PPW	33685584	/* privileged password */
#define NFB$C_NDI_NUS	33685585	/* non-priv user id */
#define NFB$C_NDI_NAC	33685586	/* non-priv user account */
#define NFB$C_NDI_NPW	33685587	/* non-priv user password */
#define NFB$C_NDI_RPA	33685588	/* recieve password */
#define NFB$C_NDI_TPA	33685589	/* trasnsmit password */
#define NFB$C_NDI_DFL	33685590	/* diagnostic load file */
#define NFB$C_NDI_HWA	33685591	/* hardware NI address (ROM address) */
#define NFB$C_NDI_LPA	33685592	/* loop assistant NI address */
#define NFB$C_NDI_NNN	33685593	/* next node name to destination (goes with nnd) */
#define NFB$C_NDI_LAA	33685594	/* node assist agent */
#define NFB$C_NDI_LAP	33685595	/* load assist parameter */

/* define field parameters for each parameter in the LNI database */
/* boolean parameters */
#define NFB$C_LNI_LCK	16777217	/* set if conditionally writable fields are not writeable */
#define NFB$C_LNI_ALI	16777218	/* set if ALIAS INBOUND has been enabled */
/* longword parameters */
#define NFB$C_LNI_ADD	16842768	/* address */
#define NFB$C_LNI_ACL	16842769	/* Total number of active links */
#define NFB$C_LNI_ITI	16842770	/* Incoming timer */
#define NFB$C_LNI_OTI	16842771	/* outgoing timer */
#define NFB$C_LNI_STA	16842772	/* State */
#define NFB$C_LNI_MLK	16842773	/* Maximum links */
#define NFB$C_LNI_DFA	16842774	/* delay factor */
#define NFB$C_LNI_DWE	16842775	/* delay weight */
#define NFB$C_LNI_IAT	16842776	/* Inactivity timer */
#define NFB$C_LNI_RFA	16842777	/* Retransmit factor */
#define NFB$C_LNI_ETY	16842778	/* Executor type */
#define NFB$C_LNI_RTI	16842779	/* Routing timer */
#define NFB$C_LNI_RSI	16842780	/* Routing suppression timer */
#define NFB$C_LNI_SAD	16842781	/* Subaddress - low word = low limit, high word = high limit */
#define NFB$C_LNI_MAD	16842782	/* Maximum address */
#define NFB$C_LNI_MLN	16842783	/* Maximum lines */
#define NFB$C_LNI_MCO	16842784	/* Maximum costs */
#define NFB$C_LNI_MHO	16842785	/* Maximum hops */
#define NFB$C_LNI_MVI	16842786	/* Maximum visits */
#define NFB$C_LNI_MBU	16842787	/* Maximum buffers */
#define NFB$C_LNI_BUS	16842788	/* Forwarding buffer size */
#define NFB$C_LNI_LPC	16842789	/* Loop count */
#define NFB$C_LNI_LPL	16842790	/* Loop length */
#define NFB$C_LNI_LPD	16842791	/* Loop data type */
#define NFB$C_LNI_DAC	16842792	/* Default access switch (inbound, outbound, etc.) */
#define NFB$C_LNI_DPX	16842793	/* default proxy switch (inbound, outbound, etc. */
#define NFB$C_LNI_PIQ	16842794	/* Pipeline quota */
#define NFB$C_LNI_LPH	16842795	/* Loop help type of assistance given to loop requestors */
#define NFB$C_LNI_BRT	16842796	/* Broadcast routing timer */
#define NFB$C_LNI_MAR	16842797	/* Maximum areas */
#define NFB$C_LNI_MBE	16842798	/* Maximum nonrouters on NI */
#define NFB$C_LNI_MBR	16842799	/* Maximum routers on NI */
#define NFB$C_LNI_AMC	16842800	/* Area maximum cost */
#define NFB$C_LNI_AMH	16842801	/* area maximum hops */
#define NFB$C_LNI_SBS	16842802	/* Segment buffer size */
#define NFB$C_LNI_ALA	16842803	/* Alias local node address (cluster address) */
#define NFB$C_LNI_ALM	16842804	/* Alias maximum links */
/* string parameters */
#define NFB$C_LNI_COL	16908352	/* Collating fields */
#define NFB$C_LNI_NAM  	16908353	/* Local node name */
#define NFB$C_LNI_CNT	16908354	/* Counters */
#define NFB$C_LNI_IDE	16908355	/* Identification */
#define NFB$C_LNI_MVE	16908356	/* Management version */
#define NFB$C_LNI_NVE	16908357	/* NSP version */
#define NFB$C_LNI_RVE	16908358	/* Routing version */
#define NFB$C_LNI_PHA	16908359	/* Physical NI address (current address) */

/* define field parameters for each parameter in the OBI database */
/* boolean parameters */
#define NFB$C_OBI_LCK	50331649	/* set if conditionally writable fields are not writeable */
#define NFB$C_OBI_SET	50331650	/* set if a "set" qio has ever modified the CNF. If
					   not then the CNF was due to a "declare name/object"
					   only and may be deleted when the declaring process
					   breaks the channel over which the object was declared */
#define NFB$C_OBI_ALO	50331651	/* Alias outgoing enabled/disabled */
#define NFB$C_OBI_ALI	50331652	/* Alias incoming enabled/disabled */
/* longword parameters */
#define NFB$C_OBI_LPR	50397200	/* low order privileges */
#define NFB$C_OBI_HPR	50397201	/* high order privileges */
#define NFB$C_OBI_UCB	50397202	/* Owner's UCB address */
#define NFB$C_OBI_CHN	50397203	/* Owner's channel */
#define NFB$C_OBI_NUM	50397204	/* number */
#define NFB$C_OBI_PID	50397205	/* Process I.D. */
#define NFB$C_OBI_PRX	50397206	/* Proxy login switch (inbound, outbound, etc) */
/* string parameters */
#define NFB$C_OBI_COL  	50462784	/* collating field */
#define NFB$C_OBI_ZNA	50462785	/* zero obj+name identifier */
#define NFB$C_OBI_SFI	50462786	/* Parsed file i.d. */
#define NFB$C_OBI_IAC	50462787	/* Default inbound combined access control string */
#define NFB$C_OBI_NAM	50462788	/* Name */
#define NFB$C_OBI_FID	50462789	/* File ID */
#define NFB$C_OBI_USR	50462790	/* User ID */
#define NFB$C_OBI_ACC	50462791	/* Account */
#define NFB$C_OBI_PSW	50462792	/* Password */

/* define field parameters for each parameter in the CRT database
 *  - use
 *    ----
 *  C = common
 *  E = Executor (used by Transport)
 *  X = Native X.25 network management
 *  D = DECnet (not X.25)
 */
/* boolean parameters */
#define NFB$C_CRI_LCK	67108865	/* D Set if conditionally writeable fields are not writeable */
#define NFB$C_CRI_SER	67108866	/* D set if service functions not allowed */
#define NFB$C_CRI_BLK	67108867	/* E Blocking */
#define NFB$C_CRI_VER_FILL	67108868	/* Filler (VER retired) */
#define NFB$C_CRI_DLM	67108869	/* E Circuit to be used as X.25 datalink, if set
					 * If clear, circuit is for native X.25 use */
/* longword parameters */
#define NFB$C_CRI_OWPID	67174416	/* D PID of temp owner of line in service state */
#define NFB$C_CRI_CTA	67174417	/* D absolute due time for counter logging */
#define NFB$C_CRI_SRV	67174418	/* D service substate qualifier */
#define NFB$C_CRI_STA	67174419	/* C State */
#define NFB$C_CRI_SUB	67174420	/* C Substate */
#define NFB$C_CRI_LCT	67174421	/* C Counter timer */
#define NFB$C_CRI_PNA	67174422	/* E Adjacent node address */
#define NFB$C_CRI_BLO	67174423	/* E Partner's receive block size */
#define NFB$C_CRI_COS	67174424	/* E Cost */
#define NFB$C_CRI_HET	67174425	/* E Hello timer */
#define NFB$C_CRI_LIT	67174426	/* E Listen timer */
#define NFB$C_CRI_MRC	67174427	/* E Maximum recalls */
#define NFB$C_CRI_RCT	67174428	/* E Recall timer */
#define NFB$C_CRI_POL	67174429	/* D Polling state */
#define NFB$C_CRI_PLS	67174430	/* D Polling substate */
#define NFB$C_CRI_USE	67174431	/* X Usage */
#define NFB$C_CRI_TYP	67174432	/* C Type */
#define NFB$C_CRI_CHN	67174433	/* X X.25 Channel */
#define NFB$C_CRI_MBL	67174434	/* X Maximum block */
#define NFB$C_CRI_MWI	67174435	/* X Maximum window */
#define NFB$C_CRI_TRI	67174436	/* D Tributary */
#define NFB$C_CRI_BBT	67174437	/* D Babble timer */
#define NFB$C_CRI_TRT	67174438	/* D Transmit timer */
#define NFB$C_CRI_MRB	67174439	/* D Maximum receive buffers */
#define NFB$C_CRI_MTR	67174440	/* D Maximum transmits */
#define NFB$C_CRI_ACB	67174441	/* D Active base */
#define NFB$C_CRI_ACI	67174442	/* D active increment */
#define NFB$C_CRI_IAB	67174443	/* D Inactive base */
#define NFB$C_CRI_IAI	67174444	/* D inactive increment */
#define NFB$C_CRI_IAT	67174445	/* D Inactive threshold */
#define NFB$C_CRI_DYB	67174446	/* D Dying base */
#define NFB$C_CRI_DYI	67174447	/* D Dying increment */
#define NFB$C_CRI_DYT	67174448	/* D Dying threshold */
#define NFB$C_CRI_DTH	67174449	/* D Dead threshold */
#define NFB$C_CRI_MST	67174450	/* D Maintenance mode state (0 => On, 1=>off) */
#define NFB$C_CRI_XPT	67174451	/* E Transport protocol to use */
#define NFB$C_CRI_MRT	67174452	/* E Maximum routers on this NI */
#define NFB$C_CRI_RPR	67174453	/* E Router priority */
#define NFB$C_CRI_DRT	67174454	/* E Designated router on NI (node address) */
#define NFB$C_CRI_VER	67174455	/* D Verification Enabled/Disabled/Inbound on circuit */
/* string parameters */
#define NFB$C_CRI_COL	67240000	/* D Collating field */
#define NFB$C_CRI_NAM	67240001	/* C Circuit name */
#define NFB$C_CRI_VMSNAM	67240002	/* D Device name in VMS format */
#define NFB$C_CRI_CHR	67240003	/* D Characteristics buffer for startup control QIO */
#define NFB$C_CRI_CNT	67240004	/* C Counters */
#define NFB$C_CRI_P2P	67240005	/* D Line's PhaseII Partner name (for loopback) */
#define NFB$C_CRI_LOO	67240006	/* E Loopback name */
#define NFB$C_CRI_PNN	67240007	/* E Adjacent node name */
#define NFB$C_CRI_NUM	67240008	/* X Call number */
#define NFB$C_CRI_DTE	67240009	/* X DTE */
#define NFB$C_CRI_DEVNAM	67240010	/* D Device name in VMS format, with unit included */
#define NFB$C_CRI_NET	67240011	/* XD Network name */

/* define field parameters for each parameter in the PLI database
 * c = common
 * l = LAPB (X.25)
 * D = DDCMP (not X.25)
 * E = ethernet
 */
/* boolean parameters */
#define NFB$C_PLI_LCK	83886081	/* D set if conditionally writable fields are not writeable */
#define NFB$C_PLI_SER	83886082	/* D Service */
#define NFB$C_PLI_DUP	83886083	/* C Duplex (set if half) */
#define NFB$C_PLI_CON	83886084	/* C Controller (set if loopback) */
#define NFB$C_PLI_CLO	83886085	/* C Clock mode (set if internal) */
#define NFB$C_PLI_SWI	83886086	/* D Async line - switch */
#define NFB$C_PLI_HNG	83886087	/* D Async line - Hangup */
/* longword parameters */
#define NFB$C_PLI_CTA	83951632	/* D Absolute time for counter read and clear */
#define NFB$C_PLI_STA	83951633	/* C state */
#define NFB$C_PLI_SUB	83951634	/* C substate */
#define NFB$C_PLI_LCT	83951635	/* D counter timer */
#define NFB$C_PLI_PRO	83951636	/* C protocol */
#define NFB$C_PLI_STI	83951637	/* D service timer */
#define NFB$C_PLI_HTI	83951638	/* L holdback timer */
#define NFB$C_PLI_MBL	83951639	/* L maximum block */
#define NFB$C_PLI_MRT	83951640	/* L maximum retransmits */
#define NFB$C_PLI_MWI	83951641	/* L maximum window */
#define NFB$C_PLI_SLT	83951642	/* D scheduling timer */
#define NFB$C_PLI_DDT	83951643	/* D Dead timer */
#define NFB$C_PLI_DLT	83951644	/* D delay timer */
#define NFB$C_PLI_SRT	83951645	/* D Stream timer */
#define NFB$C_PLI_BFN	83951646	/* D Receive buffers */
#define NFB$C_PLI_BUS	83951647	/* D Action routine returns bufsiz used for line */
#define NFB$C_PLI_PLVEC	83951648	/* D PLVEC i.d. */
#define NFB$C_PLI_RTT	83951649	/* D Retransmit timer */
#define NFB$C_PLI_MOD	83951650	/* L X.25 mode (DCE, DTE, etc) */
#define NFB$C_PLI_LPC	83951651	/* L loop count */
#define NFB$C_PLI_LPL	83951652	/* L loop length */
#define NFB$C_PLI_LPD	83951653	/* L loop data type */
#define NFB$C_PLI_EPT	83951654	/* E Ethernet protocol type for datalink */
#define NFB$C_PLI_LNS	83951655	/* D async line - line speed */
#define NFB$C_PLI_BFS	83951656	/* C line buffer size (overrides executor bufsiz) */
#define NFB$C_PLI_TPI	83951657	/* D Transmit pipeline */
/* string parameters */
#define NFB$C_PLI_COL  	84017216	/* D collating field */
#define NFB$C_PLI_NAM	84017217	/* C line name */
#define NFB$C_PLI_VMSNAM	84017218	/* D device name in VMS format */
#define NFB$C_PLI_CHR	84017219	/* D set-mode QIO line characteristics buffer */
#define NFB$C_PLI_CNT	84017220	/* C counters */
#define NFB$C_PLI_MCD	84017221	/* L filespec for microcode dump (initiates dump) */
#define NFB$C_PLI_HWA	84017222	/* D NI hardware address (ROM address) */
#define NFB$C_PLI_DEVNAM	84017223	/* D device name in VMS format, with unit included */
#define NFB$C_PLI_NET	84017224	/* L Network name */

/* define field parameters for each parameter in the EFI database */
/* boolean parameters */
#define NFB$C_EFI_LCK	100663297	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_EFI_SIN	100728848
#define NFB$C_EFI_SP1	100728849
#define NFB$C_EFI_B1	100728850
#define NFB$C_EFI_B2	100728851
/* string parameters */
#define NFB$C_EFI_COL	100794432	/* collating field */
#define NFB$C_EFI_EVE	100794433
#define NFB$C_EFI_SB1	100794434
#define NFB$C_EFI_SB2	100794435
#define NFB$C_EFI_SB3	100794436

/* define field parameters for each parameter in the ESI database */
/* boolean parameters */
#define NFB$C_ESI_LCK	117440513	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_ESI_SNK	117506064
#define NFB$C_ESI_STA	117506065
#define NFB$C_ESI_SP1	117506066
#define NFB$C_ESI_B1	117506067
#define NFB$C_ESI_B2	117506068
/* string parameters */
#define NFB$C_ESI_COL	117571648	/* collating field */
#define NFB$C_ESI_LNA	117571649
#define NFB$C_ESI_SB1	117571650
#define NFB$C_ESI_SB2	117571651
#define NFB$C_ESI_SB3	117571652

/* define field parameters for each parameter in the LLI database */
/* boolean parameters */
#define NFB$C_LLI_LCK	134217729	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_LLI_DLY	134283280	/* Round trip delay time */
#define NFB$C_LLI_STA	134283281	/* State */
#define NFB$C_LLI_LLN	134283282	/* Local link number */
#define NFB$C_LLI_RLN	134283283	/* Rmote link number */
#define NFB$C_LLI_PNA	134283284	/* Partner's node address */
#define NFB$C_LLI_PID	134283285	/* External process i.d. */
#define NFB$C_LLI_IPID	134283286	/* Internal process i.d. */
#define NFB$C_LLI_XWB	134283287	/* pointer to XWB */
#define NFB$C_LLI_CNT	134283288	/* counters */
/* string parameters */
#define NFB$C_LLI_COL  	134348864	/* collating field */
#define NFB$C_LLI_USR	134348865	/* user name */
#define NFB$C_LLI_PRC	134348866	/* process name */
#define NFB$C_LLI_PNN	134348867	/* partner's node name */
#define NFB$C_LLI_RID	134348868	/* partner's process i.d. */

/* X.25 network parameters (part of MODULE X-25 Protocol) */
/* define field parameters for each parameter in the XNI database */
/* boolean parameters */
#define NFB$C_XNI_LCK	150994945	/* set if conditionally writeable fields are not writeable */
#define NFB$C_XNI_MNS_FILL	150994946	/* X.25 multi-network support (set if enabled) [no longer used] */
/* longword parameters */
#define NFB$C_XNI_CAT	151060496	/* Call timer */
#define NFB$C_XNI_CLT	151060497	/* Clear timer */
#define NFB$C_XNI_DBL	151060498	/* Default data */
#define NFB$C_XNI_DWI	151060499	/* Default window */
#define NFB$C_XNI_MBL	151060500	/* Maximum data */
#define NFB$C_XNI_MCL	151060501	/* Maximum clears */
#define NFB$C_XNI_MRS	151060502	/* Maximum resets */
#define NFB$C_XNI_MST	151060503	/* Maximum restarts */
#define NFB$C_XNI_MWI	151060504	/* Maximum window */
#define NFB$C_XNI_RST	151060505	/* Rest timer */
#define NFB$C_XNI_STT	151060506	/* restart timer */
/* string parameters */
#define NFB$C_XNI_COL  	151126080	/* collating field */
#define NFB$C_XNI_NETENT	151126081	/* Network */
#define NFB$C_XNI_PROF	151126082	/* Profile name */

/* X.25 DTE parameters (qualified by a given network) */
/* define field parameters for each parameter in the XDI database */
/* boolean parameters */
#define NFB$C_XDI_LCK 	184549377	/* set if conditionnaly writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XDI_ACH	184614928	/* Active channels */
#define NFB$C_XDI_ASW	184614929	/* Active switched */
#define NFB$C_XDI_CTM	184614930	/* Counter timer */
#define NFB$C_XDI_MCH	184614931	/* Maximum channels */
#define NFB$C_XDI_STA	184614932	/* State */
#define NFB$C_XDI_SUB	184614933	/* Substate */
#define NFB$C_XDI_MCI	184614934	/* Maximum circuits [VMS only] */
#define NFB$C_XDI_CAT	184614935	/* Call timer */
#define NFB$C_XDI_CLT	184614936	/* Clear timer */
#define NFB$C_XDI_DBL	184614937	/* Default data */
#define NFB$C_XDI_DWI	184614938	/* Default window */
#define NFB$C_XDI_MBL	184614939	/* Maximum data */
#define NFB$C_XDI_MCL	184614940	/* maximum clears */
#define NFB$C_XDI_MRS	184614941	/* maximum resets */
#define NFB$C_XDI_MST	184614942	/* maximum restarts */
#define NFB$C_XDI_MWI	184614943	/* maximum window */
#define NFB$C_XDI_RST	184614944	/* reset timer */
#define NFB$C_XDI_STT	184614945	/* restart timer */
#define NFB$C_XDI_MODE	184614946	/* DTE mode */
#define NFB$C_XDI_ITT	184614947	/* Interrupt timer */
/* string parameters */
#define NFB$C_XDI_COL  	184680512	/* collating field */
#define NFB$C_XDI_DTE	184680513	/* DTE address */
#define NFB$C_XDI_CHN	184680514	/* Channels */
#define NFB$C_XDI_LIN	184680515	/* Line */
#define NFB$C_XDI_DNT	184680516	/* Network */
#define NFB$C_XDI_CNT	184680517	/* Counters */

/* X.25 group parameters (qualified by a given DTE) */
/* define field parameters for each parameter in the XGI database */
/* boolean parameters */
#define NFB$C_XGI_LCK	167772161	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XGI_GNM	167837712	/* group number */
#define NFB$C_XGI_GTY	167837713	/* Group type */
/* string parameters */
#define NFB$C_XGI_COL  	167903296	/* collating field. this field must be unique accross
					   all entries in this database. It consists of the
					   group name string followed by the DTE address. */
#define NFB$C_XGI_GRP	167903297	/* Group name */
#define NFB$C_XGI_GDT	167903298	/* Group DTE address */
#define NFB$C_XGI_GNT	167903299	/* Group network */

/* X.25 server parameters (global parameters for all destinations) */
/* define field parameters for each parameter in the XS5 database */
/* boolean parameters */
#define NFB$C_XS5_LCK	201326593	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XS5_MCI	201392144	/* Maximum circuits allowed */
#define NFB$C_XS5_STA	201392145	/* State */
#define NFB$C_XS5_ACI	201392146	/* Active circuits */
#define NFB$C_XS5_CTM	201392147	/* Counter timer */
/* string parameters */
#define NFB$C_XS5_COL  	201457728	/* collating field. Must be unique */
#define NFB$C_XS5_CNT	201457729	/* Counters */
                                           
/* X.25 destination parameters (part of MODULE X25-SERVER) */
/* define field parameters for each parameter in the XD5 database */
/* boolean parameters */
#define NFB$C_XD5_LCK	218103809	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XD5_PRI	218169360	/* Priority */
#define NFB$C_XD5_SAD	218169361	/* subaddress range (low word = low limit, high word = high limit */
#define NFB$C_XD5_NOD	218169362	/* remote node address containing server (gateways only) */
#define NFB$C_XD5_RED	218169363	/* redirect reason */
/* string parameters */
#define NFB$C_XD5_COL	218234944	/* collating field. must be unique */
#define NFB$C_XD5_DST	218234945	/* Destination DTE address */
#define NFB$C_XD5_CMK	218234946	/* call mask */
#define NFB$C_XD5_CVL	218234947	/* call value */
#define NFB$C_XD5_GRP	218234948	/* group name */
#define NFB$C_XD5_SDTE	218234949	/* sending DTE address (formally number) */
#define NFB$C_XD5_OBJ	218234950	/* && object name */
#define NFB$C_XD5_FIL	218234951	/* Command procedure to execute when starting object */
#define NFB$C_XD5_USR	218234952	/* user name */
#define NFB$C_XD5_PSW	218234953	/* password */
#define NFB$C_XD5_ACC	218234954	/* account */
#define NFB$C_XD5_CDTE	218234955	/* called DTE */
#define NFB$C_XD5_RDTE	218234956	/* receiving DTE */
#define NFB$C_XD5_NET	218234957	/* network */
#define NFB$C_XD5_EMK	218234958	/* extension mask */
#define NFB$C_XD5_EVL	218234959	/* extension value */
#define NFB$C_XD5_ACL	218234960	/* acl, a list of ace;structure, part of ORB */
#define NFB$C_XD5_IDTE	218234961	/* incoming address */

/* X.29 server parameters (global parameters for all destinations ) */
/* define field parameters for each parameter in the XS9 database */
/* boolean parameters */
#define NFB$C_XS9_LCK	234881025	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XS9_MCI	234946576	/* Maximum circuits allowed */
#define NFB$C_XS9_STA	234946577	/* State */
#define NFB$C_XS9_ACI	234946578	/* Active circuits */
#define NFB$C_XS9_CTM	234946579	/* Counter timer */
/* string parameters */
#define NFB$C_XS9_COL  	235012160	/* collating field, must be unique */
#define NFB$C_XS9_CNT	235012161	/* counters */

/* X.29 destination parameters (part of MODULE X29-SERVER) */
/* define field parameters for each parameter in the XD9 database */
/* boolean parameters */
#define NFB$C_XD9_LCK	251658241	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XD9_PRI	251723792	/* Priority */
#define NFB$C_XD9_SAD	251723793	/* subaddress range (low word = low limit, high word = high limit) */
#define NFB$C_XD9_NOD	251723794	/* remote node address containing server (gateways only) */
#define NFB$C_XD9_RED	251723795	/* redirect reason */
/* string parameters */
#define NFB$C_XD9_COL  	251789376	/* collating field, must be unique */
#define NFB$C_XD9_DST	251789377	/* Destination DTE address */
#define NFB$C_XD9_CMK	251789378	/* call mask */
#define NFB$C_XD9_CVL	251789379	/* call value */
#define NFB$C_XD9_GRP	251789380	/* group name */
#define NFB$C_XD9_SDTE	251789381	/* sending DTE */
#define NFB$C_XD9_OBJ	251789382	/* && object name */
#define NFB$C_XD9_FIL	251789383	/* command procedure to execute when starting object */
#define NFB$C_XD9_USR	251789384	/* user name */
#define NFB$C_XD9_PSW	251789385	/* password */
#define NFB$C_XD9_ACC	251789386	/* account */
#define NFB$C_XD9_CDTE	251789387	/* caller DTE */
#define NFB$C_XD9_RDTE	251789388	/* receiving DTE */
#define NFB$C_XD9_NET	251789389	/* network */
#define NFB$C_XD9_EMK	251789390	/* extension mask */
#define NFB$C_XD9_EVL	251789391	/* extension value */
#define NFB$C_XD9_ACL	251789392	/* ACL, a list of ACE'structure, part of ORB */
#define NFB$C_XD9_IDTE	251789393	/* incoming address */

/* X.25 tracing facility (global) parameters */
/* define field parameters for each parameter in the XTI database */
/* boolean parameters */
#define NFB$C_XTI_LCK	268435457	/* set if conditionally writeable fields are not writeable */
/* longword parameters */
#define NFB$C_XTI_STA	268501008	/* State */
#define NFB$C_XTI_BFZ	268501009	/* buffer size */
#define NFB$C_XTI_CPL	268501010	/* capture limit */
#define NFB$C_XTI_MBK	268501011	/* maximum blocks/file */
#define NFB$C_XTI_MBF	268501012	/* maximum number of buffers */
#define NFB$C_XTI_MVR	268501013	/* maximum trace file version number */
/* string parameters */
#define NFB$C_XTI_COL	268566592	/* collating field, must be unique */
#define NFB$C_XTI_FNM	268566593	/* trace file name */

/* X.25 tracepont (local) parameters */
/* define field parameters for each parameter in the XTT database */
/* boolean parameters */
#define NFB$C_XTT_LCK	285212673	/* set if conditionally writeable fields are note writeable */
/* longword parameters */
#define NFB$C_XTT_TST	285278224	/* state */
#define NFB$C_XTT_CPS	285278225	/* capture size */
/* string parameters */
#define NFB$C_XTT_COL	285343808	/* collating field, must be unique */
#define NFB$C_XTT_TPT	285343809	/* tracepoint name */

/* X.25 access (qualified by a given network) */
/* define field parameters for each parameter in the XAI database */
/* boolean parameters */
#define NFB$C_XAI_LCK	452984833	/* set if conditionally writeable fields are note writeable */
/* longword parameters */
#define NFB$C_XAI_NDA	453050384	/* node address */
/* string parameters */
#define NFB$C_XAI_COL	453115968	/* collating field */
#define NFB$C_XAI_NET	453115969	/* network */
#define NFB$C_XAI_USR	453115970	/* user id */
#define NFB$C_XAI_PSW	453115971	/* password */
#define NFB$C_XAI_ACC	453115972	/* account */
#define NFB$C_XAI_NOD	453115973	/* node id */

/* X.25 security (qualified by a given network) */
/* define field parameters for each parameter in the XDTE database */
/* boolean parameters */
#define NFB$C_XDTE_LCK	352321537	/* set if conditionally writeable fields are note writeable */
/* string parameters */
#define NFB$C_XDTE_COL	352452672	/* collating field */
#define NFB$C_XDTE_NET	352452673	/* network */
#define NFB$C_XDTE_DTE	352452674	/* DTE address */
#define NFB$C_XDTE_ID	352452675	/* ID list, ARB rights list */
#define NFB$C_XDTE_ACL	352452676	/* ACL, a list of ACE's, part of ORB */

/* define SPI (server process) parameters */
/* boolean parameters */
#define NFB$C_SPI_LCK	301989889	/* set if conditionally writeable fields are note writeable */
#define NFB$C_SPI_PRL	301989890	/* proxy flag which initially started server process */
/* longword parameters */
#define NFB$C_SPI_PID	302055440	/* Server PID */
#define NFB$C_SPI_IRP	302055441	/* IRP of waiting DECLSERV QIO (0 if proceess active) */
#define NFB$C_SPI_CHN	302055442	/* Channel associated with DECLSERV IRP */
#define NFB$C_SPI_RNA	302055443	/* Remote node address which initially started server process */
/* string parameters */
#define NFB$C_SPI_COL	302121024	/* collating field */
#define NFB$C_SPI_ACS	302121025	/* ACS used to initially start server process */
#define NFB$C_SPI_RID	302121026	/* Remote user id which initially started server */
#define NFB$C_SPI_SFI	302121027	/* last (current) SFI given to server process */
#define NFB$C_SPI_NCB	302121028	/* last (current) NCB given to server process */
#define NFB$C_SPI_PNM	302121029	/* last (current) process name given to server */

/* define AJI (adjacency) parameters */
/* boolean parameters */
#define NFB$C_AJI_LCK	318767105	/* set if conditionally writeable fields are note writeable */
#define NFB$C_AJI_REA	318767106	/* reachable (set if two way communication established) */
#define NFB$C_AJI_RRA	318767107
/* longword parameters */
#define NFB$C_AJI_ADD	318832656	/* Node address */
#define NFB$C_AJI_TYP	318832657	/* node type */
#define NFB$C_AJI_LIT	318832658	/* listen timer for this adjacency */
#define NFB$C_AJI_BLO	318832659	/* partner's block size */
#define NFB$C_AJI_RPR	318832660	/* partner's router priority (on NI) */
/* string parameters */
#define NFB$C_AJI_COL	318898240	/* collating field */
#define NFB$C_AJI_NNA	318898241	/* node name */
#define NFB$C_AJI_CIR	318898242	/* circuit name */

/* define SDI (service DLE) parameters */
/* boolean parameters */
#define NFB$C_SDI_LCK	436207617	/* set if conditionally writeable fields are note writeable */
/* longword parameters */
#define NFB$C_SDI_SUB	436273168	/* service substate */
#define NFB$C_SDI_PID	436273169	/* PID of process owning this DEL link */
/* string parameters */
#define NFB$C_SDI_COL	436338752	/* collating field */
#define NFB$C_SDI_CIR	436338753	/* circuit name */
#define NFB$C_SDI_PHA	436338754	/* service physical address (BC only) */
#define NFB$C_SDI_PRC	436338755	/* nameof process owning this DLE link */

/* define ARI (Area database) (read only) for level 2 Phase IV routers only.parameters */
/* boolean parameters */
#define NFB$C_ARI_LCK	335544321	/* set if conditionally writeable fields are note writeable */
#define NFB$C_ARI_REA	335544322	/* set if node is reachable */
/* longword parameters */
#define NFB$C_ARI_ADD	335609872	/* address */
#define NFB$C_ARI_DCO	335609873	/* destination cost */
#define NFB$C_ARI_DHO	335609874	/* destination hops */
#define NFB$C_ARI_NND	335609875	/* next node address */
/* string parameters */
#define NFB$C_ARI_COL	335675456	/* collating field */
#define NFB$C_ARI_DLI	335675457	/* circuit used for normal traffic to area */

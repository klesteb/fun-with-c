
#ifndef _PROTOS_H
#define _PROTOS_H

/* 210ctdtl.c */

extern char doAide(char moreYet, char first);
extern void doChat(char moreYet, char first);
extern void doEnter(char moreYet, char first);
extern void doGoto(char expand, char first);
extern void doHelp(char expand, char first);
extern void doKnown(char expand, char first);
extern void doLogin(char moreYet, char first);
extern void doLogout(char expand, char first);
extern void doRead(char moreYet, char first);
extern char doRegular(char x, char c);
extern char doSysop(char c, char first);
extern char getCommand(char *c);
extern void greeting(void);

/* 210log.c */

extern void crypte(char *buf, unsigned len, unsigned seed);
extern void getLog(struct logBuffer *lBuf, int n);
extern int hash(char *str);
extern void login(char *password);
extern void logInit(void);
extern void newPW(void);
extern void newUser(void);
extern void noteLog(void);
extern void putLog(struct logBuffer *lBuf, int n);
extern void slideLTab(int slot, int last);
extern void sortLog(void );
extern void storeLog(void);
extern void terminate(char discon);
extern void zapLogFile(void);
extern int  PWSlot(char *pw);

/* 210misc.c */

extern void doCR(void);
extern char visible(char c);
extern void configure(void);
extern int  tutorial(char *filename);
extern void printDate(int year, int month, int day);
extern void getDate(int *year, int *month, int *day);

/* 210modem.c */

extern char BBSCharReady(void);
extern int  getCh(void);
extern void putCh(void);
extern int  iChar(void);
extern char KBReady(void);
extern void modemInit(void);
extern void putChar(char c);
extern void ringSysop(void);
extern void upLoad(void);

/* 210msg.c */

extern void aideMessage(char noteDeletedMessage);
extern char dGetWord(char *dest, int lim);
extern void dPrintf(char *format, ...);
extern void fakeFullCase(char *text);
extern int  findPerson(char *name, struct logBuffer *lBuf);
extern void flushMsgBuf(void);
extern void getMessage(void);
extern char getMsgChar(void);
extern void getMsgStr(char *dest, int lim);
extern int  getWord(char *dest, char *source, int offset, int lim);
extern char mAbort(void);
extern int  makeMessage(char uploading);
extern void mFormat(char *string);
extern void mPeek(void);
extern void mWCprintf(char *format, ...);
extern void msgInit(void);
extern void noteLogMessage(struct logBuffer *lBuf, int logNo);
extern void note2Message(int id, int loc);
extern void printMessage(int loc, unsigned id);
extern void pullIt(int m);
extern char putMessage(char uploading);
extern int  putMsgChar(char c);
extern void putWord(char *st);
extern void showMessages(char whichMess, char revOrder);
extern void startAt(int sect, int byt);
extern void unGetMsgChar(char c);
extern void zapMsgFile(void);
extern void noteMessage(struct logBuffer *lBuf, int logNo);

/* 210rooma.c */

extern void dumpRoom(void);
extern int  fileDir(char *fileName);
extern char gotoRoom(char *nam);
extern void init(void);
extern void initCitadel(void);
extern void listRooms(char doDull);
extern void openFile(char *filename, int *fd);
extern int  roomExists(char *room);
extern void setSpace(char disk, char user);
extern void setUp(char justIn);
extern void systat(void);
extern void unspace(char *from, char *to);
extern void wildCard(int (*fn)(char *), char *filename);
extern void fillMailRoom(void);

/* 210roomb.c */

extern int  editText(char *buf, int lim);
extern int  findRoom(void);
extern int  getNumber(char *prompt, unsigned bottom, unsigned top);
extern void getRoom(int rm, struct roomBuffer *buf);
extern void getString(char *prompt, char *buf, int lim);
extern char getText(char *prompt, char *buf, int lim);
extern char getYesNo(char *prompt);
extern void givePrompt(void);
extern void indexRooms(void);
extern void makeRoom(void);
extern char *matchString(char *buf, char *pattern, char *bufEnd);
extern void normalizeString(char *s);
extern void noteRoom(void);
extern void putRoom(int rm, struct roomBuffer *buf);
extern int  renameRoom(void);
extern void replaceString(char *buf, int lim);

#endif



#ifndef _PROTOS_H
#define _PROTOS_H

/* ctdtl.c */

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

/* log.c */

extern void getLog(struct logBuffer *lBuf, int n);
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
extern void initSysop(void);

/* misc.c */

extern void doCR(void);
extern void configure(void);
extern int  hash(char *str);
extern char visible(char c);
extern int  displayMenu(char *menu);
extern int  displayHelp(char *topic);
extern void loadConfig(char *filename);
extern void printDate(int year, int month, int day);
extern void getDate(int *year, int *month, int *day);
extern void crypte(void *buf, unsigned len, unsigned seed);

/* modem.c */

extern char BBSCharReady(void);
extern int  getCh(void);
extern void putCh(void);
extern int  iChar(void);
extern void oChar(char);
extern char KBReady(void);
extern void modemInit(void);
extern void putChar(char c);
extern void ringSysop(void);
extern void upLoad(void);
extern void endTerminal(void);
extern void initTerminal(void);
extern void putError(char *format, ...);
extern void putString(char *format, ...);
extern void getString(char *prompt, char *buf, int lim);

/* msg.c */

extern void mPeek(void);
extern char mAbort(void);
extern void msgInit(void);
extern void pullIt(int m);
extern void getMessage(void);
extern void zapMsgFile(void);
extern int  loadMsgBuf(void);
extern int  flushMsgBuf(void);
extern void putWord(char *st);
extern int  putMsgChar(char c);
extern void mFormat(char *string);
extern void fakeFullCase(char *text);
extern unsigned char getMsgChar(void);
extern void dPrintf(char *format, ...);
extern char putMessage(char uploading);
extern int  makeMessage(char uploading);
extern void mWCprintf(char *format, ...);
extern void unGetMsgChar(unsigned char c);
extern char dGetWord(char *dest, int lim);
extern void getMsgStr(char *dest, int lim);
extern void aideMessage(char noteDeletedMessage);
extern void showMessages(char whichMess, char revOrder);
extern int  findPerson(char *name, struct logBuffer *lBuf);
extern int  startAt(long sect, long byt);
extern void note2Message(long id, long loc);
extern void printMessage(long loc, long id);
extern int  getWord(char *dest, char *source, int offset, int lim);
extern void noteMessage(struct logBuffer *lBuf, long logNo);
extern void noteLogMessage(struct logBuffer *lBuf, long logNo);

/* rooma.c */

extern void init(void);
extern void systat(void);
extern void dumpRoom(void);
extern void initCitadel(void);
extern void loadRoomTab(void);
extern void fillMailRoom(void);
extern void setUp(char justIn);
extern char gotoRoom(char *nam);
extern int  roomExists(char *room);
extern void listRooms(char doDull);
extern int  fileDir(char *fileName);
extern void unspace(char *from, char *to);
extern void setSpace(char disk, char user);
extern void openFile(char *filename, int *fd);
extern void wildCard(int (*fn)(char *), char *filename);

/* roomb.c */

extern int  findRoom(void);
extern void makeRoom(void);
extern void noteRoom(void);
extern void givePrompt(void);
extern void indexRooms(void);
extern int  renameRoom(void);
extern void zapRoomFile(void);
extern char getYesNo(char *prompt);
extern void normalizeString(char *s);
extern int  editText(char *buf, int lim);
extern void replaceString(char *buf, int lim);
extern void getRoom(int rm, struct roomBuffer *buf);
extern void putRoom(int rm, struct roomBuffer *buf);
extern char getText(char *prompt, char *buf, int lim);
extern char *matchString(char *buf, char *pattern, char *bufEnd);
extern int  getNumber(char *prompt, unsigned bottom, unsigned top);

#endif


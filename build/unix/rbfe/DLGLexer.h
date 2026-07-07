#ifndef DLGLexer_h
#define DLGLexer_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: parser.dlg
 *
 * 1989-2001 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR33
 */


#include "DLexerBase.h"

class DLGLexer : public DLGLexerBase {
public:
public:
	static const int MAX_MODE;
	static const int DfaStates;
	static const int START;
	static const int STRING;
	typedef unsigned short DfaState;

	DLGLexer(DLGInputStream *in,
		unsigned bufsize=2000)
		: DLGLexerBase(in, bufsize, 1)
	{
	;
	}
	void	  mode(int);
	ANTLRTokenType nextTokenType(void);
	void     advance(void);
protected:
	ANTLRTokenType act1();
	ANTLRTokenType act2();
	ANTLRTokenType act3();
	ANTLRTokenType act4();
	ANTLRTokenType act5();
	ANTLRTokenType act6();
	ANTLRTokenType act7();
	ANTLRTokenType act8();
	ANTLRTokenType act9();
	ANTLRTokenType act10();
	ANTLRTokenType act11();
	ANTLRTokenType act12();
	ANTLRTokenType act13();
	ANTLRTokenType act14();
	ANTLRTokenType act15();
	ANTLRTokenType act16();
	ANTLRTokenType act17();
	ANTLRTokenType act18();
	ANTLRTokenType act19();
	ANTLRTokenType act20();
	ANTLRTokenType act21();
	ANTLRTokenType act22();
	ANTLRTokenType act23();
	ANTLRTokenType act24();
	ANTLRTokenType act25();
	ANTLRTokenType act26();
	ANTLRTokenType act27();
	ANTLRTokenType act28();
	ANTLRTokenType act29();
	ANTLRTokenType act30();
	ANTLRTokenType act31();
	ANTLRTokenType act32();
	ANTLRTokenType act33();
	ANTLRTokenType act34();
	ANTLRTokenType act35();
	ANTLRTokenType act36();
	ANTLRTokenType act37();
	ANTLRTokenType act38();
	ANTLRTokenType act39();
	ANTLRTokenType act40();
	ANTLRTokenType act41();
	ANTLRTokenType act42();
	ANTLRTokenType act43();
	ANTLRTokenType act44();
	ANTLRTokenType act45();
	ANTLRTokenType act46();
	ANTLRTokenType act47();
	ANTLRTokenType act48();
	ANTLRTokenType act49();
	ANTLRTokenType act50();
	ANTLRTokenType act51();
	ANTLRTokenType act52();
	ANTLRTokenType act53();
	ANTLRTokenType act54();
	ANTLRTokenType act55();
	ANTLRTokenType act56();
	ANTLRTokenType act57();
	ANTLRTokenType act58();
	ANTLRTokenType act59();
	ANTLRTokenType act60();
	ANTLRTokenType act61();
	ANTLRTokenType act62();
	ANTLRTokenType act63();
	ANTLRTokenType act64();
	ANTLRTokenType act65();
	ANTLRTokenType act66();
	ANTLRTokenType act67();
	ANTLRTokenType act68();
	ANTLRTokenType act69();
	ANTLRTokenType act70();
	ANTLRTokenType act71();
	ANTLRTokenType act72();
	static DfaState st0[53];
	static DfaState st1[53];
	static DfaState st2[53];
	static DfaState st3[53];
	static DfaState st4[53];
	static DfaState st5[53];
	static DfaState st6[53];
	static DfaState st7[53];
	static DfaState st8[53];
	static DfaState st9[53];
	static DfaState st10[53];
	static DfaState st11[53];
	static DfaState st12[53];
	static DfaState st13[53];
	static DfaState st14[53];
	static DfaState st15[53];
	static DfaState st16[53];
	static DfaState st17[53];
	static DfaState st18[53];
	static DfaState st19[53];
	static DfaState st20[53];
	static DfaState st21[53];
	static DfaState st22[53];
	static DfaState st23[53];
	static DfaState st24[53];
	static DfaState st25[53];
	static DfaState st26[53];
	static DfaState st27[53];
	static DfaState st28[53];
	static DfaState st29[53];
	static DfaState st30[53];
	static DfaState st31[53];
	static DfaState st32[53];
	static DfaState st33[53];
	static DfaState st34[53];
	static DfaState st35[53];
	static DfaState st36[53];
	static DfaState st37[53];
	static DfaState st38[53];
	static DfaState st39[53];
	static DfaState st40[53];
	static DfaState st41[53];
	static DfaState st42[53];
	static DfaState st43[53];
	static DfaState st44[53];
	static DfaState st45[53];
	static DfaState st46[53];
	static DfaState st47[53];
	static DfaState st48[53];
	static DfaState st49[53];
	static DfaState st50[53];
	static DfaState st51[53];
	static DfaState st52[53];
	static DfaState st53[53];
	static DfaState st54[53];
	static DfaState st55[53];
	static DfaState st56[53];
	static DfaState st57[53];
	static DfaState st58[53];
	static DfaState st59[53];
	static DfaState st60[53];
	static DfaState st61[53];
	static DfaState st62[53];
	static DfaState st63[53];
	static DfaState st64[53];
	static DfaState st65[53];
	static DfaState st66[53];
	static DfaState st67[53];
	static DfaState st68[53];
	static DfaState st69[53];
	static DfaState st70[53];
	static DfaState st71[53];
	static DfaState st72[53];
	static DfaState st73[53];
	static DfaState st74[53];
	static DfaState st75[53];
	static DfaState st76[53];
	static DfaState st77[53];
	static DfaState st78[53];
	static DfaState st79[53];
	static DfaState st80[53];
	static DfaState st81[53];
	static DfaState st82[53];
	static DfaState st83[53];
	static DfaState st84[53];
	static DfaState st85[53];
	static DfaState st86[53];
	static DfaState st87[53];
	static DfaState st88[53];
	static DfaState st89[53];
	static DfaState st90[53];
	static DfaState st91[53];
	static DfaState st92[53];
	static DfaState st93[53];
	static DfaState st94[53];
	static DfaState st95[53];
	static DfaState st96[53];
	static DfaState st97[53];
	static DfaState st98[53];
	static DfaState st99[53];
	static DfaState st100[53];
	static DfaState st101[53];
	static DfaState st102[53];
	static DfaState st103[53];
	static DfaState st104[53];
	static DfaState st105[53];
	static DfaState st106[53];
	static DfaState st107[53];
	static DfaState st108[53];
	static DfaState st109[53];
	static DfaState st110[53];
	static DfaState st111[53];
	static DfaState st112[53];
	static DfaState st113[53];
	static DfaState st114[53];
	static DfaState st115[53];
	static DfaState st116[53];
	static DfaState st117[53];
	static DfaState st118[53];
	static DfaState st119[53];
	static DfaState st120[53];
	static DfaState st121[53];
	static DfaState st122[53];
	static DfaState st123[53];
	static DfaState st124[53];
	static DfaState st125[53];
	static DfaState st126[53];
	static DfaState st127[53];
	static DfaState st128[53];
	static DfaState st129[53];
	static DfaState st130[53];
	static DfaState st131[53];
	static DfaState st132[53];
	static DfaState st133[53];
	static DfaState st134[53];
	static DfaState st135[53];
	static DfaState st136[53];
	static DfaState st137[53];
	static DfaState st138[53];
	static DfaState st139[53];
	static DfaState st140[53];
	static DfaState st141[53];
	static DfaState st142[53];
	static DfaState st143[53];
	static DfaState st144[53];
	static DfaState st145[53];
	static DfaState st146[53];
	static DfaState st147[53];
	static DfaState st148[53];
	static DfaState st149[53];
	static DfaState st150[53];
	static DfaState st151[53];
	static DfaState st152[53];
	static DfaState st153[53];
	static DfaState st154[53];
	static DfaState st155[53];
	static DfaState st156[53];
	static DfaState st157[53];
	static DfaState st158[53];
	static DfaState st159[53];
	static DfaState st160[53];
	static DfaState st161[53];
	static DfaState st162[53];
	static DfaState st163[53];
	static DfaState st164[53];
	static DfaState st165[53];
	static DfaState st166[53];
	static DfaState st167[53];
	static DfaState st168[53];
	static DfaState st169[53];
	static DfaState st170[5];
	static DfaState st171[5];
	static DfaState st172[5];
	static DfaState st173[5];
	static DfaState st174[5];
	static DfaState st175[5];
	static DfaState *dfa[176];
	static DfaState dfa_base[];
	static unsigned char *b_class_no[];
	static DfaState accepts[177];
	static DLGChar alternatives[177];
	static ANTLRTokenType (DLGLexer::*actions[73])();
	static unsigned char shift0[257];
	static unsigned char shift1[257];
	int ZZSHIFT(int c) { return b_class_no[automaton][1+c]; }
//
// 133MR1 Deprecated feature to allow inclusion of user-defined code in DLG class header
//
#ifdef DLGLexerIncludeFile
#include DLGLexerIncludeFile
#endif
};
typedef ANTLRTokenType (DLGLexer::*PtrDLGLexerMemberFunc)();
#endif

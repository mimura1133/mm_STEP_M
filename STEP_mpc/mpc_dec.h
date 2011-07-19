#ifndef _mpp_dec_h_
#define _mpp_dec_h_

#include <stdio.h>
#include <io.h>

#ifndef _in_mpc_h_
class StreamInfo;
#endif

/* D E F I N E S */
#define EQ_TAP          13                      // length of FIR filter for EQ
#define DELAY           ((EQ_TAP + 1) / 2)      // delay of FIR
#define FIR_BANDS       4                       // number of subbands to be FIR filtered

class Reader
{
protected:
	bool aborting,prebuffering;
public:
	Reader() {aborting=0;prebuffering=0;}
	virtual size_t read(void * ptr,size_t size)=0;
	virtual size_t write(void * ptr,size_t size)=0;
	virtual int seek(int offset,int origin)=0;
	virtual long tell()=0;
	virtual void seteof(size_t ofs) {};
	virtual ~Reader() {}
	virtual int canwrite() {return 0;};//is writing supported or not
	virtual void setabort(bool a) {aborting=a;};
	virtual void setprebuf(bool p) {prebuffering=p;}
	virtual void do_prebuf() {}
	virtual void idle();
};

class MPC_decoder
{
private:
	Reader * m_reader;
public:
	MPC_decoder(Reader * r);
	~MPC_decoder();

	/* F U N C T I O N S */
	int   DECODE            ( char* );
	void  RESET_Synthesis   ( void );
	void  RESET_Globals     ( void );
	void  InitDSCF          ( void );
	void  Lese_Bitstrom_SV6 ( void );
	void  Lese_Bitstrom_SV7 ( void );

	void SetStreamInfo(StreamInfo * si);

	/* V A R I A B L E S */
	// global variables for EQ
	float         EQ_gain   [ 32 - FIR_BANDS ];
	float         EQ_Filter [ FIR_BANDS ][ EQ_TAP ];
	unsigned int  EQ_activated;

	// globale flag for Dithering
	int           DitherUsed;

	// ...more globals
	unsigned int  StreamVersion;         // version of bitstream
	unsigned int  MS_used;               // MS-coding used ?
	unsigned int  FwdJumpInfo;
	unsigned int  ActDecodePos;
	unsigned int  FrameWasValid;
	unsigned int  OverallFrames;
	unsigned int  DecodedFrames;
	unsigned int  LastValidSamples;
	unsigned int  TrueGaplessPresent;

	//mpp_dec.cpp

	typedef struct {
		int  L [36];
		int  R [36];
	} QuantTyp;

	int                     SCF_Index_L [32] [3];
	int                     SCF_Index_R [32] [3];       // holds scalefactor-indices
	QuantTyp                Q [32];                     // holds quantized samples
	int                     Res_L [32];
	int                     Res_R [32];                 // holds the chosen quantizer for each subband
	int                     DSCF_Flag_L [32];
	int                     DSCF_Flag_R [32];           // differential SCF used?
	int                     SCFI_L [32];
	int                     SCFI_R [32];                // describes order of transmitted SCF
	int                     DSCF_Reference_L [32];
	int                     DSCF_Reference_R [32];      // holds last frames SCF
	int                     MS_Flag[32];                // MS used?


	void RESET_Y ( void );
	void perform_EQ ( void );
	void Requantisierung ( const int Last_Band );

	//perform_EQ crap
    float  SAVE_L     [DELAY] [32];                      // buffer for ...
    float  SAVE_R     [DELAY] [32];                      // ... upper subbands
    float  FirSave_L  [FIR_BANDS] [EQ_TAP];              // buffer for ...
    float  FirSave_R  [FIR_BANDS] [EQ_TAP];              // ... lowest subbands

	//bitstream.h
#define MEMSIZE    16384          // overall buffer size
#define MEMSIZE2   (MEMSIZE/2)    // size of one buffer
#define MEMMASK    (MEMSIZE-1)

	/* T Y P E S */
	typedef struct {
		unsigned int  Code;
		unsigned int  Length;
		int           Value;
	} HuffmanTyp;

	/* V A R I A B L E N */
	unsigned int  Speicher [MEMSIZE];     // read-buffer
	unsigned int  dword;                  // actually decoded 32bit-word
	unsigned int  pos;                    // bit-position within dword
	unsigned int  Zaehler;                // actual index within read-buffer
	unsigned int  WordsRead;            // counts amount of decoded dwords
	/* P R O Z E D U R E N */
	void          Reset_BitstreamDecode ( void );
	unsigned int  BitsRead              ( void );
	unsigned int  Bitstream_read        ( const unsigned int );
	int           Huffman_Decode        ( const HuffmanTyp* );   // works with maximum lengths up to 14
	int           Huffman_Decode_fast   ( const HuffmanTyp* );   // works with maximum lengths up to 10
	int           Huffman_Decode_faster ( const HuffmanTyp* );   // works with maximum lengths up to  5
	void          SCFI_Bundle_read      ( const HuffmanTyp*, int*, int* );
	void          Resort_HuffTables     ( const unsigned int elements, HuffmanTyp* Table, const int offset );

	//huff_new.h
/* V A R I A B L E S */
	HuffmanTyp         HuffHdr  [10];
	HuffmanTyp         HuffSCFI [ 4];
	HuffmanTyp         HuffDSCF [16];
	HuffmanTyp*  HuffQ [2] [8];

	//huff_new_cpp
	HuffmanTyp  HuffQ1 [2] [3*3*3];
	HuffmanTyp  HuffQ2 [2] [5*5];
	HuffmanTyp  HuffQ3 [2] [ 7];
	HuffmanTyp  HuffQ4 [2] [ 9];
	HuffmanTyp  HuffQ5 [2] [15];
	HuffmanTyp  HuffQ6 [2] [31];
	HuffmanTyp  HuffQ7 [2] [63];

	//huff_old.h
	/* F U N C T I O N S */
	void  Huffman_SV6_Encoder ( void );
	void  Huffman_SV6_Decoder ( void );

	/* V A R I A B L S */
	const HuffmanTyp*  SampleHuff [18];
	HuffmanTyp         SCFI_Bundle   [ 8];
	HuffmanTyp         DSCF_Entropie [13];
	HuffmanTyp         Region_A [16];
	HuffmanTyp         Region_B [ 8];
	HuffmanTyp         Region_C [ 4];

	//huff_old.cpp
	HuffmanTyp  Entropie_1 [ 3];
	HuffmanTyp  Entropie_2 [ 5];
	HuffmanTyp  Entropie_3 [ 7];
	HuffmanTyp  Entropie_4 [ 9];
	HuffmanTyp  Entropie_5 [15];
	HuffmanTyp  Entropie_6 [31];
	HuffmanTyp  Entropie_7 [63];

	//in_mpc.cpp
	unsigned short* SeekTable;
	double          sfreq_khz;                              // Sample frequency in kHz
	unsigned int    MPCHeaderPos;                       // AB: needed to support ID3v2
	void EQSet ( int on, char data[10], int preamp );

	void Helper1 ( unsigned long bitpos );
	void Helper2 ( unsigned long bitpos );
	void Helper3 ( unsigned long bitpos, long* buffoffs );
	int perform_jump ( int* done, int* Frames,int seek_needed);
	int FileInit();
	void UpdateBuffer(unsigned int RING);

	//requant.h
	float              SCF  [256];       // holds adapted scalefactors (for clipping prevention)
	unsigned int       Q_bit [32];       // holds amount of bits to save chosen quantizer (SV6)
	unsigned int       Q_res [32] [16];  // holds conversion: index -> quantizer (SV6)
	int                Max_Band;
	void ScaleOutput                          ( double factor );
	void Quantisierungsmodes ( void );            // conversion: index -> quantizer (bitstream reading)

	_inline void SetAbort(bool a) {m_reader->setabort(a);}
	_inline void SetPrebuf(bool p) {m_reader->setprebuf(p);}
	_inline void DoPrebuf() {m_reader->do_prebuf();}
	_inline void Idle() {m_reader->idle();}

private:
	void initialisiere_Quantisierungstabellen ( void );
	void  Huffman_SV7_Encoder (void);
	void  Huffman_SV7_Decoder (void);

private:
	_inline int f_read(void * ptr,size_t size) {return m_reader->read(ptr,size);};
	_inline int f_seek(int offset,int origin) {return m_reader->seek(offset,origin);};
};

class Reader_file : public Reader
{
private:
	FILE *f;
public:
	Reader_file(FILE * _f) {f=_f;}
	~Reader_file() {if (f) fclose(f);}
	virtual size_t read(void * ptr,size_t size) {return fread(ptr,1,size,f);}
	virtual size_t write(void * ptr,size_t size) {return fwrite(ptr,1,size,f);}
	virtual int seek(int offset,int origin) {return fseek(f,offset,origin);}
	virtual long tell() {return ftell(f);}
	virtual void seteof(size_t ofs) {_chsize ( _fileno (f), ofs);};
	virtual int canwrite() {return 1;}
};

#endif

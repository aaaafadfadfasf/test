
//it,s created  by fandongsheng on date 2015.3.8
//for analysis rtp Packet and frame information .
#ifndef _STREAMPARSER
#define _STREAMPARSER

#include "kdvtype.h"
#include "kdvdef.h"

#ifdef _USE_OSPSMALL_
#include "osp_small.h"
#else
#include "osp.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USE_OSPSMALL_
#define  KDVSTREAMPARSERPRINTF           OsApi_Printf
#else
#define  KDVSTREAMPARSERPRINTF           OspPrintf
#endif

//audio mode used to analysis to audio context 
#define	    AUDIOMODE_MP3_WORST   						(s32)0
#define	    AUDIOMODE_MP3_BAD     					    (s32)1
#define	    AUDIOMODE_MP3_NORMAL  						(s32)2
#define	    AUDIOMODE_MP3_FINE    						(s32)3
#define	    AUDIOMODE_MP3_BEST    					    (s32)4
#define  	AUDIOMODE_PCMA    							(s32)5//a law
#define  	AUDIOMODE_PCMU    							(s32)6//u law 
#define  	AUDIOMODE_G723_6  							(s32)7 
#define  	AUDIOMODE_G723_5  							(s32)8
#define  	AUDIOMODE_G728								(s32)9 
#define  	AUDIOMODE_G722								(s32)10 
#define  	AUDIOMODE_G729								(s32)11 

//position of frame message in rtp extend head 
#define 	EXTENDRTPHEAD_AUDIOMODE_POS 				(s32)2
#define 	EXTENDRTPHEAD_VIDEOKEYFRAME_POS 			(s32)2
#define 	EXTENDRTPHEAD_VIDEOFRAMERATE_POS 		    (s32)3
#define 	EXTENDRTPHEAD_VIDEOFRAMEID_POS 			    (s32)4
#define 	EXTENDRTPHEAD_VIDEOFRAMEWIDTH_POS 	        (s32)8
#define 	EXTENDRTPHEAD_VIDEOFRAMEHEIGHT_POS 	        (s32)10

//RTP  head max len
#define 	RTPHEAD_MAXLEN 								(s32)(3*4)
#define 	RTPHEAD_FIXEDLEN 							(s32)(3*4) //static len 
#define 	EXTENDRTPHEAD_MAXLEN 						(s32)(RTPHEAD_MAXLEN + 4)
#define 	EXTENDRTPHEAD_MINLEN 						(s32) 4
#define 	EXTENDRTPHEAD_SIZE                          (s32)4
#define     RTPPACK_MAXSIZE                             (s32)8192

//about h264 stream definition 
#define  MAXNUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE 		(s32)255 //256
#define	 MAXNUM_SLICE_GROUPS_MINNUS1                    (s32)8
#define  MAX_NALU_SIZE                                  (s32)(3000) //max two rtp buff size 

//about ps head definition 
#define PS_HEAD_STARTCODE								(s32)0x000001BA
#define PS_SYSTEMHEAD_STARTCODE                         (s32)0x000001BB
#define PS_MAPHEAD_STARTCODE                            (s32)0x000001BC
#define PS_STREAM_ENDCODE                               (s32)0x000001B9

#define PS_HEAD_BYTE    							    (u8)(PS_HEAD_STARTCODE & 0xff)
#define PS_SYSTEMHEAD_BYTE                              (u8)(PS_SYSTEMHEAD_STARTCODE & 0xff)
#define PS_MAPHEAD_BYTE                                 (u8)(PS_MAPHEAD_STARTCODE & 0xff)
#define PS_END_BYTE                        			    (u8)(PS_STREAM_ENDCODE & 0xff)

//PES stream flag
#define PES_AUDIOSTREAM    								(u8)0xC0
#define PES_VIDEOSTREAM                					(u8)0xE0

//bit check
#define CHECK_BITSCOUNT(u8bitscount, count)             if(u8bitscount  > count) {return 0;}
#define MAX_STREAMNUM_IN_A_PSSTREAM                     (u8)255
//ps payload definition 
#define PS_PAYLOAD_MPEG1 							    (u8)(0x01)
#define PS_PAYLOAD_MPEG2                        		(u8)(0x02)
#define PS_PAYLOAD_MPEG4                        		(u8)(0x10)
#define PS_PAYLOAD_H264									(u8)(0x1B)
#define PS_PAYLOAD_H265                                 (u8)(0X24)
#define PS_PAYLOAD_H265_OLD                             (u8)(0xa4)
#define PS_PAYLOAD_SVACV								(u8)(0x80)

#define PS_PAYLOAD_MP1                         			(u8)(0x03)
#define PS_PAYLOAD_MP2                         			(u8)(0x04)
#define PS_PAYLOAD_AACLC                       			(u8)(0x11)
#define PS_PAYLOAD_OPUS                       			(u8)(0xB1)
#define PS_PAYLOAD_MP2AAC						  		(u8)(0x0f)
#define PS_PAYLOAD_G711A                        		(u8)(0x90) 
#define PS_PAYLOAD_G7221                        		(u8)(0x92)
#define PS_PAYLOAD_G7231                        		(u8)(0x93)
#define PS_PAYLOAD_G729                         		(u8)(0x99)
#define PS_PAYLOAD_SVACA                       			(u8)(0x9B)

#define MAX_PSFRAME_SIZE                                (s32)(65535*4)
//pes stream id
#define PES_PROGRAM_STREAM_MAP         					0xBC//节目映射流
#define PES_PADDING_STREAM              				0xBE//填充流
#define PES_PRIVATE_STREAM_1           					0xBD//私有流1
#define PES_PRIVATE_STREAM_2            				0xBF//私有流2
#define PES_ECM_STREAM                  				0xF0//ECM流
#define PES_EMM_STREAM             						0xF1//EMM流
#define PES_DSM_CC_STREAM              					0xF2//DSM流
#define PES_ISOIEC_13522_STREAM      					0xF3//13522流
#define PES_RESERVED_DATA_STREAM        				0xF0//保留数据流
#define PES_PROGRAM_STREAM_DIRECTORY    				0xFF//节目流路径

//pes head len 

#define PES_DATAHEAD_LENGTH                             (u8)(6) //PES包头长度(固定6字节)
#define PES_ESINFO_LENGTH          						(u8)(3) //ES流特有信息(固定3字节)

///////////////////////////////////////////////////////////////////////////////////////////////
//解析mpeg4所需的结构体以及宏定义
//码流信息结构
typedef struct
{
    unsigned int u32Cur;      //当前码流数据
    unsigned int u32Next;     //字节反转后码流数据
    unsigned int u32Pos;      //码流位置
    unsigned int *u32Ptr;     //码流指针
    unsigned int *u32Buf;     //码流Buffer
    unsigned int u32Length;   //码流长度
    unsigned int u32DecBsErr; //码流解码错误标记
    unsigned int u32Offset;   //偏移
} TBitReader;

#define VOL_TYPE_DEFAULT                            0
#define VOL_TYPE_SIMPLE                             1
#define VOL_TYPE_SIMPLE_SCALABLE                    2
#define VOL_TYPE_CORE                               3
#define VOL_TYPE_MAIN                               4
#define VIDEOJLAY_TYPE_CORE_STUDIO                  16
#define VIDOBJLAY_TYPE_ASP                          17

// Aspect ratio
#define VOL_AR_EXTPAR               15
//Shape
#define VOL_SHAPE_RECTANGULAR       0
#define VOL_SHAPE_BINARY            1
#define VOL_SHAPE_BINARY_ONLY       2
#define VOL_SHAPE_GRAYSCALE         3

#define START_CODE          0x00000100		// start code , 27 bits
#define VO_START_CODE       0x00000100	
#define VOL_START_CODE	    0x00000120	
#define VO_SEQ_START_CODE   0x000001b0
#define USERDATA_START_CODE 0x000001b2
#define GOV_START_CODE      0x000001b3
#define VSO_START_CODE      0x000001b5		// Visual Object start code
#define VOP_START_CODE      0x000001b6
#define SLICE_START_CODE    0x000001b7

#define MARKER()    BsSkip(ptBs, 1)
///////////////////////////////////////////////////////////////////////////////////////////////

//  typedef struct tagKdvCodecContext 
//  {
//  	s32 sample_rate;///< samples per second
//  	s32 channels;///< number of audio channels
//  	u16  bybits;  //bits
//  }TKDVCodecContext, TAudioParam;

typedef TKdvCodecContext TKDVCodecContext;
// NAL类型
enum ENetPacketH264NalType
{
    H264_NAL_SLICE = 1,
		H264_NAL_PARTA,
		H264_NAL_PARTB,
		H264_NAL_PARTC,
		H264_NAL_IDRSLICE,
		H264_NAL_SEI,
		H264_NAL_SPS,
		H264_NAL_PPS,
		H264_NAL_DELIMITER,
		H264_NAL_FILL = 12,
};

// NAL类型
enum ENetPacketSVACNalType
{
    SVAC_NAL_SLICE = 1,
		SVAC_NAL_IDRSLICE,
};

enum ENetPacketH265NalType
{
    H265_NAL_SLICE = 1,
		H265_NAL_IDRSLICE = 19,
		H265_NAL_VPS = 32,
		H265_NAL_SPS,
		H265_NAL_PPS,
};

typedef struct tagParserPs
{
	//psm information 
	u8 u8StreamNum;
	u8 au8StreamType[MAX_STREAMNUM_IN_A_PSSTREAM];

	//pes information 
	u8 u8StreamId; // must equal with u8AudioID or u8VideoID

	//frame buf and len 
	u8  *pu8FrameBuf;
	u32 u32FrameLen;
	//pes payload buffer and length
	u8 *pu8PayloadBuffer;
	u32 u32PayloadLength;

	u64 u64Dts;//pes timestamp 
	TKdvCodecContext m_tAudioParam[MAX_STREAMNUM_IN_A_PSSTREAM];
	u8 byVideoStreamNum;
	u8 byAudioStreamNum;
}TParserPs;

static const u16 audio_sample_PerSec_old[16] = {64000, 48000, 44100, 32000, 16000, 8000};


//rtp information 
typedef struct tagRtpInfo
{
	u8   m_byMark;
	u8   m_byExtence;
	u8   m_byPadNum;
	u8   m_byPayload;
	u32  m_dwSSRC;
	u16  m_wSequence;
	u32  m_dwTimeStamp;
	u8  *m_pExData;
	s32  m_nExSize;
	u8  *m_pRealData;
	s32  m_nRealSize;
	s32  m_nPreBufSize;
}TRtpInfo;

typedef  struct tagMSVideoParam
{
	BOOL32    m_bKeyFrame;
	u16       m_wVideoWidth;
	u16       m_wVideoHeight;
	BOOL32    m_bIsFrameFlag;// used by svacv (modify)
}TMSVideoParam;

typedef struct tagFrameInfo
{
	//video frame
	u8     m_byVideoType;
	u8     m_byFrameRate;

	TMSVideoParam m_tVideoParam;
	u32   m_dwH264ProfileValue;
	//audio frame
	u8 m_byAudioType;
	TKdvCodecContext tAudioParam;

	//ps information
	TParserPs tParserps; // record message what is needed by decode ps frame

	BOOL32 bLastPackHaveSPS; // flag of if have sps in last pack
	u8 byConnectBuf[MAX_NALU_SIZE]; // used to connect two packet
	u8 *pbyLastPackPtr;  //last pack information
	s32 nLastPackLength;
	u16 wLastRtpPackSeq; 
	u32 dwMaxPartionWidth;//LCU宽度
	u32 dwSPSId;// SPS的
}TFrameInfo;

typedef struct tagRtpAndFrameInfo
{
	TRtpInfo tRtpInfo;
	TFrameInfo tFrameInfo;
}TRtpAndFrameInfo;
	
///===============interface for outside 
//analysis rtp head and rtpextend head where get frame information. 
static BOOL32 ParseRtpHead(u8 *pBuf, s32 nSize, TRtpAndFrameInfo *ptRtpAndFrameInfo);
//get frame information from buf  that without rtp head .
static BOOL32 ParseH264(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo);
static BOOL32 ParseH265(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo);
static BOOL32 ParseSVACV(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo);
static BOOL32 ParseMP4(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo);
static BOOL32 ParsePS(u8 *pBuf, s32 nSize, u16 wRtpSeq, TFrameInfo *ptFrameInfo);
//get frame information from frame buffer ,only support H264,H265,SVACV .
static BOOL32 ParseFrame(u8 byPayload, u8* pbyFrameBuf, u32 dwFrameLen, TFrameInfo* ptFrameInfo);
//audio convert between  context and mode 
static BOOL32 KdvContextToAudioMode_old(TKdvCodecContext *ptAVcontext, u8 *pbyAudioMode, u8 byMediaType);
static BOOL32 KdvAudioModeToContext_old(TKdvCodecContext *ptAVcontext, u8 byAudioMode, u8 byMediaType);
///===============================================================///

//(1) all base function 
static void ConvertN2H(u8 *pBuf, s32 nStartIndex, s32 nSize)
{
	s32 i = 0;
	for(i = nStartIndex; i < (nStartIndex+nSize); i++) 
	{
		((u32 *)pBuf)[i] = ntohl(((u32 *)pBuf)[i]);
	}
	return;
}

static u32 GetBitField(u32 dwValue, s32 nStartBit, s32 nBits)
{
	s32  nMask = (1 << nBits) - 1;

	return (dwValue >> nStartBit) & nMask; 
}

//(2)analysis h264 stream struct and funtion 
//struct
typedef struct
{
	u8 *pu8Start;
	u8 *pu8P;
	u8 *pu8End;
	s32 s32Left;        // i_count number of available bits 
	u8 au8HeadInfo[30];
} TParserBitStream; 

typedef struct  tagParserPicParameterSetRBSP
{
	BOOL32		bIsValid;										  // indicates the parameter set is valid
	u32			pic_parameter_set_id;                             // ue(v)
	u32			seq_parameter_set_id;                             // ue(v)
	BOOL32		entropy_coding_mode_flag;                         // u(1)
	// if( pic_order_cnt_type < 2 )  in the sequence parameter set
	BOOL32      pic_order_present_flag;                           // u(1)
	u32			num_slice_groups_minus1;                          // ue(v)
	u32			slice_group_map_type;							  // ue(v)
	// if( slice_group_map_type = = 0 )
	u32			run_length_minus1[MAXNUM_SLICE_GROUPS_MINNUS1];	// ue(v)
	// else if( slice_group_map_type = = 2 )
	u32			top_left[MAXNUM_SLICE_GROUPS_MINNUS1];			  // ue(v)
	u32			bottom_right[MAXNUM_SLICE_GROUPS_MINNUS1];	      // ue(v)
	// else if( slice_group_map_type = = 3 || 4 || 5
	BOOL32		slice_group_change_direction_flag;				  // u(1)
	u32			slice_group_change_rate_minus1;					  // ue(v)
	// else if( slice_group_map_type = = 6 )
	u32			num_slice_group_map_units_minus1;				  // ue(v)
	u32			*slice_group_id;								  // complete MBAmap u(v)
	u32			num_ref_idx_l0_active_minus1;                     // ue(v)
	u32			num_ref_idx_l1_active_minus1;                     // ue(v)
	BOOL32		weighted_pred_flag;                               // u(1)
	BOOL32		weighted_bipred_idc;                              // u(2)
	s32			pic_init_qp_minus26;                              // se(v)
	s32			pic_init_qs_minus26;                              // se(v)
	s32			chroma_qp_index_offset;                           // se(v)
	BOOL32		deblocking_filter_control_present_flag;           // u(1)
	BOOL32		constrained_intra_pred_flag;                      // u(1)
	BOOL32		redundant_pic_cnt_present_flag;                   // u(1)
	BOOL32		vui_pic_parameters_flag;                          // u(1)
	//#ifndef G50_SPS
	//	BOOL32   frame_cropping_flag;                           // u(1)
	//    u32  frame_cropping_rect_left_offset;					// ue(v)
	//    u32  frame_cropping_rect_right_offset;				// ue(v)
	//    u32  frame_cropping_rect_top_offset;					// ue(v)
	//    u32  frame_cropping_rect_bottom_offset;				// ue(v)
	//#endif
} TParserPicParameterSetRBSP;

typedef struct tagParserSeqParameterSetRBSP
{
	BOOL32   bIsValid;												// indicates the parameter set is valid

	u32	profile_idc;										        // u(8)
	//#ifdef G50_SPS
	BOOL32		constrained_set0_flag;								// u(1)
	BOOL32		constrained_set1_flag;								// u(1)
	BOOL32		constrained_set2_flag;								// u(1)
	//#endif
	//uint_8  reserved_zero_5bits; /*equal to 0*/					// u(5)
	u32			level_idc;											// u(8)
	//#ifndef G50_SPS
	//	BOOL32		more_than_one_slice_group_allowed_flag;			// u(1)
	//	BOOL32		arbitrary_slice_order_allowed_flag;				// u(1)
	//	BOOL32		redundant_slices_allowed_flag;					// u(1)
	//#endif
	u32			seq_parameter_set_id;								// ue(v)
	u32			log2_max_frame_num_minus4;							// ue(v)
	u32			pic_order_cnt_type;									// ue(v)
	// if( pic_order_cnt_type == 0 ) 
	u32			log2_max_pic_order_cnt_lsb_minus4;					// ue(v)
	// else if( pic_order_cnt_type == 1 )
	BOOL32		delta_pic_order_always_zero_flag;					// u(1)
	s32			offset_for_non_ref_pic;								// se(v)
	s32			offset_for_top_to_bottom_field;						// se(v)
	u32			num_ref_frames_in_pic_order_cnt_cycle;				// ue(v)
	// for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
	s32			offset_for_ref_frame[MAXNUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE];   // se(v)
	u32			num_ref_frames;										// ue(v)
	BOOL32		gaps_in_frame_num_value_allowed_flag;				// u(1)
	u32			pic_width_in_mbs_minus1;							// ue(v)
	u32			pic_height_in_map_units_minus1;						// ue(v)
	BOOL32		frame_mbs_only_flag;								// u(1)
	// if( !frame_mbs_only_flag ) 
	BOOL32		mb_adaptive_frame_field_flag;						// u(1)
	BOOL32		direct_8x8_inference_flag;							// u(1)
	//#ifdef G50_SPS
	BOOL32		frame_cropping_flag;								// u(1)
	u32			frame_cropping_rect_left_offset;					// ue(v)
	u32			frame_cropping_rect_right_offset;					// ue(v)
	u32			frame_cropping_rect_top_offset;						// ue(v)
	u32			frame_cropping_rect_bottom_offset;					// ue(v)
	//#endif
	BOOL32		vui_parameters_present_flag;						// u(1)
	//not of syntax
	s32			MaxFrameNum;
} TParserSeqParameterSetRBSP;

typedef enum 
{
	H264_P_SLICE = 0,
	H264_B_SLICE,
	H264_I_SLICE,
	H264_SP_SLICE,
	H264_SI_SLICE
} Parserstdh264SliceType;

typedef struct
{
	u16		first_mb_in_slice;						//ue(v)  slice中第一个MB的地址
	u16		slice_type;								//ue(v)  slice的编码类型
	u16		pic_parameters_set_id;	
} TParserstdh264Dec_SliceHeaderData;

//H265

typedef struct
{
    u16		first_mb_in_slice;						//ue(v)  slice中第一个MB的地址
    u16		slice_type;								//ue(v)  slice的编码类型
    u16		pic_parameters_set_id;	
} Tstdh265Dec_SliceHeaderData;

//SVACV  start
typedef struct
{
    s16 s16Width;                     //图像宽度(帧级别)
    s16 s16Height;                    //图像高度(帧级别)
    s16 s16PicHeight;                 //图像高度(图像级别)
    u8 u8Valid;                       //标识此sps正常
    u8 u8FrameMbsOnlyFlag;            //FrameMBsOnlyFlag
    u8 u8MbAdaptFrameFieldFlag;       //标志序列的场帧编解码格式
    u8 u8OutPutFrameFlag;             //标识图像解码输出格式(1表示帧，0表示场)
    u8 u8FrameCroppingFlag;           //裁减参数标志位
    u8 u8NumRefFrames;                //指定参考帧队列可能达到的最大长度
    u8 u8SpsID;
    u8 u8ProFile;                     //Profile类型（判断是否需要延迟输出）

    u8 u8ProgessiveSeqFlag;         //0:可为场，可为帧； 1：帧格式
    u8 u8RoiFlag;
    u8 u8SvcFlag;

    u8 u8PicWidthInMbs;
    u8 u8PicHeightInMbs;
    u16 u16PicInMbs;	
} TSVACSPS;

#define  MAX_ROI_NUM (1)
//pps结构
typedef struct
{
    TSVACSPS *ptSps;                      //关联的sps的指针
    u8 u8PocFlag;                     //pic_order_present_flag
    u8 u8Valid;                       //标识此pps正常
    u8 u8RefFrame;                    //list0参考帧数目
    u8 u8NumRefIdxl1Active;           //list1参考帧队列的长度
    u8 u8Qp;                          //初始qp
    s8 s8ChromaQpOffset;              //chroma_qp_index_offset
    s8 s8ChromaQpOffset2;              //chroma_qp_index_offset2
    u8 u8DeblockFlag;                 //deblock flag
    u8 u8ContrainedIntra;             //constrained_intra_pred_flag
    u8 u8RedundantFlag;               //redundant_pic_cnt_present_flag
    u8 u8WeightedPredFlag;            //指明是否允许P片和SP片段加权预测
    u8 u8WeightedBiPredFlag;          //指明是否允许B片加权预测
    u8 u8EntropyCodingModeFlag;       //熵编码的标志，0:CAVLC,1:CABAC
    u8 u8SpsID;
    u8 u8PpsID;
    u8 u8NumRoi;
    u8 u8NonRoiSkipFlag;
    u8 u8ScalableNonRoiSkipFlag;
    u8 u8PicQP;
    u8 u8PicQPForRoi;
    u32 au32TopLeft[MAX_ROI_NUM];
    u32 au32BottomRight[MAX_ROI_NUM];
//     u32 *pu32TopLeft;
//     u32 *pu32BottomRight;
    u8 u8FixPicQp;
    u8 u8LoopFilterDisableFlag;
	BOOL32 bIsValid;
} TSVACPPS;

typedef struct
{
    u16        first_mb_in_slice;                        //ue(v)  slice中第一个MB的地址
    u16        slice_type;                                //ue(v)  slice的编码类型
    u16        pic_parameters_set_id;
    u16        frame_num;
    u16        idr_pic_id;
    u16        field_pic_flag;
    u16        bottom_field_flag;
}TstdSVACDec_SliceHeaderData;

//SVACV end

//sps结构
typedef struct tagTSPS
{
    u8 u8VpsId;  //sps_video_parameter_set_id
    u8 u8SpsId;  //sps_seq_parameter_set_id
    u8 u8Valid;
    u8 u8SpsTemporalIdNestingFlag; //sps_temporal_id_nesting_flag
    u8 u8ChromaFormatIdc;  //chroma_format_idc
    u16 u16Width;          //pic_width_in_luma_samples
    u16 u16Height;         //pic_height_in_luma_samples   
    s32 l32ConformanceWindow_flag;  //conformance_window_flag 
    s32 l32ConfWinLeftOffset;   //conf_win_left_offset
    s32 l32ConfWinRightOffset;   //conf_win_right_offset
    s32 l32ConfWinTopOffset;   //conf_win_top_offset
    s32 l32ConfWinBottomOffset;   //conf_win_bottom_offset
    u8 u8BitDepthLumaMinus8;   //bit_depth_luma_minus8
    u8 u8BitDepthChromaMinus8;  //bit_depth_Chroma_minus8 
    u8 au8SpsMaxDecPicBufferingMinus1[7]; //sps_max_dec_pic_buffering_minus1
    u8 au8SpsMaxNumReorderPics[7]; //sps_max_num_reorder_pics
    u8 au8SpsMaxLatencyIncreasePlus1[7]; //sps_max_latency_increase_plus1
    u8 u8Log2MaxPicOrderCntLsbMinus4;  //log2_max_pic_order_cnt_lsb_minus4 
    u8 u8Log2MinCodingBlockSizeMinus3;  //log2_min_coding_block_size_minus3
    u8 u8Log2DiffMaxMinLumaCodingBlockSize;  //log2_diff_max_min_coding_block_size
    u8 u8Log2MinTransformBlockSizeMinus2; //log2_min_transform_block_size_minus2
    u32 u32MaxPartionWidth;
} TSPS;

//pps结构
typedef struct
{
    u8 u8SpsId;   //pps_seq_parameter_set_id
    u8 u8PpsId;   //pic_parameter_set_id
    u8 u8Valid;
    u8 u8CabacInitPresentFlag; //cabac_init_present_flag
    u8 u8dDependentSliceSegmentsEnabledFlag; //dependent_slice_segments_enabled_flag
    u8 u8OutputFlagPresentFlag;  //output_flag_present_flag
    u8 u8SignDataHidingFlag;      //sign_data_hiding_flag
    u8 u8NumRefIdxL0DefaultActiveMinus1; //num_ref_idx_l0_default_active_minus1
    u8 u8NumRefIdxL1DefaultActiveMinus1; //num_ref_idx_l1_default_active_minus1
    s8 s8PicInitQpMinus26; //pic_init_qp_minus26
    s32 l32ConstrainedIntrPredFlag;  // constrained_intra_pred_flag
    s32 l32Transform_SkipEnabled_Flag;  //transform_skip_enabled_flag
    s32 l32CUQpDeltaEnabledFlag;    //cu_qp_delta_enabled_flag
    u8 u8DiffCUQpDeltaDepth; //diff_cu_qp_delta_depth
    u8 u8PpsSliceChromaQpOffsetsPresentFlag;//pps_slice_chroma_qp_offsets_present_flag
    u8 u8TransquantBypassEnabledFlag;      //transquant_bypass_enable_flag
    s32 l32TilesEnabledFlag;  //tiles_enabled_flag
    u8 u8EntropyCodingSyncEnabledFlag; //entropy_coding_sync_enabled_flag
    s32 l32NumTileColumnsMinus1; //num_tile_columns_minus1
    s32 l32NumTileRowsMinus1; //num_tile_rows_minus1
    u8 u8UniformSpacingFlag; //uniform_spacing_flag
    s32 l32LoopFilterAcrossTilesEnabledFlag; //loop_filter_across_tiles_enabled_flag
    s32 l32LoopFilterAcrossSlicesEnabledFlag; //loop_filter_across_slices_enabled_flag
    u8 u8DeblockingFilterControlPresentFlag; //deblocking_filter_control_present_flag
    u8 u8DeblockingFilterOverrideEnabledFlag;//deblocking_filter_override_enabled_flag
    s32 l32DeblockingFilterDisableFlag; //pps_disable_deblocking_filter_flag
    s8 s8PPSBetaOffsetDiv2; //pps_beta_offset_div2
    s8 s8PPSTcOffsetDiv2; //pps_tc_offset_div2
    u8 u8ListsModificationPresentFlag; //lists_modification_present_flag
    u8 u8Log2ParallelMergeLevelMinus2; //log2_parallel_merge_level_minus2
} TPPS;



//function
static void stdh264_bs_init( TParserBitStream *s, void *p_data, s32 i_data )
{
	s->pu8Start = (u8*) p_data;
	s->pu8P     = (u8*) p_data;
	s->pu8End   = (u8*) s->pu8P + i_data;
	s->s32Left  = 8;
}

static s32 stdh264_bs_pos( TParserBitStream *s )
{
	return( 8 * ( s->pu8P - s->pu8Start ) + 8 - s->s32Left );
}

static s32 stdh264_bs_eof( TParserBitStream *s )
{
	return( s->pu8P >= s->pu8End ? 1: 0 );
}

static u32 stdh264_bs_read( TParserBitStream *s, s32 i_count )
{
	static u32 dwstdh264MaskArr[33] = 
	{ 
		0x00,
		0x01,      0x03,      0x07,      0x0f,
		0x1f,      0x3f,      0x7f,      0xff,
		0x1ff,     0x3ff,     0x7ff,     0xfff,
		0x1fff,    0x3fff,    0x7fff,    0xffff,
		0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
		0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
		0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
		0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff };

		s32 i_shr;
		u32 i_result = 0;

		while( i_count > 0 )
		{
			if( s->pu8P >= s->pu8End )
			{
				break;
			}

			if( ( i_shr = s->s32Left - i_count ) >= 0 )
			{
				/* more in the buffer than requested */
				i_result |= ( *s->pu8P >> i_shr )&dwstdh264MaskArr[i_count];
				s->s32Left -= i_count;
				if( s->s32Left == 0 )
				{
					s->pu8P++;
					s->s32Left = 8;
				}
				return( i_result );
			}
			else
			{
				/* less in the buffer than requested */
				i_result |= (*s->pu8P&dwstdh264MaskArr[s->s32Left]) << -i_shr;
				i_count  -= s->s32Left;
				s->pu8P++;
				s->s32Left = 8;
			}
		}

		return( i_result );
}

static u32 stdh264_bs_read1( TParserBitStream *s )
{

	if( s->pu8P < s->pu8End )
	{
		u32 i_result;

		s->s32Left--;
		i_result = ( *s->pu8P >> s->s32Left )&0x01;
		if( s->s32Left == 0 )
		{
			s->pu8P++;
			s->s32Left = 8;
		}
		return i_result;
	}

	return 0;
}

static u32 stdh264_bs_show( TParserBitStream *s, s32 i_count )
{
	if( s->pu8P < s->pu8End && i_count > 0 )
	{
		u32 i_cache = ((s->pu8P[0] << 24)+(s->pu8P[1] << 16)+(s->pu8P[2] << 8)+s->pu8P[3]) << (8-s->s32Left);
		return( i_cache >> ( 32 - i_count) );
	}
	return 0;
}

static void stdh264_bs_skip( TParserBitStream *s, s32 i_count )
{
	s->s32Left -= i_count;

	while( s->s32Left <= 0 )
	{
		s->pu8P++;
		s->s32Left += 8;
	}
}

static s32 stdh264_bs_read_ue( TParserBitStream *s )
{
	s32 i = 0;

	while( stdh264_bs_read1( s ) == 0 && s->pu8P < s->pu8End && i < 32 )
	{
		i++;
	}
	return( ( 1 << i) - 1 + stdh264_bs_read( s, i ) );
}

static s32 stdh264_bs_read_se( TParserBitStream *s )
{
	s32 val = stdh264_bs_read_ue( s );

	return val&0x01 ? (val+1)/2 : -(val/2);
}

static s32 stdh264_bs_read_te( TParserBitStream *s, s32 x )
{
	if( x == 1 )
	{
		return 1 - stdh264_bs_read1( s );
	}
	else if( x > 1 )
	{
		return stdh264_bs_read_ue( s );
	}
	return 0;
}

static s32 stdh264_FirstPartOfSliceHeader(TParserBitStream *s, 
	TParserstdh264Dec_SliceHeaderData *dec_slice_header)
{
	s32 tmp;
	dec_slice_header->first_mb_in_slice = stdh264_bs_read_ue( s );
	tmp = stdh264_bs_read_ue( s );
	if (tmp>4) 
	{
		tmp -=5;
	}
	dec_slice_header->slice_type = (Parserstdh264SliceType) tmp;

	return 0;
}

static const u8 au8ZZSCAN[16]  = {  0,  1,  4,  8,  5,  2,  3,  6,  9, 12, 13, 10,  7, 11, 14, 15};

static const u8 au8ZZSCAN8[64] =
{  0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63};

static void ScalingList(s32 l32SizeOfScalingList, TParserBitStream *s)
{
	s32 l32Index, l32Scanj, l32TmpScale;
	s32 l32DeltaScale, l32LastScale, l32NextScale;

	l32LastScale      = 8;
	l32NextScale      = 8;

	for(l32Index = 0; l32Index < l32SizeOfScalingList; l32Index++)
	{
		l32Scanj = (l32SizeOfScalingList==16) ? au8ZZSCAN[l32Index]:au8ZZSCAN8[l32Index];
		if(l32NextScale!=0)
		{
			//delta_scale
			l32DeltaScale = stdh264_bs_read_se( s );
			l32NextScale = (l32LastScale + l32DeltaScale + 256) % 256;
		}
		l32TmpScale = (l32NextScale==0) ? l32LastScale:l32NextScale;
		l32LastScale = l32TmpScale;
	}
}

static BOOL32 DecodeH264SPS( TParserBitStream *s, TParserSeqParameterSetRBSP *sps, 
	TFrameInfo *ptVideoFrameInfo)
{
	u32 i;
	s32 chroma_format_idc, seq_scaling_matrix_present_flag; 
	s32 seq_scaling_list_present_flag;
	u32 crop_unit_x, crop_unit_y; 
	u32 sps_pic_height;

	sps->profile_idc               = stdh264_bs_read( s, 8 );
	ptVideoFrameInfo->m_dwH264ProfileValue = sps->profile_idc;
	sps->constrained_set0_flag     = stdh264_bs_read( s, 1 );
	sps->constrained_set1_flag     = stdh264_bs_read( s, 1 );
	sps->constrained_set2_flag     = stdh264_bs_read( s, 1 );
	stdh264_bs_skip( s, 5 );        //reserved_zero

	sps->level_idc                 = stdh264_bs_read( s, 8 );

	sps->seq_parameter_set_id      = stdh264_bs_read_ue( s );
	if(sps->profile_idc  == 100) //HighProfile 
	{ 
		chroma_format_idc = stdh264_bs_read_ue( s );         //chroma_format_idc 

		if(chroma_format_idc == 3) 
			stdh264_bs_read( s, 1 );//residual_colour_transform_flag 

		stdh264_bs_read_ue( s );//bit_depth_luma_minus8 
		stdh264_bs_read_ue( s );//bit_depth_chroma_minus8 

		stdh264_bs_read( s, 1 );// lossless_qpprime_flag 
		seq_scaling_matrix_present_flag = stdh264_bs_read( s, 1 );//seq_scaling_matrix_present_flag 

		//ScalingMatrix; 
		if(seq_scaling_matrix_present_flag) 
		{ 
			for(i = 0; i < 8; i ++) 
			{ 
				seq_scaling_list_present_flag = stdh264_bs_read( s, 1 );//seq_scaling_list_present_flag 

				if(seq_scaling_list_present_flag) 
				{ 
					if(i < 6) 
					{ 
						ScalingList(16, s); 
					} 
					else 
					{ 
						ScalingList(64, s); 
					} 
				} 
			} 
		} 
	}         

	sps->log2_max_frame_num_minus4 = stdh264_bs_read_ue( s );
	sps->pic_order_cnt_type        = stdh264_bs_read_ue( s );

	if (sps->pic_order_cnt_type == 0)
	{
		sps->log2_max_pic_order_cnt_lsb_minus4 = stdh264_bs_read_ue( s );
	}
	else if (sps->pic_order_cnt_type == 1)
	{
		sps->delta_pic_order_always_zero_flag      = stdh264_bs_read( s, 1 );
		sps->offset_for_non_ref_pic                = stdh264_bs_read_se( s );
		sps->offset_for_top_to_bottom_field        = stdh264_bs_read_se( s );
		sps->num_ref_frames_in_pic_order_cnt_cycle = stdh264_bs_read_ue( s );
		if (sps->num_ref_frames_in_pic_order_cnt_cycle > MAXNUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE)
		{
			return FALSE;
		}
		for(i=0; i<sps->num_ref_frames_in_pic_order_cnt_cycle; i++)
		{
			sps->offset_for_ref_frame[i]           = stdh264_bs_read_se( s );
		}
	}

	sps->num_ref_frames                        = stdh264_bs_read_ue( s );
	sps->gaps_in_frame_num_value_allowed_flag  = stdh264_bs_read( s, 1 );
	sps->pic_width_in_mbs_minus1               = stdh264_bs_read_ue( s );  //  Width
	sps->pic_height_in_map_units_minus1        = stdh264_bs_read_ue( s );  //  Height
	sps->frame_mbs_only_flag                   = stdh264_bs_read( s, 1 );

	sps_pic_height = (2 - sps->frame_mbs_only_flag) * (sps->pic_height_in_map_units_minus1 + 1) * 16;

	if (!sps->frame_mbs_only_flag)
	{
		sps->mb_adaptive_frame_field_flag      = stdh264_bs_read( s, 1 );
	}
	sps->direct_8x8_inference_flag             = stdh264_bs_read( s, 1 );
	sps->frame_cropping_flag                   = stdh264_bs_read( s, 1 );

	if (sps->frame_cropping_flag) 
	{ 
		crop_unit_x = 2;
		crop_unit_y = 2 * (2 - sps->frame_mbs_only_flag);

		sps->frame_cropping_rect_left_offset   = stdh264_bs_read_ue( s ); 
		sps->frame_cropping_rect_right_offset  = stdh264_bs_read_ue( s ); 
		sps->frame_cropping_rect_top_offset    = stdh264_bs_read_ue( s ); 
		sps->frame_cropping_rect_bottom_offset = stdh264_bs_read_ue( s ); 

		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = (u16)(sps->pic_width_in_mbs_minus1 + 1) * 16
			- (u16)(sps->frame_cropping_rect_left_offset + sps->frame_cropping_rect_right_offset) * (u16)crop_unit_x;

		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = (u16)sps_pic_height
			- (u16)(sps->frame_cropping_rect_top_offset + sps->frame_cropping_rect_bottom_offset) * (u16)crop_unit_y;
	}
	else
	{
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth  = (u16)(sps->pic_width_in_mbs_minus1 + 1) * 16; 
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = (u16)sps_pic_height;
	}  

	sps->vui_parameters_present_flag           = stdh264_bs_read( s, 1 ); 

	if (sps->vui_parameters_present_flag) 
	{ 
		//OspPintf( 1, 0, "VUI sequence parameters present but not supported, ignored, proceeding to next NALU\n"); 
	} 

	sps->bIsValid = TRUE; 

	//	pStdH264Header->m_bIsValidSPS = TRUE; 
	//	pStdH264Header->m_dwSPSId     = sps->seq_parameter_set_id; 

	return TRUE;
}

static BOOL32 DecodeH264PPS( TParserBitStream *s, TParserPicParameterSetRBSP *pps, TFrameInfo *ptVideoFrameInfo)
{
	u32 i;
	s32 NumberBitsPerSliceGroupId;

	pps->pic_parameter_set_id     = stdh264_bs_read_ue( s );
	pps->seq_parameter_set_id     = stdh264_bs_read_ue( s );
	pps->entropy_coding_mode_flag = stdh264_bs_read( s, 1 );
	pps->pic_order_present_flag   = stdh264_bs_read( s, 1 );
	pps->num_slice_groups_minus1  = stdh264_bs_read_ue( s );

	// FMO stuff begins here
	if (pps->num_slice_groups_minus1 > MAXNUM_SLICE_GROUPS_MINNUS1)
	{
		return FALSE;
	}

	if (pps->num_slice_groups_minus1 > 0)
	{
		pps->slice_group_map_type = stdh264_bs_read_ue( s );

		switch( pps->slice_group_map_type )
		{
		case 0:
			{
				for (i=0; i<=pps->num_slice_groups_minus1; i++)
				{
					pps->run_length_minus1[i] = stdh264_bs_read_ue( s );
				}
				break;
			}
		case 2:
			{
				for (i=0; i<pps->num_slice_groups_minus1; i++)
				{
					//! JVT-F078: avoid reference of SPS by using ue(v) instead of u(v)
					pps->top_left [i]                  = stdh264_bs_read_ue( s );
					pps->bottom_right [i]              = stdh264_bs_read_ue( s );
				}
				break;
			}
		case 3:
		case 4:
		case 5:
			{
				pps->slice_group_change_direction_flag = stdh264_bs_read( s, 1 );
				pps->slice_group_change_rate_minus1    = stdh264_bs_read_ue( s );
				break;
			}
		case 6:
			{
				if (pps->num_slice_groups_minus1+1 >4)
				{
					NumberBitsPerSliceGroupId = 3;
				}
				else if (pps->num_slice_groups_minus1+1 > 2)
				{
					NumberBitsPerSliceGroupId = 2;
				}
				else
				{
					NumberBitsPerSliceGroupId = 1;
				}

				//! JVT-F078, exlicitly signal number of MBs in the map
				pps->num_slice_group_map_units_minus1      = stdh264_bs_read_ue( s );

				//hual add 2005-06-24
				if (NULL == pps->slice_group_id)
				{
					break;
				}

				for (i=0; i<=pps->num_slice_group_map_units_minus1; i++)
				{
					pps->slice_group_id[i] = stdh264_bs_read(s,NumberBitsPerSliceGroupId );//maywrong
				}
				break;
			}
		default:
			break;
		}
	}

	// End of FMO stuff
	pps->num_ref_idx_l0_active_minus1           = stdh264_bs_read_ue( s );
	pps->num_ref_idx_l1_active_minus1           = stdh264_bs_read_ue( s );
	pps->weighted_pred_flag                     = stdh264_bs_read( s, 1 );
	pps->weighted_bipred_idc                    = stdh264_bs_read( s, 2 );
	pps->pic_init_qp_minus26                    = stdh264_bs_read_se( s );
	pps->pic_init_qs_minus26                    = stdh264_bs_read_se( s );
	pps->chroma_qp_index_offset                 = stdh264_bs_read_se( s );
	pps->deblocking_filter_control_present_flag = stdh264_bs_read( s, 1 );
	pps->constrained_intra_pred_flag            = stdh264_bs_read( s, 1 );
	pps->redundant_pic_cnt_present_flag         = stdh264_bs_read( s, 1 );

	pps->bIsValid = TRUE;
	//	pStdH264Header->m_bIsValidPPS = TRUE;

	return TRUE;
}

//(3)analysis ps frame function and struct 
typedef struct tagParserBit
{
	u8 *pu8Start;
	u8 *pu8Current;
	u8 *pu8End;
	s32 s32Left;
}TParserBit;

static void ParseBitsInit(TParserBit *ptBits, void *pvData, s32 s32Length)
{
	ptBits->pu8Start   = (u8 *)pvData;
	ptBits->pu8Current = (u8 *)pvData;
	ptBits->pu8End     = ptBits->pu8Current + s32Length;
	ptBits->s32Left    = 8;
}

static void ParseBitsSkip(TParserBit *ptBits, u32 u32BitsCount)
{
	ptBits->s32Left -= u32BitsCount;

	while(ptBits->s32Left <= 0)
	{
		ptBits->pu8Current++;
		ptBits->s32Left += 8;
	}
}

static u8 ParseBitsRead8(TParserBit *ptBits, u8 u8BitsCount)
{
	s8 s8Shr = 0;
	u8 u8Result = 0;

	static u8 u8Mask8[9] =
	{  
		0x00,
		0x01,      0x03,      0x07,      0x0f,
		0x1f,      0x3f,      0x7f,      0xff,
	};

	CHECK_BITSCOUNT(u8BitsCount, 8);

	while(u8BitsCount > 0)                                                              
	{                                                                               
		if(ptBits->pu8Current >= ptBits->pu8End)/*保证没有到字符结尾*/              
		{                                                                           
			break;                                                                  
		}                                                                           
		if((s8Shr = ptBits->s32Left - u8BitsCount) >= 0)/*可用位数大于需要读的位数*/    
		{                                                                           
			u8Result |= (*ptBits->pu8Current >> s8Shr) & u8Mask8[u8BitsCount];             
			ptBits->s32Left -= u8BitsCount;                                             
			if(ptBits->s32Left == 0)/*当前指针字节已经没有可用位*/                  
			{                                                                       
				ptBits->pu8Current++;/*移动指针到下一字节*/                         
				ptBits->s32Left = 8;/*可用位数量恢复*/                
			}                                                                       
			return u8Result;                                                        
		}                                                                           
		else/*如果当前字节可用位不够*/                                              
		{                                                                           
			u8Result |= (*ptBits->pu8Current & u8Mask8[ptBits->s32Left]) << -s8Shr;    
			u8BitsCount  -= ptBits->s32Left;/*计算还要读取下一字节的位数*/              
			ptBits->pu8Current++;/*移动指针到下一字节*/                             
			ptBits->s32Left = 8;/*可用位数量恢复*/                    
		}                                                                           
	}

	return u8Result;
}

static u16 ParseBitsRead16(TParserBit *ptBits, u8 u8BitsCount)
{
	s8  s8Shr = 0;
	u16 u16Result = 0;

	static u16 u16Mask16[17] =
	{  
		0x00,
		0x01,      0x03,      0x07,      0x0f,
		0x1f,      0x3f,      0x7f,      0xff,
		0x1ff,     0x3ff,     0x7ff,     0xfff,
		0x1fff,    0x3fff,    0x7fff,    0xffff,
	};

	CHECK_BITSCOUNT(u8BitsCount, 16);

	while(u8BitsCount > 0)                                                              
	{                                                                               
		if(ptBits->pu8Current >= ptBits->pu8End)/*保证没有到字符结尾*/              
		{                                                                           
			break;                                                                  
		}                                                                           
		if((s8Shr = ptBits->s32Left - u8BitsCount) >= 0)/*可用位数大于需要读的位数*/    
		{                                                                           
			u16Result |= (*ptBits->pu8Current >> s8Shr) & u16Mask16[u8BitsCount];             
			ptBits->s32Left -= u8BitsCount;                                             
			if(ptBits->s32Left == 0)/*当前指针字节已经没有可用位*/                  
			{                                                                       
				ptBits->pu8Current++;/*移动指针到下一字节*/                         
				ptBits->s32Left = 8;/*可用位数量恢复*/                
			}                                                                       
			return u16Result;                                                        
		}                                                                           
		else/*如果当前字节可用位不够*/                                              
		{                                                                           
			u16Result |= (*ptBits->pu8Current & u16Mask16[ptBits->s32Left]) << -s8Shr;    
			u8BitsCount  -= ptBits->s32Left;/*计算还要读取下一字节的位数*/              
			ptBits->pu8Current++;/*移动指针到下一字节*/                             
			ptBits->s32Left = 8;/*可用位数量恢复*/                    
		}                                                                           
	}

	return u16Result;
}

static u32 ParseBitsRead32(TParserBit *ptBits, u8 u8BitsCount)
{
	s8  s8Shr = 0;
	u32 u32Result = 0;

	static u32 u32Mask32[33] =
	{  
		0x00,
		0x01,      0x03,      0x07,      0x0f,
		0x1f,      0x3f,      0x7f,      0xff,
		0x1ff,     0x3ff,     0x7ff,     0xfff,
		0x1fff,    0x3fff,    0x7fff,    0xffff,
		0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
		0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
		0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
		0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff
	};

	CHECK_BITSCOUNT(u8BitsCount, 32);

	while(u8BitsCount > 0)                                                              
	{                                                                               
		if(ptBits->pu8Current >= ptBits->pu8End)/*保证没有到字符结尾*/              
		{                                                                           
			break;                                                                  
		}                                                                           
		if((s8Shr = ptBits->s32Left - u8BitsCount) >= 0)/*可用位数大于需要读的位数*/    
		{                                                                           
			u32Result |= (*ptBits->pu8Current >> s8Shr) & u32Mask32[u8BitsCount];             
			ptBits->s32Left -= u8BitsCount;                                             
			if(ptBits->s32Left == 0)/*当前指针字节已经没有可用位*/                  
			{                                                                       
				ptBits->pu8Current++;/*移动指针到下一字节*/                         
				ptBits->s32Left = 8;/*可用位数量恢复*/                
			}                                                                       
			return u32Result;                                                        
		}                                                                           
		else/*如果当前字节可用位不够*/                                              
		{                                                                           
			u32Result |= (*ptBits->pu8Current & u32Mask32[ptBits->s32Left]) << -s8Shr;    
			u8BitsCount  -= ptBits->s32Left;/*计算还要读取下一字节的位数*/              
			ptBits->pu8Current++;/*移动指针到下一字节*/                             
			ptBits->s32Left = 8;/*可用位数量恢复*/                    
		}                                                                           
	}

	return u32Result;
}

static s32 find0x001( u8 *pu8Buf, u32 u32Len)
{
	u32 u32Pos = 0;
	while (u32Pos + 2 < u32Len)
	{
		if (pu8Buf[u32Pos] == 0 && pu8Buf[u32Pos+1] == 0 && pu8Buf[u32Pos+2] == 1)
		{
			return u32Pos;
		}
		u32Pos++;
	}
	return -1;
}
static s32 find0x001And0x0001( u8 *pu8Buf, u32 u32Len, u8 *u8offset)
{
	u32 u32Pos = 0;
	while (u32Pos + 3 <=  u32Len)
	{
		if (pu8Buf[u32Pos] == 0 && pu8Buf[u32Pos+1] == 0 && pu8Buf[u32Pos+2] == 1)
		{
			*u8offset = 3;
			return u32Pos;
		}
		else if (pu8Buf[u32Pos] == 0 && pu8Buf[u32Pos+1] == 0 &&
			pu8Buf[u32Pos+2] == 0 && pu8Buf[u32Pos+3] == 1)
		{
			*u8offset = 4;
			return u32Pos;
		}
		u32Pos++;
	}
	return -1;
}

static s32 parsepshead(TParserPs *ptPsRead, u8 *pu8Buf, u32 u32Len)
{
	TParserBit tBitsBuf;
	u16 u16Tmp;

	if (u32Len < 14)
	{
		//	ptPsRead->bReadPsHead = TRUE;
		return -1;
	}
	//   ptPsRead->bReadPsHead = FALSE;
	ParseBitsInit(&tBitsBuf, pu8Buf, u32Len);

	ParseBitsSkip(&tBitsBuf, 32);
	//scr base
	u16Tmp = ParseBitsRead8(&tBitsBuf, 2);
	if (1 != u16Tmp)
	{
		return -1;
	}
	u16Tmp = ParseBitsRead8(&tBitsBuf, 3);
	ParseBitsSkip(&tBitsBuf, 1);
	u16Tmp = ParseBitsRead16(&tBitsBuf, 15);
	ParseBitsSkip(&tBitsBuf, 1);
	u16Tmp = ParseBitsRead16(&tBitsBuf, 15);
	//scr ext
	ParseBitsSkip(&tBitsBuf, 1);
	u16Tmp = ParseBitsRead16(&tBitsBuf, 9);
	//mux rate
	ParseBitsSkip(&tBitsBuf, 1);	
    ParseBitsRead32(&tBitsBuf, 22);
	ParseBitsSkip(&tBitsBuf, 7);
	//padding
	u16Tmp = ParseBitsRead8(&tBitsBuf, 3);
	if (14 + (u32)u16Tmp > u32Len)
	{
		return -1;
	}
	return 14 + u16Tmp;
}

static s32 parsesystemhead(TParserPs *ptPsRead, u8 *pu8Buf, u32 u32Len)
{
	TParserBit tBitsBuf;
	u16 u16systemheadlen = 0;

	ParseBitsInit(&tBitsBuf, pu8Buf, u32Len);

	ParseBitsSkip(&tBitsBuf, 32);
	u16systemheadlen  = ParseBitsRead16(&tBitsBuf, 16);

	if (6 + (u32)u16systemheadlen  > u32Len)
	{
		return -1;
	}

	return 6 + u16systemheadlen;
}

static u8 getstreamidbytype(u8 u8StreamType)
{
	switch (u8StreamType)
	{
	case PS_PAYLOAD_MP1:
	case PS_PAYLOAD_MP2:
	case PS_PAYLOAD_AACLC:
	case PS_PAYLOAD_MP2AAC:
	case PS_PAYLOAD_G711A:
	case PS_PAYLOAD_G7221:
	case PS_PAYLOAD_G7231:
	case PS_PAYLOAD_G729:
	case PS_PAYLOAD_SVACA:	
		return PES_AUDIOSTREAM;

	case PS_PAYLOAD_MPEG1:
	case PS_PAYLOAD_MPEG2:
	case PS_PAYLOAD_MPEG4:
	case PS_PAYLOAD_H264:
    case PS_PAYLOAD_H265_OLD:
    case PS_PAYLOAD_H265:
	case PS_PAYLOAD_SVACV:
		return PES_VIDEOSTREAM;

	default:
		return 0x00;
	}
}


static s32 parsepsmhead(TParserPs *ptPsRead, u8 *pu8Buf, u32 u32Len)
{
	TParserBit tBitsBuf;
	s32 s32i;
	u16 u16esinfolength = 0;
	u16 u16maplength = 0;
	u16 u16psinfolength = 0;
	u16 u16esmaplength = 0;
	s32 s32k = 0;	
	u8 byType = 0;
	u8 byEsId = 255;
	ptPsRead->byVideoStreamNum = 0;
	ptPsRead->byAudioStreamNum = 0;
	ParseBitsInit(&tBitsBuf, pu8Buf, u32Len);
	ParseBitsSkip(&tBitsBuf, 32);
	u16maplength = ParseBitsRead16(&tBitsBuf, 16);

	if (u32Len < (u32)u16maplength + 6)
	{
		return -1;
	}

	ParseBitsRead8(&tBitsBuf, 1);
	ParseBitsSkip(&tBitsBuf, 2);
	ParseBitsRead8(&tBitsBuf, 5);
	ParseBitsSkip(&tBitsBuf, 8);

	u16psinfolength = ParseBitsRead16(&tBitsBuf, 16);
	for (s32i = 0; s32i < u16psinfolength; s32i++)
	{
		ParseBitsSkip(&tBitsBuf, 8);
	}

	u16esmaplength = ParseBitsRead16(&tBitsBuf, 16);
	if (10 + (u32)u16psinfolength + 2 + (u32)u16esmaplength > u32Len)
	{
		return -1;
	}

	if (u16esmaplength > 1024)
	{
		KDVSTREAMPARSERPRINTF(TRUE, FALSE,  "EsMapLength is larger than 1024, error!\n");
	}

	if (u16esmaplength / 4 < MAX_STREAMNUM_IN_A_PSSTREAM)
	{
		ptPsRead->u8StreamNum = u16esmaplength / 4;
		for (s32i=0; s32i<ptPsRead->u8StreamNum; s32i++)
		{			
			if (s32i >= MAX_STREAMNUM_IN_A_PSSTREAM)
			{
				break;
			}
			byType = ParseBitsRead8(&tBitsBuf, 8);
			byEsId = ParseBitsRead8(&tBitsBuf, 8);
			if (byEsId >= 0xE0 && byEsId <= 0xEF)
			{
				ptPsRead->byVideoStreamNum++;
			}
			else if (byEsId >= 0XC0 && byEsId <= 0xCF)
			{
				ptPsRead->byAudioStreamNum++;
			}
			ptPsRead->au8StreamType[byEsId] = byType;
			u16esinfolength = ParseBitsRead16(&tBitsBuf, 16);
			for (s32k=0; s32k<u16esinfolength; s32k++)
			{
				ParseBitsSkip(&tBitsBuf, 8);
			}
		}
	}
	else
	{
		s32i = 0;
		while (u16esmaplength > 0)
		{
			byType = ParseBitsRead8(&tBitsBuf, 8);
			byEsId = ParseBitsRead8(&tBitsBuf, 8);
			ptPsRead->au8StreamType[byEsId] = byType;
			u16esinfolength = ParseBitsRead16(&tBitsBuf, 16);		
			u16esmaplength -= 4;
			for (s32k=0; s32k<u16esinfolength; s32k++)
			{
				ParseBitsSkip(&tBitsBuf, 8);
				u16esmaplength -= 1;
			}
			s32i++;
			if (s32i >= 2/*MAX_STREAM_NUM_IN_PROGRAM*/)
			{
				break;
			}
		}
		ptPsRead->u8StreamNum = s32i;
	}

	return u16maplength + 6;	
}

static u16 readpesinfo(TParserPs *ptPsRead, u8 *pu8BufInput, u32 u32LenInput, u32 * u32LenOutput)
{
	TParserBit tBitsBuf;
	u8 u8Prefix1 = 0, u8Prefix2 = 0, u8Prefix3 = 0;
	u8 u8StreamId = 0;
	u8 u8Temp = 0;
	u64 u64Temp1 = 0L, u64Temp2 = 0L, u64Temp3 = 0L;
	u32 u32PayloadStart = 0;
	u8 *pu8PesBuffer = NULL;
	u32 u32PesLength = 0;
	u8 u8PtsDtsFlag = 0;
	u8 u8HeadDataLength = 0;
	u64 u64Pts = 0;	
	u8 u8StreamNum = 0;
	u8 byAacIndex = 0;
	s32 nToc = 0;
	u8 bystereo = 0;
	u8 byConfig = 0;
	*u32LenOutput = 0;


	if((NULL == ptPsRead) || (NULL == pu8BufInput) || (0 == u32LenInput))
	{
		return -1;
	}

	pu8PesBuffer = pu8BufInput;
	u32PesLength = u32LenInput;

	ParseBitsInit(&tBitsBuf, pu8PesBuffer, u32PesLength);

	u8Prefix1 = ParseBitsRead8(&tBitsBuf, 8);
	u8Prefix2 = ParseBitsRead8(&tBitsBuf, 8);
	u8Prefix3 = ParseBitsRead8(&tBitsBuf, 8);

	if((0x00  != u8Prefix1) || (0x00  != u8Prefix2) || (0x01 != u8Prefix3))
	{
		return -1;
	}

	u8StreamId = ParseBitsRead8(&tBitsBuf, 8);
	ptPsRead->u8StreamId = u8StreamId;

	ParseBitsRead32(&tBitsBuf, 16);// pes packet len 
	if((u8StreamId != PES_PROGRAM_STREAM_MAP) 
		&& (u8StreamId != PES_PADDING_STREAM) 
		&& (u8StreamId != PES_PRIVATE_STREAM_2) 
		&& (u8StreamId != PES_ECM_STREAM) 
		&& (u8StreamId != PES_EMM_STREAM) 
		&& (u8StreamId != PES_PROGRAM_STREAM_DIRECTORY))
	{
		//固定标记
		u8Temp = ParseBitsRead8(&tBitsBuf, 2);
		if(u8Temp != 0x02)
		{
			KDVSTREAMPARSERPRINTF(TRUE, FALSE, "PesRead fail: fix flag 0x02 wrong.\n");
			return -1;
		}

		//ES特有信息
		ParseBitsRead8(&tBitsBuf, 2);//加密
		ParseBitsRead8(&tBitsBuf, 1);//优先级
		ParseBitsRead8(&tBitsBuf, 1);//数据对齐
		ParseBitsRead8(&tBitsBuf, 1);//版权
		ParseBitsRead8(&tBitsBuf, 1);//原始数据/拷贝
		u8PtsDtsFlag = ParseBitsRead8(&tBitsBuf, 2);//是否有PTS,DTS
		ParseBitsRead8(&tBitsBuf, 1);//是否有ESCR
		ParseBitsRead8(&tBitsBuf, 1);//码率
		ParseBitsRead8(&tBitsBuf, 1);//控制信息
		ParseBitsRead8(&tBitsBuf, 1);//额外信息
		ParseBitsRead8(&tBitsBuf, 1);//CRC
		ParseBitsRead8(&tBitsBuf, 1);//拓展标志

		//可变头长度
		u8HeadDataLength = ParseBitsRead8(&tBitsBuf, 8);
		*u32LenOutput += 3;

		//只有PTS信息
		if(0x02 == u8PtsDtsFlag)
		{
			u8Temp = ParseBitsRead8(&tBitsBuf, 4);
			if(0x02 != u8Temp)
			{
				KDVSTREAMPARSERPRINTF(TRUE, FALSE, 
					"PesRead fail: incomfortable PTS_DTS_FLAG. input[%02X], parsed[%02X] \n",
					u8PtsDtsFlag, u8Temp);
				return -1;
			}
			u64Temp1 = ParseBitsRead8(&tBitsBuf, 3);
			ParseBitsSkip(&tBitsBuf, 1);
			u64Temp2 = ParseBitsRead16(&tBitsBuf, 15);
			ParseBitsSkip(&tBitsBuf, 1);
			u64Temp3 = ParseBitsRead16(&tBitsBuf, 15);
			ParseBitsSkip(&tBitsBuf, 1);
			*u32LenOutput += 5;
			u64Pts = (u64Temp1 << 30) + (u64Temp2 << 15) + u64Temp3;
			ptPsRead->u64Dts = u64Pts;
		}

		//同时有PTS和DTS信息
		if(0x03 == u8PtsDtsFlag)
		{
			u8Temp = ParseBitsRead8(&tBitsBuf, 4);
			if(0x03 != u8Temp)
			{
				KDVSTREAMPARSERPRINTF(TRUE, FALSE, 
					"PesRead fail: incomfortable PTS_DTS_FLAG. input[%02X], parsed[%02X]\n",
					u8PtsDtsFlag, u8Temp);
				return -1;
			}
			u64Temp1 = ParseBitsRead8(&tBitsBuf, 3);
			ParseBitsSkip(&tBitsBuf, 1);
			u64Temp2 = ParseBitsRead16(&tBitsBuf, 15);
			ParseBitsSkip(&tBitsBuf, 1);
			u64Temp3 = ParseBitsRead16(&tBitsBuf, 15);
			ParseBitsSkip(&tBitsBuf, 1);

			u64Pts = (u64Temp1 << 30) + (u64Temp2 << 15) + u64Temp3;

			u8Temp = ParseBitsRead8(&tBitsBuf, 4);
			if(0x01 != u8Temp)
			{
				KDVSTREAMPARSERPRINTF(TRUE, FALSE,  "PesRead fail: fix bits in PTS 0x01 wrong.\n");
				return -1;
			}
			u64Temp1 = ParseBitsRead8(&tBitsBuf, 3);
			ParseBitsSkip(&tBitsBuf, 1);
			u64Temp2 = ParseBitsRead16(&tBitsBuf, 15);
			ParseBitsSkip(&tBitsBuf, 1);
			u64Temp3 = ParseBitsRead16(&tBitsBuf, 15);
			ParseBitsSkip(&tBitsBuf, 1);
			*u32LenOutput += 10;
			ptPsRead->u64Dts = (u64Temp1 << 30) + (u64Temp2 << 15) + u64Temp3;
		}
	
		//负载开始位置
		u32PayloadStart = PES_DATAHEAD_LENGTH + PES_ESINFO_LENGTH + u8HeadDataLength;

		//得到ES流位置和长度
		if (u32PesLength < u32PayloadStart)
		{
			return -1;
		}
		ptPsRead->u32PayloadLength = u32PesLength - u32PayloadStart;
		ptPsRead->pu8PayloadBuffer = pu8PesBuffer + u32PayloadStart;
	}
	else if (u8StreamId == PES_PADDING_STREAM)
	{
		//负载从6字节(48位)后开始
		if (u32PesLength < PES_DATAHEAD_LENGTH)
		{
			return -1;
		}
		ptPsRead->u32PayloadLength = u32PesLength - PES_DATAHEAD_LENGTH;
		ptPsRead->pu8PayloadBuffer = pu8PesBuffer + PES_DATAHEAD_LENGTH;

		KDVSTREAMPARSERPRINTF(TRUE, FALSE, "PesRead fail: received a padding stream.\n");
		return -1;
	}
	else
	{
		//负载从6字节(48位)后开始
		if (u32PesLength < PES_DATAHEAD_LENGTH)
		{
			return -1;
		}
		ptPsRead->u32PayloadLength = u32PesLength - PES_DATAHEAD_LENGTH;
		ptPsRead->pu8PayloadBuffer = pu8PesBuffer + PES_DATAHEAD_LENGTH;

		KDVSTREAMPARSERPRINTF(TRUE, FALSE,  "PesRead fail: received an unknown stream.\n");
		return -1;
	}
	if (ptPsRead->u8StreamId >= 0xC0 && ptPsRead->u8StreamId <= 0xCF) //默認音頻AACLC OPUS帶頭信息，不帶頭信息 解析有誤
	{

		switch (ptPsRead->au8StreamType[u8StreamId])
		{
		case PS_PAYLOAD_AACLC:
		case PS_PAYLOAD_MP2AAC:
		{
			ParseBitsSkip(&tBitsBuf, 18);
			byAacIndex = ParseBitsRead8(&tBitsBuf, 4);
			ptPsRead->m_tAudioParam[u8StreamId].sample_rate = s_anmpeg4audio_sample_rates[byAacIndex];
			ParseBitsSkip(&tBitsBuf, 1);
			ptPsRead->m_tAudioParam[u8StreamId].channels = ParseBitsRead8(&tBitsBuf, 3);
			break;
		}
		case PS_PAYLOAD_OPUS://NETPACKET_PT_STREAM_TYPE_OPUS
		{
			nToc = ParseBitsRead8(&tBitsBuf, 8);
			bystereo = (nToc >> 2) & 0x1;
			if (bystereo > 0)
			{
				ptPsRead->m_tAudioParam[u8StreamId].channels = 2;
			}
			else
			{
				ptPsRead->m_tAudioParam[u8StreamId].channels = 1;
			}
			byConfig = (nToc >> 3) & 0x1F;
			switch (byConfig)
			{
			case 1:
				ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 8000;
				break;
			case 5:
				ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 12000;
				break;
			case 9:
				ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 16000;
				break;
			case 13:
				ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 24000;
				break;
			case 15:
				ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 48000;
				break;
			default:
				break;
			}
			break;
		}

		case PS_PAYLOAD_G711A:
		case PS_PAYLOAD_G7231:
		case PS_PAYLOAD_G729:
			ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 8000;
			ptPsRead->m_tAudioParam[u8StreamId].channels = 1;
			break;

		case PS_PAYLOAD_G7221:
			ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 16000;
			ptPsRead->m_tAudioParam[u8StreamId].channels = 1;
			break;
		default:
		{
			ptPsRead->m_tAudioParam[u8StreamId].sample_rate = 8000;
			ptPsRead->m_tAudioParam[u8StreamId].channels = 1;
			break;
		}
		}
	}
	return 0;
}

static s32 parsepeshead(TParserPs *ptPsRead, u8 *pu8Buf, u32 u32Len)
{
	TParserBit tBitsBuf;
	u16 u16PesLen = 0;
	u16 u16Ret;
	u32 u32LenOutput;
	u64 u64oldpesdts = 0;
	u8 u8StreamType = 255;

	ParseBitsInit(&tBitsBuf, pu8Buf, u32Len);

	ParseBitsSkip(&tBitsBuf, 32);
	u16PesLen = ParseBitsRead16(&tBitsBuf, 16);

//	if (/*u16PesLen == 0 ||*/ (u32)u16PesLen + 6 > u32Len)
//		return -1;

	u64oldpesdts = ptPsRead->u64Dts;
	
	if (0 == u16PesLen)
	{
		u16Ret = readpesinfo(ptPsRead, pu8Buf, u32Len, &u32LenOutput);
	}
	else
	{
		if((u32)(u16PesLen+6) < u32Len)
		{
			u16Ret = readpesinfo(ptPsRead, pu8Buf, u16PesLen + 6, &u32LenOutput);
		}
		else
		{
			u16Ret = readpesinfo(ptPsRead, pu8Buf, u32Len, &u32LenOutput);
		}
	}

	if (0 == u16Ret) // correct 
	{		

		if (//ptPsRead->u8StreamId == ptPsRead->u8AudioID ||
			ptPsRead->u8StreamId >= 0xE0 && ptPsRead->u8StreamId <= 0xEF) //  just process video data,
		{
			if (0 != u16PesLen)
			{
				{
					memcpy(ptPsRead->pu8FrameBuf + ptPsRead->u32FrameLen, 
						ptPsRead->pu8PayloadBuffer, ptPsRead->u32PayloadLength);
					ptPsRead->u32FrameLen += ptPsRead->u32PayloadLength; 
				}
			}			       
		}
	}
	else
	{
		// 出错时,跳过改PES分组,直接返回改分组长度
		return (u16PesLen + 6);
	}

	if (0 == u16PesLen)
	{
		return u32LenOutput + 6;
	}
	else
	{
		return u16PesLen + 6;
	}
}

//static s32 parseotherstream(TParserPs *ptPsRead, u8 *pu8Buf, u32 u32Len)
//{
//    TParserBit tBitsBuf;
//    u16 u16HeadLen;

//	if (ptPsRead->u8StreamId == ptPsRead->u8AudioID ||
//		ptPsRead->u8StreamId == ptPsRead->u8VideoID)
//	{
//		if (ptPsRead->u32FrameLen < MAX_PSFRAME_SIZE)
//		{
//			//copy 0001 to frame buf
//			memcpy(ptPsRead->pu8FrameBuf + ptPsRead->u32FrameLen, 
//			pu8Buf, 3);
//			ptPsRead->u32FrameLen += 3;
//		}
//		return 3;
//	}

//    BitsInit(&tBitsBuf, pu8Buf, u32Len);
//    BitsSkip(&tBitsBuf, 32);
//    u16HeadLen = BitsRead16(&tBitsBuf, 16);
//    if (4 + (u32)u16HeadLen > u32Len)
//	        return -1;

//    KDVSTREAMPARSERPRINTF(1,0, "PsRead read an unknown pack<%02X> and skip.", pu8Buf[3]);
//    return 4 + u16HeadLen;
//}

static u16 JudgeIsAudio( u8 byMediaType, BOOL32 *pbAud )
{
	if( NULL == pbAud )
	{
		return 1;
	}
	
	/* 如果是不支持的码流类型，退出 */ 
	if( !( (byMediaType == MEDIA_TYPE_PCMU) || (byMediaType == MEDIA_TYPE_PCMA) ||
		(byMediaType == MEDIA_TYPE_G728) || (byMediaType == MEDIA_TYPE_G722) ||
		(byMediaType == MEDIA_TYPE_G729) || (byMediaType == MEDIA_TYPE_MP3) ||
		(byMediaType == MEDIA_TYPE_ADPCM) || (byMediaType == MEDIA_TYPE_G7221C) ||
		(byMediaType == MEDIA_TYPE_G7231) || (byMediaType == MEDIA_TYPE_AACLC) ||
		(byMediaType == MEDIA_TYPE_AACLD) || (byMediaType == MEDIA_TYPE_AACLC_PCM) ||
		(byMediaType == MEDIA_TYPE_MP4) ||	(byMediaType == MEDIA_TYPE_H262) ||
		(byMediaType == MEDIA_TYPE_H261) ||
		(byMediaType == MEDIA_TYPE_H263) || (byMediaType == MEDIA_TYPE_H263PLUS) ||
		(byMediaType == MEDIA_TYPE_H264) || (byMediaType== MEDIA_TYPE_AMR) ) )
	{
		return 1;
	}
	
	if( (byMediaType == MEDIA_TYPE_PCMU) ||	(byMediaType == MEDIA_TYPE_PCMA) ||
		(byMediaType == MEDIA_TYPE_G728) || (byMediaType == MEDIA_TYPE_G722) ||
		(byMediaType == MEDIA_TYPE_G729) || (byMediaType == MEDIA_TYPE_G7231) ||
		(byMediaType == MEDIA_TYPE_ADPCM) || (byMediaType == MEDIA_TYPE_G7221C) ||
		(byMediaType == MEDIA_TYPE_MP3) ||(byMediaType == MEDIA_TYPE_AACLC)||
		(byMediaType == MEDIA_TYPE_AACLD) ||(byMediaType == MEDIA_TYPE_AACLC_PCM) ||
		(byMediaType == MEDIA_TYPE_AMR))
	{
		*pbAud = TRUE;
	}
	else
	{
		*pbAud = FALSE;
	}
	
	return 0;
}

//analysis rtp extend head ,get frame information.
//input parameters must be analysised by function ParseRtpHead
static BOOL32 ParseRtpExtendHead(TRtpAndFrameInfo *ptRtpAndFrameInfo)
{
	BOOL32 bRet = TRUE;
	u8 byauiomode = 0;
	u32 exdata0 = 0;
	BOOL32 bAudio = FALSE;

	switch (ptRtpAndFrameInfo->tRtpInfo.m_byPayload)
	{	
		//AUDIO part, Get audio mode
	case MEDIA_TYPE_G7231:
		byauiomode = AUDIOMODE_G723_6;
		break;
	case MEDIA_TYPE_PCMU:
		byauiomode = AUDIOMODE_PCMU;
		break;
	case MEDIA_TYPE_PCMA:
		byauiomode = AUDIOMODE_PCMA;
		break;
	case MEDIA_TYPE_G722:
		byauiomode = AUDIOMODE_G722;
		break;
	case MEDIA_TYPE_G728:
		byauiomode  = AUDIOMODE_G728;
		break;
	case MEDIA_TYPE_G729:
		byauiomode = AUDIOMODE_G729;
		break;

	case MEDIA_TYPE_AACLC:
		if(ptRtpAndFrameInfo->tRtpInfo.m_byExtence && 
			ptRtpAndFrameInfo->tRtpInfo.m_nExSize >= EXTENDRTPHEAD_AUDIOMODE_POS )
		{
			byauiomode = 
				ptRtpAndFrameInfo->tRtpInfo.m_pExData[EXTENDRTPHEAD_AUDIOMODE_POS];
		}
		else
		{
			byauiomode = 0;
		}
		break;
	case MEDIA_TYPE_AACLD:
	case MEDIA_TYPE_ADPCM:
	case MEDIA_TYPE_G7221C:
	case MEDIA_TYPE_AACLC_PCM:
	case MEDIA_TYPE_AMR:
		//	ptRtpAndFrameInfo->tFrameInfo.m_byAudioMode 
		//= ptRtpAndFrameInfo->tRtpInfo.m_byPayload;
		break;

		//VIDEO  part, get framerate 'width ' height key frame information  
	case MEDIA_TYPE_H264:
		if(ptRtpAndFrameInfo->tRtpInfo.m_byExtence && 
			ptRtpAndFrameInfo->tRtpInfo.m_nExSize >= 2)
		{
			exdata0 = ntohl(*(u32 *)ptRtpAndFrameInfo->tRtpInfo.m_pExData); // trans net order to host
			//get height and width 
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_wVideoWidth =
				(u16)((exdata0 & 0x00FFF000) >> 12); // resolution in extend  data
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_wVideoHeight =
				(u16)(exdata0 & 0x00000FFF);
			//get framerate 
			ptRtpAndFrameInfo->tFrameInfo.m_byFrameRate =
				*((u8*)ptRtpAndFrameInfo->tRtpInfo.m_pExData+7);	
		}
		else
		{
			//default framerate 
			ptRtpAndFrameInfo->tFrameInfo.m_byFrameRate = 25;
		}
		break;
	case MEDIA_TYPE_MP4:
		if (ptRtpAndFrameInfo->tRtpInfo.m_nExSize*sizeof(u32) <= EXTENDRTPHEAD_MAXLEN && 
			ptRtpAndFrameInfo->tRtpInfo.m_nExSize >= 3)
		{
			//frame rate 
			ptRtpAndFrameInfo->tFrameInfo.m_byFrameRate = 
				(s32)ptRtpAndFrameInfo->tRtpInfo.m_pExData[EXTENDRTPHEAD_VIDEOFRAMERATE_POS];
			//width and height 
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_wVideoWidth =
				ntohs(*((u16 *)(ptRtpAndFrameInfo->tRtpInfo.m_pExData + EXTENDRTPHEAD_VIDEOFRAMEWIDTH_POS)));
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_wVideoHeight =
				ntohs(*((u16 *)(ptRtpAndFrameInfo->tRtpInfo.m_pExData + EXTENDRTPHEAD_VIDEOFRAMEHEIGHT_POS)));	
			//is key frame or not 
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_bKeyFrame =
				ptRtpAndFrameInfo->tRtpInfo.m_pExData[EXTENDRTPHEAD_VIDEOKEYFRAME_POS];
		}
		else
		{
			//default value 
			ptRtpAndFrameInfo->tFrameInfo.m_byFrameRate = 0;
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_wVideoWidth = 0;
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_wVideoHeight = 0;
			ptRtpAndFrameInfo->tFrameInfo.m_tVideoParam.m_bKeyFrame = 0;
		}
		break;

	default:
	//	KDVSTREAMPARSERPRINTF(1,0, "[stream parser ] not get frame info\n");
		bRet = FALSE;
		break;
	}
	
	JudgeIsAudio(ptRtpAndFrameInfo->tRtpInfo.m_byPayload, &bAudio);
	if(bAudio)
	{
		ptRtpAndFrameInfo->tFrameInfo.m_byAudioType = ptRtpAndFrameInfo->tRtpInfo.m_byPayload;
		KdvAudioModeToContext((TKdvCodecContext*)&(ptRtpAndFrameInfo->tFrameInfo.tAudioParam),
			byauiomode, ptRtpAndFrameInfo->tRtpInfo.m_byPayload);
	
	}
	else
		ptRtpAndFrameInfo->tFrameInfo.m_byVideoType = ptRtpAndFrameInfo->tRtpInfo.m_byPayload;

	return bRet;
}

//analysis rtp head and extend head.frame info(Audio mode and video resolution key frame flag )
//come from rtp extern head.input buf must with rtp head.
static BOOL32 ParseRtpHead(u8 *pBuf, s32 nSize, TRtpAndFrameInfo *ptRtpAndFrameInfo)
{
	u32 header0 = 0;
	u32 header1 = 0;
	u32 header2 = 0;
	u8 m_byVertion = 0;
	s32 nOffSet = 0;
	s32 xStart = 0;
	u32 headerxStart = 0;
	BOOL32 bRet = FALSE;

	//analysis rtp head ,get rtp information 
	if( (NULL == pBuf) || (nSize < RTPHEAD_FIXEDLEN) )
	{
		return FALSE;
	}

	memset(ptRtpAndFrameInfo, 0, sizeof(TRtpAndFrameInfo));

	header0 = ntohl(((u32 *)pBuf)[0]);
	header1 = ntohl(((u32 *)pBuf)[1]);
	header2 = ntohl(((u32 *)pBuf)[2]);
	
	//CSRC convert
	//ConvertN2H(pBuf, 3, GetBitField(header[0], 24, 4)); csrc 暂时不需要

	ptRtpAndFrameInfo->tRtpInfo.m_dwTimeStamp = header1;
	m_byVertion  = (u8)GetBitField(header0, 30, 2);
	if (m_byVertion != 2)
	{
		KDVSTREAMPARSERPRINTF(TRUE, FALSE, " [stream parser ]rtp version is wrong ! \n");
		return FALSE;
	}
	ptRtpAndFrameInfo->tRtpInfo.m_wSequence   = (u16)GetBitField(header0, 0, 16);
	ptRtpAndFrameInfo->tRtpInfo.m_dwSSRC      = header2;
	ptRtpAndFrameInfo->tRtpInfo.m_byMark      = (u8)GetBitField(header0, 23, 1);
	ptRtpAndFrameInfo->tRtpInfo.m_byPayload   = (u8)GetBitField(header0, 16, 7);

	nOffSet  = RTPHEAD_FIXEDLEN + GetBitField(header0, 24, 4) * sizeof(u32);//CSRC

	ptRtpAndFrameInfo->tRtpInfo.m_nRealSize   = nSize - nOffSet;
	ptRtpAndFrameInfo->tRtpInfo.m_pRealData   = pBuf + nOffSet; 
	ptRtpAndFrameInfo->tRtpInfo.m_byExtence   = (u8)GetBitField(header0, 28, 1);

	if (ptRtpAndFrameInfo->tRtpInfo.m_byExtence)/*Extension Bit Set*/
	{
		if(ptRtpAndFrameInfo->tRtpInfo.m_nRealSize < 
			EXTENDRTPHEAD_SIZE+EXTENDRTPHEAD_MINLEN) 
		{
			KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[streamparser ] RTP REALSIZE EXCEPTION:m_nRealSize=%d, Sequence = %d In Extence \n", 
				ptRtpAndFrameInfo->tRtpInfo.m_nRealSize, ptRtpAndFrameInfo->tRtpInfo.m_wSequence);
			return FALSE;
		}
		xStart = nOffSet/sizeof(u32);
		headerxStart = ntohl(((u32 *)pBuf)[xStart]);
	
		ptRtpAndFrameInfo->tRtpInfo.m_nExSize    = (u16)GetBitField(headerxStart, 0, 16);
		ptRtpAndFrameInfo->tRtpInfo.m_pExData    = pBuf + (xStart+1) * sizeof(u32);
		ptRtpAndFrameInfo->tRtpInfo.m_nRealSize -= 
			((ptRtpAndFrameInfo->tRtpInfo.m_nExSize + 1)*sizeof(u32));
		ptRtpAndFrameInfo->tRtpInfo.m_pRealData += 
			((ptRtpAndFrameInfo->tRtpInfo.m_nExSize + 1)*sizeof(u32));
	}

	//Padding Bit Set
	if (GetBitField(header0, 29, 1))
	{
		ptRtpAndFrameInfo->tRtpInfo.m_byPadNum  = 1;
	}

	if (ptRtpAndFrameInfo->tRtpInfo.m_nRealSize < 0 || 
		ptRtpAndFrameInfo->tRtpInfo.m_nRealSize > RTPPACK_MAXSIZE)
	{
		KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[stream parser] RTP REALSIZE EXCEPTION:m_nRealSize=%d\n",
			ptRtpAndFrameInfo->tRtpInfo.m_nRealSize);
		return FALSE;
	}

	//call function ParseRtpExtendHead, and get frame information 
	bRet = ParseRtpExtendHead(ptRtpAndFrameInfo);

	return TRUE;
}

//analysis video frame, input buf without rtp head, without ooo1
static BOOL32 ParseH264(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	BOOL32 bRet = TRUE;
	u8  byNaluTypeOctet = 0;
	u32 dwNaluType = 0;
	TParserBitStream tBitStream;
	TParserSeqParameterSetRBSP tSPS;
	TParserPicParameterSetRBSP tPPS;
	TParserstdh264Dec_SliceHeaderData tSlice_header;

	if (nSize <= 1 || NULL == ptVideoFrameInfo)
	{
		return FALSE;
	}

	//clear video frame information
	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		memset(&(ptVideoFrameInfo->m_tVideoParam), 0, sizeof(ptVideoFrameInfo->m_tVideoParam));
	}


	// the format of the NAL unit type octet is reprinted below:
	//  +---------------+
	//	|0|1|2|3|4|5|6|7|
	//	+-+-+-+-+-+-+-+-+
	//	|F|NRI|  Type   |
	//	+---------------+
	byNaluTypeOctet = (u8) (*pBuf);

	dwNaluType = byNaluTypeOctet & 0x1F;

//	if(bhavepsm)	
//		KDVSTREAMPARSERPRINTF(1,0, " bynalutypeoct = %x dwnalutype = %d \n", byNaluTypeOctet, dwNaluType);

	memset(&tBitStream, 0, sizeof(tBitStream));

	if (28 == dwNaluType)
	{
		if ((pBuf[1] & 0x80) > 0)
		{
			dwNaluType = pBuf[1] & 0x1F;
			stdh264_bs_init(&tBitStream, (pBuf+2), (nSize-2));
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		stdh264_bs_init(&tBitStream, (pBuf+1), (nSize-1));
	}

	memset(&tSPS, 0, sizeof(tSPS));
	memset(&tPPS, 0, sizeof(tPPS));
	memset(&tSlice_header, 0, sizeof(tSlice_header));
	
	switch(dwNaluType) 
	{
	case 1:
	case 5:  // NALU_TYPE_IDR
		stdh264_FirstPartOfSliceHeader(&tBitStream, &tSlice_header);
		if( H264_I_SLICE == tSlice_header.slice_type)
		{
			ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = TRUE;
		}
		break;
	case 7:  // NALU_TYPE_SPS
		bRet = DecodeH264SPS(&tBitStream, &tSPS, ptVideoFrameInfo);

		//if i find sps, then record this pack information, in case of can not parsed W/H in this packet.
		if(ptVideoFrameInfo->pbyLastPackPtr && ptVideoFrameInfo->nLastPackLength <= MAX_NALU_SIZE)
		{
			memcpy(ptVideoFrameInfo->byConnectBuf, ptVideoFrameInfo->pbyLastPackPtr, ptVideoFrameInfo->nLastPackLength);
			ptVideoFrameInfo->bLastPackHaveSPS = TRUE;				
		}
		
//		KDVSTREAMPARSERPRINTF(1,0, " have sps \n");
		break;
	case 8:  // NALU_TYPE_PPS
		bRet = DecodeH264PPS(&tBitStream, &tPPS, ptVideoFrameInfo);
		break;
	default:
		break;
	}

	if(bRet)
	{
		ptVideoFrameInfo->m_byVideoType = MEDIA_TYPE_H264;
	}

	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//analysis video frame, input H264 keyframe
//ParseH264Frame不能与ParseFrame合并，返回值与使用场景不能统一
static BOOL32 ParseH264Frame(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	s32 nLoop = 0;
	u8  byNaluTypeOctet = 0;	
	u32 dwNaluType = 0;
	BOOL32 bRet = FALSE;
	// one frame at least has 001
	if (nSize <= 4 || NULL == pBuf || NULL == ptVideoFrameInfo)
	{
		return FALSE;
	}

	for (nLoop = 0; nLoop < nSize; nLoop++)
	{
		if (nLoop > nSize - 4)
		{
			return FALSE;
		}
		else if (0 == pBuf[nLoop] && 0 == pBuf[nLoop+1] && 1 == pBuf[nLoop+2])
		{
			bRet = ParseH264(pBuf+nLoop+3, nSize - nLoop - 3, ptVideoFrameInfo);
		}
		else if (0 == pBuf[nLoop] && 0 == pBuf[nLoop+1] && 0 == pBuf[nLoop+2] && 1 == pBuf[nLoop+3])
		{
			bRet = ParseH264(pBuf+nLoop+4, nSize - nLoop - 4, ptVideoFrameInfo);
		}	
		
		if (TRUE == bRet)
		{
			break;
		}
	}

	return bRet;
}

static BOOL32 ParseWith001(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	u8 *pbydata = pBuf;
	s32 s32maxreadpos = nSize;
	s32 s32startpos = -1;
	u8 arrynula[MAX_NALU_SIZE] = {0};
	BOOL32 bHave001Head = FALSE;
	u8 u8offset = 0;
	s32 arrynaludatalen = 0;

	while(s32maxreadpos > 0)
	{
		u8offset = -1;
		arrynaludatalen = 0;
		s32startpos = find0x001And0x0001(pbydata, s32maxreadpos, &u8offset);
		if(s32startpos < 0)
		{
			//if it do not have 0001 head, just return FALSE when occurrence.
			if(!bHave001Head)
			{
				return FALSE;
			}
			//copy a buff to analysis, because maybe its a last nalu in this packet,its wiil not be analysised when 
			//not find next 0001;
			if(s32maxreadpos > MAX_NALU_SIZE)
			{
				memcpy(arrynula, pbydata, MAX_NALU_SIZE);
				arrynaludatalen = MAX_NALU_SIZE;
			}
			else
			{
				memcpy(arrynula, pbydata, s32maxreadpos);
				arrynaludatalen = s32maxreadpos;	
			}

			//Get Nalu And analysis 
			if(arrynaludatalen)
			{				
			    switch(ptVideoFrameInfo->m_byVideoType)
			        {
                                    case MEDIA_TYPE_H264:
                                    {
                                        ParseH264(arrynula, arrynaludatalen, ptVideoFrameInfo);
                                        if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
                                                ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
                                                ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
                                        {
                                            return TRUE;
                                        }
                                    }
                                    break;
                                    case MEDIA_TYPE_H265:
                                        {
                                            ParseH265(arrynula, arrynaludatalen, ptVideoFrameInfo);
                                            if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
                                                    ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
                                                    ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
                                            {
                                                return TRUE;
                                            }
                                        }
                                        break;
                                    case MEDIA_TYPE_SVACV:
                                        {
                                            ParseSVACV(arrynula, arrynaludatalen, ptVideoFrameInfo);
                                            if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
                                                    ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
                                                    ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
                                            {
                                                return TRUE;
                                            }
                                        }
                                        break;
                                    default:
                                        break;
				}
			}
			return FALSE;
		}
		else if(s32startpos == 0)
		{
			if(u8offset != -1 && s32maxreadpos >= u8offset)
			{
				pbydata += u8offset;
				s32maxreadpos -= u8offset;

				bHave001Head = TRUE;
			}
			else
			{
				break;
			}
		}
		else  // >0
		{
			if(s32maxreadpos > s32startpos)
			{
				if(s32startpos < MAX_NALU_SIZE)
				{
					memcpy(arrynula, pbydata, s32startpos);
					arrynaludatalen = s32startpos;
					pbydata += s32startpos;
					s32maxreadpos -= s32startpos;
				}
				else 
				{
					memcpy(arrynula, pbydata, MAX_NALU_SIZE);
					arrynaludatalen = MAX_NALU_SIZE;
					pbydata += s32startpos;
					s32maxreadpos -= s32startpos;
				}
			}
			else
			{
				break;
			}
		}

		//Get Nalu And analysis 
		if(arrynaludatalen)
		{				
                    switch(ptVideoFrameInfo->m_byVideoType)
                    {
                        case MEDIA_TYPE_H264:
                            {
                                ParseH264(arrynula, arrynaludatalen, ptVideoFrameInfo);
                                if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
                                        ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
                                        ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
                                {
                                    return TRUE;
                                }
                            }
                            break;
                        case MEDIA_TYPE_H265:
                            {
                                ParseH265(arrynula, arrynaludatalen, ptVideoFrameInfo);
                                if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
                                        ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
                                        ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
                                {
                                    return TRUE;
                                }
                            }
                            break;
                        case MEDIA_TYPE_SVACV:
                            {
                                ParseSVACV(arrynula, arrynaludatalen, ptVideoFrameInfo);
                                if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
                                        ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
                                        ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
                                {
                                    return TRUE;
                                }
                            }
                            break;
                        default:
							break;
			}
		}
	}

	return FALSE;
}

/*=============================================================================
	函数集		：stdh265_bs_init ....
	功能		：h265 码流信息分析
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               无           

	返回值说明： 无
=============================================================================*/
static void stdh265_bs_init( TParserBitStream *s, void *p_data, s32 i_data )
{
    u32 u32Next24bit;
	u8 *pu8TempData = NULL, *pu8CurData = (u8 *)p_data, *pu8Head = NULL;
	u8 u8Code;
    s32 s32Len = i_data < 30 ? i_data : 30;
	s32 s32Len1 = s32Len;
	pu8Head = s->au8HeadInfo;

	s->pu8Start = pu8Head;
    s->pu8P     = pu8Head;
    s->pu8End   = (u8*) s->pu8P + s32Len;
    s->s32Left  = 8;
	u32Next24bit = 0xFFFFFFFF;
    memcpy(pu8Head, (u8 *)p_data, i_data < 30 ? i_data : 30);
    pu8TempData = pu8Head;
	do
	{
		u8Code = *pu8CurData++;
		u32Next24bit = ((u32Next24bit << 8) | u8Code) & 0xFFFFFF;
		
		if(u32Next24bit != 0x000003 &&
		   u32Next24bit != 0x000001 &&
		   u32Next24bit != 0x000000)
		{
			//复制-除了竞争码
			*pu8TempData++ = u8Code;
		}
        else
        {
            s32Len -= 1;
        }
	}while((u32Next24bit != 0x000001) && (u32Next24bit != 0x000000) && (pu8CurData < ((u8 *)p_data + s32Len1)));
	s->pu8End = s->pu8Start + s32Len;
}

static s32 stdh265_bs_pos( TParserBitStream *s )
{
    return( 8 * ( s->pu8P - s->pu8Start ) + 8 - s->s32Left );
}

static s32 stdh265_bs_eof( TParserBitStream *s )
{
    return( s->pu8P >= s->pu8End ? 1: 0 );
}

static u32 stdh265_bs_read( TParserBitStream *s, s32 i_count )
{
	static u32 dwstdh264MaskArr[33] = 
	{ 
		0x00,
        0x01,      0x03,      0x07,      0x0f,
        0x1f,      0x3f,      0x7f,      0xff,
        0x1ff,     0x3ff,     0x7ff,     0xfff,
        0x1fff,    0x3fff,    0x7fff,    0xffff,
        0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
        0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
        0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
        0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff };
		
    s32 i_shr;
    u32 i_result = 0;

    while( i_count > 0 )
    {
        if( s->pu8P >= s->pu8End )
        {
            break;
        }

        if( ( i_shr = s->s32Left - i_count ) >= 0 )
        {
            /* more in the buffer than requested */
            i_result |= ( *s->pu8P >> i_shr )&dwstdh264MaskArr[i_count];
            s->s32Left -= i_count;
            if( s->s32Left == 0 )
            {
                s->pu8P++;
                s->s32Left = 8;
            }
            return( i_result );
        }
        else
        {
            /* less in the buffer than requested */
           i_result |= (*s->pu8P&dwstdh264MaskArr[s->s32Left]) << -i_shr;
           i_count  -= s->s32Left;
           s->pu8P++;
           s->s32Left = 8;
        }
    }

    return( i_result );
}

static u32 stdh265_bs_read1( TParserBitStream *s )
{
	u32 i_result;

    if( s->pu8P < s->pu8End )
    {        
        s->s32Left--;
        i_result = ( *s->pu8P >> s->s32Left )&0x01;
        if( s->s32Left == 0 )
        {
            s->pu8P++;
            s->s32Left = 8;
        }
        return i_result;
    }

    return 0;
}

static u32 stdh265_bs_show( TParserBitStream *s, s32 i_count )
{
	u32 i_cache = 0;

    if( s->pu8P < s->pu8End && i_count > 0 )
    {
        i_cache = ((s->pu8P[0] << 24)+(s->pu8P[1] << 16)+(s->pu8P[2] << 8)+s->pu8P[3]) << (8-s->s32Left);
        return( i_cache >> ( 32 - i_count) );
    }
    return 0;
}

/* TODO optimize */
static void stdh265_bs_skip( TParserBitStream *s, s32 i_count )
{
    s->s32Left -= i_count;

    while( s->s32Left <= 0 )
    {
        s->pu8P++;
        s->s32Left += 8;
    }
}

/*====================================================================
函数名      ：  H265DecBitstreamGetBits
功能        ：  读取一定数量bit
算法实现    ：  （可选项）
引用全局变量：  无
输入参数说明：  ptBs：熵解码结构指针[in]
                l32NBits：待跳过的bit数目[in]
返回值说明  ：  bit所表示的数值
====================================================================*/
static u32 H265DecBitstreamGetBits(TParserBitStream *ptBs, s32 l32NBits)
{
    u32 u32Tmp;

    u32Tmp = stdh265_bs_show(ptBs, l32NBits);

    stdh265_bs_skip(ptBs, l32NBits);

    return u32Tmp;
}


static s32 stdh265_bs_read_ue( TParserBitStream *s )
{
    s32 i = 0;

    while( stdh265_bs_read1( s ) == 0 && s->pu8P < s->pu8End && i < 32 )
    {
        i++;
    }
    return( ( 1 << i) - 1 + stdh265_bs_read( s, i ) );
}

static s32 stdh265_bs_read_se( TParserBitStream *s )
{
    s32 val = stdh265_bs_read_ue( s );

    return val&0x01 ? (val+1)/2 : -(val/2);
}

static s32 stdh265_bs_read_te( TParserBitStream *s, s32 x )
{
    if( x == 1 )
    {
        return 1 - stdh265_bs_read1( s );
    }
    else if( x > 1 )
    {
        return stdh265_bs_read_ue( s );
    }
    return 0;
}

static s32 stdh265_FirstPartOfSliceHeader(TParserBitStream *s, Tstdh265Dec_SliceHeaderData *dec_slice_header, TFrameInfo *ptVideoFrameInfo, s32 l32NalType)
{
    u32 first_slice_segment_in_pic_flag;
    s32 BitsSliceSegmentAddress = 0;
    s32 tmp;
    u32 WidthInCU;
    u32 HeightInCU;
    u32 NumOfLcuInFrame;

    if(ptVideoFrameInfo->dwMaxPartionWidth == 0)
    {
        return 0;
    }

    WidthInCU = (ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth % ptVideoFrameInfo->dwMaxPartionWidth) ? (ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth / ptVideoFrameInfo->dwMaxPartionWidth + 1) : (ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth / ptVideoFrameInfo->dwMaxPartionWidth);
    HeightInCU = (ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight % ptVideoFrameInfo->dwMaxPartionWidth) ? (ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight / ptVideoFrameInfo->dwMaxPartionWidth + 1) : (ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight / ptVideoFrameInfo->dwMaxPartionWidth);
    NumOfLcuInFrame = WidthInCU * HeightInCU;

    //跳过first_slice_segment_in_pic_flag
    first_slice_segment_in_pic_flag = H265DecBitstreamGetBits(s, 1);

    if(l32NalType >=  16 && l32NalType <= 23)
    {
        //读取no_output_of_prior_pics_flag,暂不支持非零情况，todo
        tmp = H265DecBitstreamGetBits(s, 1);
    }

    //slice_pic_parameter_set_id
    dec_slice_header->pic_parameters_set_id = stdh265_bs_read_ue( s );

    if(!first_slice_segment_in_pic_flag)
    {
        //calculate number of bits required for slice address
        while(NumOfLcuInFrame > ((u32)(1 << BitsSliceSegmentAddress))) 
        {
            BitsSliceSegmentAddress++;
        }
        //读取slice_segment_address
        dec_slice_header->first_mb_in_slice = (u16)H265DecBitstreamGetBits(s, BitsSliceSegmentAddress);
    }
    else
    {
        dec_slice_header->first_mb_in_slice = 0;
    }

    //num_extra_slice_header_bits等于0，直接读取slice_type
    dec_slice_header->slice_type = stdh265_bs_read_ue(s);

    return 0;
}


/*====================================================================
函数名      ：  DecodePTL
功能        ：  解析PTL信息
算法实现    ：  （可选项）
引用全局变量：  无
输入参数说明：  ptBs：指向解码码流指针[in]
                u8MaxSubLayersMinus1：层数[in]
返回值说明  ：  成功返回：VIDEO_SUCCESS，错误返回相应错误码
====================================================================*/
static BOOL32 DecodePTL(TParserBitStream *ptBs, u8 u8MaxSubLayersMinus1)
{
    s32 l32Index;
    u8 au8SubLayerProfilePresentFlag[6];
    u8 au8SubLayerLevelPresentFlag[6];

    stdh265_bs_skip(ptBs, 8); //读取general_profile_space，general_tier_flag，general_profile_idc

    stdh265_bs_skip(ptBs, 32); //读取general_profile_compatibility_flag[ i ]

    //读取剩下的语法元素general_progressive_source_flag至general_level_idc
    stdh265_bs_skip(ptBs, 32);
    stdh265_bs_skip(ptBs, 24);

    for(l32Index = 0; l32Index < u8MaxSubLayersMinus1; l32Index++)
    {
        au8SubLayerProfilePresentFlag[l32Index] = (u8)H265DecBitstreamGetBits(ptBs, 1); //读取sub_layer_profile_present_flag[ i ]
        au8SubLayerLevelPresentFlag[l32Index] = (u8)H265DecBitstreamGetBits(ptBs, 1);  //读取sub_layer_level_present_flag[ i ]
    }
    if(u8MaxSubLayersMinus1 > 0)
    {
        for(l32Index = u8MaxSubLayersMinus1; l32Index < 8; l32Index++)
        {
            stdh265_bs_skip(ptBs, 2);  //读取reserved_zero_2bits[ i ]
        }
    }
    for(l32Index = 0; l32Index < u8MaxSubLayersMinus1; l32Index++)
    {
        if(au8SubLayerProfilePresentFlag[l32Index])
        {
            stdh265_bs_skip(ptBs, 8);  //读取sub_layer_profile_space[ i ]、sub_layer_tier_flag[ i ]和sub_layer_profile_idc[ i ]
            stdh265_bs_skip(ptBs, 32);  //读取sub_layer_profile_compatibility_flag[ i ][ j ]
            stdh265_bs_skip(ptBs, 48);  //读取sub_layer_progressive_source_flag[ i ]至sub_layer_reserved_zero_44bits[i ]
        }
        if(au8SubLayerLevelPresentFlag[l32Index])
        {
            stdh265_bs_skip(ptBs, 8);  //读取sub_layer_level_idc[ i ]
        }
    }

    return 0;
}

/*=============================================================================
函数名        ：DecodeH265SPS
功能        ：解析 h.265 码流中的 pps 信息
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：

    返回值说明： TRUE - 成功
=============================================================================*/
static BOOL32 DecodeH265SPS( TParserBitStream *s, TSPS *ptSps, TFrameInfo *ptVideoFrameInfo)
{
    s32 l32RetCode = 0;
    //u8 u8SpsId = 0;
    s32 l32Val = 0;
    s32 l32Ret = 0;
    //u32 u32AddCUDepth = 0;
    u8 u8MaxSubLayersMinus1 = 0;
    s32 l32Index = 0;

    ptSps->u8VpsId = (u8)H265DecBitstreamGetBits(s, 4); //提取sps_video_parameter_set_id

    u8MaxSubLayersMinus1 = (u8)H265DecBitstreamGetBits(s, 3); //提取sps_max_sub_layers_minus1,todo

    ptSps->u8SpsTemporalIdNestingFlag = (u8)H265DecBitstreamGetBits(s, 1); //提取sps_temporal_id_nesting_flag

    //解析PTL语法结构
    l32Ret = DecodePTL(s, u8MaxSubLayersMinus1);

    //读取sps_seq_parameter_set_id
    l32Val = stdh265_bs_read_ue(s);
    ptSps->u8SpsId = l32Val;

    //读取chroma_format_idc
    l32Val = stdh265_bs_read_ue(s);
    ptSps->u8ChromaFormatIdc = l32Val;

    //读取pic_width_in_luma_samples和pic_height_in_luma_samples
    ptSps->u16Width = stdh265_bs_read_ue(s);
    ptSps->u16Height = stdh265_bs_read_ue(s);

    //读取conformance_window_flag
    ptSps->l32ConformanceWindow_flag = H265DecBitstreamGetBits(s, 1);
    if(ptSps->l32ConformanceWindow_flag)
    {
        ptSps->l32ConfWinLeftOffset = stdh265_bs_read_ue(s);
        ptSps->l32ConfWinRightOffset = stdh265_bs_read_ue(s);
        ptSps->l32ConfWinTopOffset = stdh265_bs_read_ue(s);
        ptSps->l32ConfWinBottomOffset = stdh265_bs_read_ue(s);

        ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = ptSps->u16Width - (ptSps->l32ConfWinLeftOffset + ptSps->l32ConfWinRightOffset) * 2;
        ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = ptSps->u16Height - (ptSps->l32ConfWinTopOffset + ptSps->l32ConfWinBottomOffset) * 2;
    }
    else
    {
        ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = ptSps->u16Width;
        ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = ptSps->u16Height;
    }

    //读取bit_depth_luma_minus8和bit_depth_chroma_minus8
    ptSps->u8BitDepthLumaMinus8 = stdh265_bs_read_ue(s);
    ptSps->u8BitDepthChromaMinus8 = stdh265_bs_read_ue(s);

    //读取log2_max_pic_order_cnt_lsb_minus4
    ptSps->u8Log2MaxPicOrderCntLsbMinus4 = stdh265_bs_read_ue(s);

    //读取sps_sub_layer_ordering_info_present_flag
    l32Val = H265DecBitstreamGetBits(s, 1);

    l32Index = (l32Val ?  0 : u8MaxSubLayersMinus1);
    for(; l32Index <= u8MaxSubLayersMinus1; l32Index++)
    {
        //读取sps_max_dec_pic_buffering_minus1[i]
        ptSps->au8SpsMaxDecPicBufferingMinus1[l32Index] = stdh265_bs_read_ue(s);

        //读取sps_max_num_reorder_pics[i]
        ptSps->au8SpsMaxNumReorderPics[l32Index] = stdh265_bs_read_ue(s);

        //读取sps_max_latency_increase_plus1[i]
        ptSps->au8SpsMaxLatencyIncreasePlus1[l32Index] = stdh265_bs_read_ue(s);
    }
    if(!l32Val)
    {
        for(l32Index = 0; l32Index < u8MaxSubLayersMinus1; l32Index++)
        {
            ptSps->au8SpsMaxDecPicBufferingMinus1[l32Index] = ptSps->au8SpsMaxDecPicBufferingMinus1[u8MaxSubLayersMinus1];
            ptSps->au8SpsMaxNumReorderPics[l32Index] = ptSps->au8SpsMaxNumReorderPics[u8MaxSubLayersMinus1];
            ptSps->au8SpsMaxLatencyIncreasePlus1[l32Index] = ptSps->au8SpsMaxLatencyIncreasePlus1[u8MaxSubLayersMinus1];
        }
    }

    //读取log2_min_coding_block_size_minus3
    ptSps->u8Log2MinCodingBlockSizeMinus3 = stdh265_bs_read_ue(s);

    //读取log2_diff_max_min_coding_block_size
    ptSps->u8Log2DiffMaxMinLumaCodingBlockSize = stdh265_bs_read_ue(s);

    ptSps->u32MaxPartionWidth = (1 << (ptSps->u8Log2MinCodingBlockSizeMinus3 + 3 + ptSps->u8Log2DiffMaxMinLumaCodingBlockSize));

    ptSps->u8Valid = TRUE;
    ptVideoFrameInfo->dwMaxPartionWidth = ptSps->u32MaxPartionWidth;
    ptVideoFrameInfo->dwSPSId = ptSps->u8SpsId;
    //ptVideoFrameInfo->m_bIsValidSPS = ptSps->u8Valid;

    return l32RetCode;
}

/*=============================================================================
函数名        ：DecodeH265PPS
功能        ：解析 h.265 码流中的 pps 信息
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：

    返回值说明： TRUE - 成功
=============================================================================*/
static BOOL32 DecodeH265PPS( TParserBitStream *s, TPPS *ptPps, TFrameInfo *ptVideoFrameInfo)
{
    //s32 l32RetCode = 0;
    s32 l32Val;

    //读取pic_parameter_set_id
    ptPps->u8PpsId = stdh265_bs_read_ue(s);

    //读取pps_seq_parameter_set_id
    ptPps->u8SpsId = stdh265_bs_read_ue(s);

    ptPps->u8dDependentSliceSegmentsEnabledFlag = (u8)H265DecBitstreamGetBits(s, 1);//提取dependent_slice_segments_enabled_flag，目前暂不支持，todo

    ptPps->u8OutputFlagPresentFlag = (u8)H265DecBitstreamGetBits(s, 1);//提取output_flag_present_flag，目前暂不支持，todo

    //读取num_extra_slice_header_bits todo
    l32Val = H265DecBitstreamGetBits(s, 3);

    //读取sign_data_hiding_flag
    ptPps->u8SignDataHidingFlag = (u8)H265DecBitstreamGetBits(s, 1);

    ptPps->u8CabacInitPresentFlag = (u8)H265DecBitstreamGetBits(s, 1);  //提取cabac_init_present_flag

    //读取num_ref_idx_l0_default_active_minus1和num_ref_idx_l1_default_active_minus1
    ptPps->u8NumRefIdxL0DefaultActiveMinus1 = stdh265_bs_read_ue(s);

    ptPps->u8NumRefIdxL1DefaultActiveMinus1 = stdh265_bs_read_ue(s);

    //读取pic_init_qp_minus26
    ptPps->s8PicInitQpMinus26 = stdh265_bs_read_se(s);

    //读取constrained_intra_pred_flag，transform_skip_enabled_flag，cu_qp_delta_enabled_flag
    l32Val = H265DecBitstreamGetBits(s, 3);
    ptPps->l32ConstrainedIntrPredFlag = ((l32Val >> 2) & 1);
    ptPps->l32Transform_SkipEnabled_Flag = ((l32Val >> 1) & 1);
    ptPps->l32CUQpDeltaEnabledFlag = (l32Val & 1);
    if(ptPps->l32CUQpDeltaEnabledFlag)
    {
        ptPps->u8DiffCUQpDeltaDepth = stdh265_bs_read_ue(s);  //读取diff_cu_qp_delta_depth
    }

    //读取pps_cb_qp_offset和pps_cr_qp_offset，暂不支持非零值，todo
    l32Val = stdh265_bs_read_se(s);
    l32Val = stdh265_bs_read_se(s);

    //读取pps_slice_chroma_qp_offsets_present_flag至entropy_coding_sync_enabled_flag
    l32Val = H265DecBitstreamGetBits(s, 6);
    //暂不支持pps_slice_chroma_qp_offsets_present_flag至transquant_bypass_enabled_flag,todo
    ptPps->u8TransquantBypassEnabledFlag = ((l32Val >> 2) & 1);//提取transquant_bypass_enabled_flag
    ptPps->l32TilesEnabledFlag = ((l32Val >> 1) & 1);  //提取tiles_enabled_flag
    //暂不支持entropy_coding_sync_enabled_flag,todo
    ptPps->u8EntropyCodingSyncEnabledFlag = (l32Val & 1);

    if(ptPps->l32TilesEnabledFlag)
    {
        //读取num_tile_columns_minus1和num_tile_rows_minus1
        ptPps->l32NumTileColumnsMinus1 = stdh265_bs_read_ue(s);
        ptPps->l32NumTileRowsMinus1 = stdh265_bs_read_ue(s);

        //读取uniform_spacing_flag  //暂不支持自定义tile宽高，todo
        ptPps->u8UniformSpacingFlag = (u8)H265DecBitstreamGetBits(s, 1);

        //读取loop_filter_across_tiles_enabled_flag
        ptPps->l32LoopFilterAcrossTilesEnabledFlag = H265DecBitstreamGetBits(s, 1);
    }
    else
    {
        ptPps->u8UniformSpacingFlag = 1;
        ptPps->l32LoopFilterAcrossTilesEnabledFlag = 1;
    }

    ptPps->l32LoopFilterAcrossSlicesEnabledFlag = H265DecBitstreamGetBits(s, 1);  //读取loop_filter_across_slices_enabled_flag

    ptPps->u8DeblockingFilterControlPresentFlag = (u8)H265DecBitstreamGetBits(s, 1);  //读取deblocking_filter_control_present_flag

    if(ptPps->u8DeblockingFilterControlPresentFlag)
    {
        ptPps->u8DeblockingFilterOverrideEnabledFlag = (u8)H265DecBitstreamGetBits(s, 1); //读取deblocking_filter_override_enabled_flag
        //暂不支持deblocking_filter_override_enabled_flag不等于1,todo

        ptPps->l32DeblockingFilterDisableFlag = H265DecBitstreamGetBits(s, 1); //读取pps_deblocking_filter_disabled_flag ,todo

        if(!ptPps->l32DeblockingFilterDisableFlag)
        {
            //暂不支持pps_beta_offset_div2和pps_tc_offset_div2
            ptPps->s8PPSBetaOffsetDiv2 = stdh265_bs_read_se(s);
            ptPps->s8PPSTcOffsetDiv2 = stdh265_bs_read_se(s);
        }
    }

    //读取pps_scaling_list_data_present_flag
    l32Val = H265DecBitstreamGetBits(s, 1);

    //读取lists_modification_present_flag
    ptPps->u8ListsModificationPresentFlag = (u8)H265DecBitstreamGetBits(s, 1);

    //读取log2_parallel_merge_level_minus2
    ptPps->u8Log2ParallelMergeLevelMinus2 = stdh265_bs_read_ue(s);

    //读取slice_segment_header_extension_present_flag和pps_extension_flag
    l32Val = H265DecBitstreamGetBits(s, 2);

    ptPps->u8Valid = TRUE;
    //ptVideoFrameInfo->m_bIsValidPPS = TRUE;

    return 0;
}

static BOOL32 ParseH265(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	BOOL32 bRet = TRUE;
	u8  byNaluTypeOctet = 0;
	u32 dwNaluType = 0;
	TParserBitStream tBitStream;
	TSPS tSPS;
    TPPS tPPS;
    Tstdh265Dec_SliceHeaderData tSlice_header;

	if (nSize <= 1 || NULL == ptVideoFrameInfo)
    {
        KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[ParseH265] nalu length too small\n");
        return FALSE;
    }

	//clear video frame information
	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = FALSE;
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = 0;
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = 0;
		//memset(&(ptVideoFrameInfo->m_tVideoParam), 0, sizeof(ptVideoFrameInfo->m_tVideoParam));
	}
	    
    //+---------------+---------------+
    //|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //|F|   Type    |  LayerId  | TID |
    //+-------------+-----------------+
    byNaluTypeOctet = (u8) (*pBuf);
	
    dwNaluType = (byNaluTypeOctet & 0x7E)>>1;  // 取得中间6位
    
    memset(&tBitStream, 0, sizeof(tBitStream));
	
    if (49 == dwNaluType)//碎片包
    {
        if ((pBuf[2] & 0x80) > 0)//第一包
        {
            // 如果是第一个碎片包，要向后偏移两个字节
            dwNaluType = pBuf[2] & 0x3F;
            stdh265_bs_init(&tBitStream, (pBuf+3), (nSize-3));
        }
        else
        {
            // 不是第一包，什么都不做
            return FALSE;
        }
    }
    else
    {
        // TBitStream结构从RTP包负载数据的第3字节开始（前2字节为Nalu头）
        stdh265_bs_init(&tBitStream, (pBuf+2), (nSize-2));
    }
	
    memset(&tSPS, 0, sizeof(tSPS));
    memset(&tPPS, 0, sizeof(tPPS));
    memset(&tSlice_header, 0, sizeof(tSlice_header));
	
    switch(dwNaluType)
    {
    case 1:
    case 19:  // NALU_TYPE_IDR
        stdh265_FirstPartOfSliceHeader(&tBitStream, &tSlice_header, ptVideoFrameInfo, dwNaluType);
        if(H264_I_SLICE == tSlice_header.slice_type)
        {
            ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame= TRUE;
        }
        //KDVSTREAMPARSERPRINTF( 1, 0, "[CKdvNetRcv::ParseH264Head] NALU_TYPE_IDR ...........\n",  );
        break;
    case 33:  // NALU_TYPE_SPS
        bRet = DecodeH265SPS(&tBitStream, &tSPS, ptVideoFrameInfo);
        break;
    case 34:  // NALU_TYPE_PPS
        bRet = DecodeH265PPS(&tBitStream, &tPPS, ptVideoFrameInfo);
        break;
    default:
        break;
    }
	
	if(bRet)
	{
		ptVideoFrameInfo->m_byVideoType = MEDIA_TYPE_H265;
	}
	
	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*=============================================================================
	函数集		：stdSVAC_bs_init ....
	功能		：SVAC 码流信息分析
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               无           

	返回值说明： 无
=============================================================================*/
static void stdSVAC_bs_init( TParserBitStream *s, void *p_data, s32 i_data )
{
    u32 u32Next24bit = 0;
	u8 *pu8TempData = NULL, *pu8CurData = (u8 *)p_data, *pu8Head = NULL;
	u8 u8Code = 0;
    s32 s32Len = i_data < 30 ? i_data : 30;
	s32 s32Len1 = s32Len;
	pu8Head = s->au8HeadInfo;

	s->pu8Start = pu8Head;
    s->pu8P     = pu8Head;
    s->pu8End   = (u8*) s->pu8P + s32Len;
    s->s32Left  = 8;
	u32Next24bit = 0xFFFFFFFF;
    memcpy(pu8Head, (u8 *)p_data, i_data < 30 ? i_data : 30);
    pu8TempData = pu8Head;
	do
	{
		u8Code = *pu8CurData++;
		u32Next24bit = ((u32Next24bit << 8) | u8Code) & 0xFFFFFF;
		
		if(u32Next24bit != 0x000003 &&
		   u32Next24bit != 0x000001 &&
		   u32Next24bit != 0x000000)
		{
			//复制-除了竞争码
			*pu8TempData++ = u8Code;
		}
        else
        {
            s32Len -= 1;
        }
	}while((u32Next24bit != 0x000001) && (u32Next24bit != 0x000000) && (pu8CurData < ((u8 *)p_data + s32Len1)));
	s->pu8End = s->pu8Start + s32Len;
}

static s32 stdSVAC_bs_pos( TParserBitStream *s )
{
    return( 8 * ( s->pu8P - s->pu8Start ) + 8 - s->s32Left );
}

static s32 stdSVAC_bs_eof( TParserBitStream *s )
{
    return( s->pu8P >= s->pu8End ? 1: 0 );
}

static u32 stdSVAC_bs_read( TParserBitStream *s, s32 i_count )
{
	static u32 dwstdSVACMaskArr[33] = 
	{ 
		0x00,
        0x01,      0x03,      0x07,      0x0f,
        0x1f,      0x3f,      0x7f,      0xff,
        0x1ff,     0x3ff,     0x7ff,     0xfff,
        0x1fff,    0x3fff,    0x7fff,    0xffff,
        0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
        0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
        0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
        0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff };
		
    s32 i_shr = 0;
    u32 i_result = 0;

    while( i_count > 0 )
    {
        if( s->pu8P >= s->pu8End )
        {
            break;
        }

        if( ( i_shr = s->s32Left - i_count ) >= 0 )
        {
            /* more in the buffer than requested */
            i_result |= ( *s->pu8P >> i_shr )&dwstdSVACMaskArr[i_count];
            s->s32Left -= i_count;
            if( s->s32Left == 0 )
            {
                s->pu8P++;
                s->s32Left = 8;
            }
            return( i_result );
        }
        else
        {
            /* less in the buffer than requested */
           i_result |= (*s->pu8P&dwstdSVACMaskArr[s->s32Left]) << -i_shr;
           i_count  -= s->s32Left;
           s->pu8P++;
           s->s32Left = 8;
        }
    }

    return( i_result );
}

static u32 stdSVAC_bs_read1( TParserBitStream *s )
{
    if( s->pu8P < s->pu8End )
    {
        u32 i_result;

        s->s32Left--;
        i_result = ( *s->pu8P >> s->s32Left )&0x01;
        if( s->s32Left == 0 )
        {
            s->pu8P++;
            s->s32Left = 8;
        }
        return i_result;
    }

    return 0;
}

static u32 stdSVAC_bs_show( TParserBitStream *s, s32 i_count )
{
	u32 i_cache  = 0;

    if( s->pu8P < s->pu8End && i_count > 0 )
    {
        i_cache = ((s->pu8P[0] << 24)+(s->pu8P[1] << 16)+(s->pu8P[2] << 8)+s->pu8P[3]) << (8-s->s32Left);
        return( i_cache >> ( 32 - i_count) );
    }
    return 0;
}

/* TODO optimize */
static void stdSVAC_bs_skip( TParserBitStream *s, s32 i_count )
{
    s->s32Left -= i_count;

    while( s->s32Left <= 0 )
    {
        s->pu8P++;
        s->s32Left += 8;
    }
}

static s32 stdSVAC_bs_read_ue( TParserBitStream *s )
{
    s32 i = 0;

    while( stdSVAC_bs_read1( s ) == 0 && s->pu8P < s->pu8End && i < 32 )
    {
        i++;
    }
    return( ( 1 << i) - 1 + stdSVAC_bs_read( s, i ) );
}

static s32 stdSVAC_bs_read_se( TParserBitStream *s )
{
    s32 val = stdSVAC_bs_read_ue( s );

    return val&0x01 ? (val+1)/2 : -(val/2);
}


static s32 stdSVAC_FirstPartOfSliceHeader(TParserBitStream *s, TstdSVACDec_SliceHeaderData *dec_slice_header, TFrameInfo *ptSVACHeader, s32 l32NalType)
{
    //pic_parameter_set_id
    dec_slice_header->pic_parameters_set_id = stdSVAC_bs_read_ue(s);
    //frame_num
    dec_slice_header->frame_num = (u16)stdSVAC_bs_read(s, 8);
    
    //idr_pic_id
    if(l32NalType==2 || l32NalType == 4)
    {
        dec_slice_header->idr_pic_id = stdSVAC_bs_read_ue(s);
    }
    
    //progressive_seq_flag
    if(!ptSVACHeader->m_tVideoParam.m_bIsFrameFlag)
    {
        //field_pic_flag
        dec_slice_header->field_pic_flag = (u16)stdSVAC_bs_read(s, 1);
        if(dec_slice_header->field_pic_flag)
        {
            dec_slice_header->bottom_field_flag = (u16)stdSVAC_bs_read(s, 1);
        }
    }
    
    //first_mb_in_slice
    dec_slice_header->first_mb_in_slice = stdSVAC_bs_read_ue(s);
    //slice_type
    dec_slice_header->slice_type = stdSVAC_bs_read_ue(s);
	return 0;
}

/*=============================================================================
函数名		：DecodeSVACSPS
功能		：解析 SVAC 码流中的 sps 信息
算法实现	：（可选项）
引用全局变量：无
输入参数说明：

  返回值说明： TRUE - 成功
=============================================================================*/
static BOOL32 DecodeSVACSPS( TParserBitStream *s, TSVACSPS *sps, TFrameInfo*ptVideoFrameInfo )
{
	s32 chroma_format_idc = 0; 
	u32 sps_pic_height = 0;
    s32 s32Val = 0;
	
	sps->u8ProFile                 = (u8)stdSVAC_bs_read( s, 8 );	
	s32Val = stdSVAC_bs_read( s, 8 );
	
	sps->u8SpsID                   = stdSVAC_bs_read_ue( s );
    chroma_format_idc              = stdSVAC_bs_read( s, 2 );	         //chroma_format_idc 
    s32Val = stdSVAC_bs_read_ue( s );//bit_depth_luma_minus8 
	s32Val = stdSVAC_bs_read_ue( s );//bit_depth_chroma_minus8 
	sps->u8PicWidthInMbs           = stdSVAC_bs_read_ue( s );  //  Width
	sps->u8PicHeightInMbs          = stdSVAC_bs_read_ue( s );  //  Height
	sps->u8ProgessiveSeqFlag       = (u8)stdSVAC_bs_read( s, 1 );
	sps_pic_height = (2 - sps->u8ProgessiveSeqFlag) * (sps->u8PicHeightInMbs + 1) * 16;
    ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth       = (u16)(sps->u8PicWidthInMbs + 1) * 16; 
    ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight      = (u16)sps_pic_height;
    
    sps->u8RoiFlag                 = (u8)stdSVAC_bs_read( s, 1 );
    sps->u8SvcFlag                 = (u8)stdSVAC_bs_read( s, 1 );
	
	s32Val = stdSVAC_bs_read( s, 1 ); 
	
	sps->u8Valid = TRUE; 
        ptVideoFrameInfo->dwSPSId = sps->u8SpsID;
        ptVideoFrameInfo->m_tVideoParam.m_bIsFrameFlag = sps->u8ProgessiveSeqFlag;

	return TRUE;
}

/*=============================================================================
函数名		：DecodeSVACPPS
功能		：解析 SVAC 码流中的 pps 信息
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
				   
	返回值说明： TRUE - 成功
=============================================================================*/
static BOOL32 DecodeSVACPPS( TParserBitStream *s, TSVACPPS *pps, TFrameInfo *ptVideoFrameInfo )
{
    s32 s32Idx = 0;
	if (s->pu8P == NULL || s->pu8Start == NULL)
	{
		return FALSE;
	}
	
	pps->u8PpsID     = stdSVAC_bs_read_ue( s );
	pps->u8SpsID     = stdSVAC_bs_read_ue( s );
	pps->u8EntropyCodingModeFlag = (u8)stdSVAC_bs_read( s, 1 );

	pps->u8PicQP     = (u8)stdSVAC_bs_read( s, 6 );
    
//    if(pStdSVACHeader->m_bIsRoiFlag)
//    {
        pps->u8NumRoi = stdSVAC_bs_read_ue( s );
        if(pps->u8NumRoi > 0)
        {
            pps->u8NonRoiSkipFlag = (u8)stdSVAC_bs_read( s, 1 );
            if((!pps->u8NonRoiSkipFlag) )
            {
                pps->u8ScalableNonRoiSkipFlag = (u8)stdSVAC_bs_read( s, 1 );
            }
            pps->u8PicQPForRoi = (u8)stdSVAC_bs_read( s, 6 );
            for(s32Idx = 0; s32Idx < pps->u8NumRoi; s32Idx++)
            {
                pps->au32TopLeft[s32Idx] = stdSVAC_bs_read_ue( s );
                pps->au32BottomRight[s32Idx] = stdSVAC_bs_read_ue( s );
            }
        }
//    }
    
    pps->u8FixPicQp = (u8)stdSVAC_bs_read( s, 1 );
    //weighting_pred_flag
    pps->u8WeightedPredFlag = (u8)stdSVAC_bs_read( s, 1 );
    //loop_filter_disable_flag
    pps->u8LoopFilterDisableFlag = (u8)stdSVAC_bs_read( s, 1 );

	pps->bIsValid = TRUE;
//	pStdSVACHeader->m_bIsValidPPS = TRUE;

	return TRUE;
}

/*=============================================================================
函数名		：DecodeSVAC2SPS
功能		：解析 SVAC 码流中的 sps 信息
算法实现	：（可选项）
引用全局变量：无
输入参数说明：

  返回值说明： TRUE - 成功
=============================================================================*/
static BOOL32 DecodeSVAC2SPS( TParserBitStream *s, TSVACSPS *sps, TFrameInfo*ptVideoFrameInfo )
{

    s32 l32Val, l32CropUnitX, l32CropUnitY;
    s32 l32Profile, l32CheckPara;
    s32 l32Index;
	s32 l32Framerate = 0;

    //profile_id level_id
    l32Val = stdSVAC_bs_read(s, 16);
    sps->u8ProFile = l32Val >> 8;

    //ldp_mode_flag
    l32Val = stdSVAC_bs_read(s, 1);

	//width
    l32Val = stdSVAC_bs_read(s, 16) + 1;
	ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = (s16)l32Val;

	//height
	l32Val = stdSVAC_bs_read(s, 16) + 1;
	ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = (s16)l32Val;

    //chroma_format_idc
    l32Val = stdSVAC_bs_read(s, 2);

    //bit_depth_luma_minus8
    l32Val = stdSVAC_bs_read(s, 2);
 
    //max_ref_num
    l32Val = stdSVAC_bs_read(s, 3);

    //framerate
    l32Framerate= stdSVAC_bs_read(s, 3);

    //extend_sb_size
    l32Val = stdSVAC_bs_read(s, 1);;
 
    //tile_enable
    l32Val = stdSVAC_bs_read(s, 1);

	//wpp_enable
    l32Val = stdSVAC_bs_read(s, 1);
    //sao_enable
    l32Val = stdSVAC_bs_read(s, 1);

    //alf_flag
    l32Val = stdSVAC_bs_read(s, 1);

    //roi
    l32Val = stdSVAC_bs_read(s, 1);

    //temproal_svc_flag
    l32Val = stdSVAC_bs_read(s, 1);
  	if(l32Val == 1)
	{
		l32Val = stdSVAC_bs_read(s, 2) + 1;
	}	

	//spatial_svc_flag
    l32Val = stdSVAC_bs_read(s, 1);

    if(l32Val)
	{
		//svc_ratio
		l32Val = stdSVAC_bs_read(s, 1);
		
		//svc_mode
		l32Val = stdSVAC_bs_read(s, 1);
	}
    // vui_parameters_present_flag, 此信息很长, 在需要的情况下补充完整
    if(l32Framerate >= 4)
    {    	
		l32Val = stdSVAC_bs_read(s, 1);
		//CHECKSPS(l32Val == 1, ERR_VID_SVACDEC_VUI_PARAMETERS_PRESENT_FLAG);
    }

    return TRUE;
}

static BOOL32 ParseSVACV1(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	BOOL32 bRet = TRUE;
	u8  byNaluTypeOctet = 0;
	u32 dwNaluType = 0;
	TParserBitStream tBitStream = {0};
	TSVACSPS tSPS = {0};
	TSVACPPS tPPS = {0};
	TstdSVACDec_SliceHeaderData tSlice_header = {0};

    if (nSize <= 1 || NULL == ptVideoFrameInfo)
	{
		return FALSE;
	}

    	//clear video frame information
	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = FALSE;
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = 0;
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = 0;
		//memset(&(ptVideoFrameInfo->m_tVideoParam), 0, sizeof(ptVideoFrameInfo->m_tVideoParam));
	}
    
	// the format of the NAL unit type octet is reprinted below:
	//  +---------------+
	//	|0|1|2|3|4|5|6|7|
	//	+-+-+-+-+-+-+-+-+
	//	|F|NRI|  Type   |
	//	+---------------+
	byNaluTypeOctet = (u8) (*pBuf);

	dwNaluType = (byNaluTypeOctet & 0x3c)>>2;  // 取得低5位
	
	memset(&tBitStream, 0, sizeof(tBitStream));

    // xp090224
    if (14 == dwNaluType)
    {
        if ((pBuf[1] & 0x80) > 0)
        {
            // 如果是第一个碎片包，要向后偏移一个字节
            dwNaluType = (pBuf[1] & 0x3c)>>2;
            stdSVAC_bs_init(&tBitStream, (pBuf+2), (nSize-2));
        }
        else
        {
            // 不是第一包，什么都不做 Modified by kangshouwei return value change from TRUE to FALSE
            return FALSE;
        }
    }
    else
    {
		// TBitStream结构从RTP包的第14字节开始（前12字节为RTP头，第13字节为Nalu类型）
		stdSVAC_bs_init(&tBitStream, (pBuf+1), (nSize-1));
    }
	
	memset(&tSPS, 0, sizeof(tSPS));
	memset(&tPPS, 0, sizeof(tPPS));
	memset(&tSlice_header, 0, sizeof(tSlice_header));
	
	switch(dwNaluType) 
	{
	case 1:
        case 2:  // NALU_TYPE_IDR
		stdSVAC_FirstPartOfSliceHeader(&tBitStream, &tSlice_header, ptVideoFrameInfo, dwNaluType);
		if( H264_I_SLICE == tSlice_header.slice_type)
		{
			ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = TRUE;
		}
//                OspPrintf( 1, 0, "[ParseSVACV] tSlice_header.slice_type ...........(%d)\n",tSlice_header.slice_type);
//		OspPrintf( 1, 0, "[ParseSVACV] NALU_TYPE_IDR ...........(%d)(%d:%d)\n",ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame, ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth, ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight);
		break;
	case 7:  // NALU_TYPE_SPS
		bRet = DecodeSVACSPS(&tBitStream, &tSPS, ptVideoFrameInfo);
//		OspPrintf( 1, 0, "[ParseSVACV] DecodeSVACVSPS ...........(%d)(%d:%d)\n",ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame, ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth, ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight);
		break;
//	case 8:  // NALU_TYPE_PPS
//		bRet = DecodeSVACPPS(&tBitStream, &tPPS, ptVideoFrameInfo);
//		OspPrintf( 1, 0, "[ParseSVACV] DecodeSVACVPPS ...........\n" );
//		break;
	default:
		break;
	}

        if(bRet)
	{
		ptVideoFrameInfo->m_byVideoType = MEDIA_TYPE_SVACV;
	}

	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return bRet;
}

static BOOL32 ParseSVACV2(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	BOOL32 bRet = TRUE;
	u8  byNaluTypeOctet = 0;
	u32 dwNaluType = 0;
	TParserBitStream tBitStream = {0};
	TSVACSPS tSPS = {0};
	TSVACPPS tPPS = {0};
	TstdSVACDec_SliceHeaderData tSlice_header = {0};

    if (nSize <= 1 || NULL == ptVideoFrameInfo)
	{
		return FALSE;
	}

    	//clear video frame information
	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = FALSE;
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth = 0;
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight = 0;
		//memset(&(ptVideoFrameInfo->m_tVideoParam), 0, sizeof(ptVideoFrameInfo->m_tVideoParam));
	}
    
	// the format of the NAL unit type octet is reprinted below:
	//  +---------------+
	//	|0|1|2|3|4|5|6|7|
	//	+-+-+-+-+-+-+-+-+
	//	|F|NRI|  Type   |
	//	+---------------+
	byNaluTypeOctet = (u8) (*pBuf);

	dwNaluType = (byNaluTypeOctet & 0x3c)>>2;  // 取得低5位
	
	memset(&tBitStream, 0, sizeof(tBitStream));

    // xp090224
    if (14 == dwNaluType)
    {
        if ((pBuf[1] & 0x80) > 0)
        {
            // 如果是第一个碎片包，要向后偏移一个字节
            dwNaluType = (pBuf[1] & 0x3c)>>2;
            stdSVAC_bs_init(&tBitStream, (pBuf+2), (nSize-2));
        }
        else
        {
            // 不是第一包，什么都不做
            return FALSE;
        }
    }
    else
    {
		// TBitStream结构从RTP包的第14字节开始（前12字节为RTP头，第13字节为Nalu类型）
		stdSVAC_bs_init(&tBitStream, (pBuf+1), (nSize-1));
    }
	
	memset(&tSPS, 0, sizeof(tSPS));
	memset(&tPPS, 0, sizeof(tPPS));
	memset(&tSlice_header, 0, sizeof(tSlice_header));
	
	switch(dwNaluType) 
	{
	case 1:
		break;
    case 2:  // NALU_TYPE_IDR
		//stdSVAC_FirstPartOfSliceHeader(&tBitStream, &tSlice_header, ptVideoFrameInfo, dwNaluType);
		//if( H264_I_SLICE == tSlice_header.slice_type)
		//{
			ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = TRUE;
		//}
//                OspPrintf( 1, 0, "[ParseSVACV] tSlice_header.slice_type ...........(%d)\n",tSlice_header.slice_type);
//		OspPrintf( 1, 0, "[ParseSVACV] NALU_TYPE_IDR ...........(%d)(%d:%d)\n",ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame, ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth, ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight);
		break;
	case 7:  // NALU_TYPE_SPS
		bRet = DecodeSVAC2SPS(&tBitStream, &tSPS, ptVideoFrameInfo);
//		OspPrintf( 1, 0, "[ParseSVACV] DecodeSVACVSPS ...........(%d)(%d:%d)\n",ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame, ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth, ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight);
		break;
//	case 8:  // NALU_TYPE_PPS
//		bRet = DecodeSVACPPS(&tBitStream, &tPPS, ptVideoFrameInfo);
//		OspPrintf( 1, 0, "[ParseSVACV] DecodeSVACVPPS ...........\n" );
//		break;
	default:
		break;
	}
	
	if(bRet)
	{
		ptVideoFrameInfo->m_byVideoType = MEDIA_TYPE_SVACV;
	}

	if(ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
		ptVideoFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return bRet;
}

static BOOL32 ParseSVACV(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	BOOL32 l32Ret = TRUE;

    if (nSize <= 1 || NULL == ptVideoFrameInfo)
	{
		return FALSE;
	}
	
	if((pBuf[0] & 0x80) == 0x80)
	{
		
		l32Ret = ParseSVACV2(pBuf, nSize, ptVideoFrameInfo);
	}
	else
	{
		l32Ret = ParseSVACV1(pBuf, nSize, ptVideoFrameInfo);
	}
	
	return l32Ret;
}

////////////////////////////////////////getmp4info//////////////////////////////////////////////

/*=============================================================================
函 数 名: BSWAP
功    能: 字节反转
全局变量: 无
参    数: unsigned intVal          输入值
返 回 值: 反正后值
=============================================================================*/
static unsigned int BSWAP(unsigned int u32Val)
{
    u32Val = ((u32Val >> 24) | ((u32Val & 0xff0000) >> 8) | ((u32Val & 0xff00) << 8) | ((u32Val & 0xff) << 24));
    
    return u32Val;
}

/*=============================================================================
函 数 名: BsInit
功    能: 初始码流信息
全局变量: 无
参    数: ptBs          码流信息结构指针
          pu8BsBuf      输入码流Buffer
          unsigned intLength     码流长度
返 回 值: 无
=============================================================================*/
static void BsInit(TBitReader *const ptBs, u8 *pu8BsBuf, unsigned int u32Length)
{
    unsigned int *pu32Start = NULL;
    s32 l32PadLen;

//#ifdef WIN32
#if defined(_M_X64) || defined(__x86_64__)  || defined(_WIN64) || defined(WIN64)
	pu32Start = (unsigned int*)((unsigned long long)pu8BsBuf & (unsigned long long)~3);
	l32PadLen = (unsigned long long)pu8BsBuf - (unsigned long long)pu32Start;
#else
	//将码流以32bit对齐
	pu32Start = (unsigned int*)((unsigned long)pu8BsBuf & (unsigned long)~3);
	l32PadLen = (unsigned long)pu8BsBuf - (unsigned long)pu32Start;
#endif
//#else
//	pu32Start = (unsigned int*)((unsigned long)pu8BsBuf & (unsigned long)~3);
//	l32PadLen = (unsigned long)pu8BsBuf - (unsigned long)pu32Start;
//#endif

	

    ptBs->u32Buf = ptBs->u32Ptr = (unsigned int *)pu32Start;
    ptBs->u32Pos = l32PadLen * 8;
    ptBs->u32Length = u32Length;
    
    ptBs->u32Cur = BSWAP(*ptBs->u32Ptr ++);
    ptBs->u32Next = BSWAP(*ptBs->u32Ptr ++);
    ptBs->u32DecBsErr = 0;
}

/*=============================================================================
函 数 名: BsLength
功    能: 计算码流长度
全局变量: 无
参    数: ptBs          码流信息结构指针
返 回 值: 码流长度
=============================================================================*/
static unsigned int BsLength(TBitReader *const ptBs)
{
    return (((ptBs->u32Ptr - ptBs->u32Buf) << 2) - 4 - ((32 - ptBs->u32Pos) >> 3));
}

/*=============================================================================
函 数 名: BsSkip
功    能: 跳过nbit
全局变量: 无
参    数: ptBs          码流信息结构指针
          unsigned intBits       取出Bit数
返 回 值: 无
=============================================================================*/
static void BsSkip(TBitReader *const ptBs, const unsigned int u32Bits)
{
    unsigned int u32Consumed;
    
    ptBs->u32Pos += u32Bits;
    
    if(ptBs->u32Pos > 31) 
    {
        ptBs->u32Cur = ptBs->u32Next;
        
        ptBs->u32Next = BSWAP(*ptBs->u32Ptr++);
        ptBs->u32Pos -= 32;
        
        //计算消耗bit数
        u32Consumed = BsLength(ptBs);
        
        if(u32Consumed > ptBs->u32Length)
        {
            ptBs->u32DecBsErr = -1; //设置解码码流错误标记
        }     
    }
}

/*=============================================================================
函 数 名: BsByteAlign
功    能: 字节对齐
全局变量: 无
参    数: ptBs          码流信息结构指针
返 回 值: 无
=============================================================================*/
static void BsByteAlign(TBitReader *const ptBs)
{
    unsigned int u32Remained = ptBs->u32Pos & 7; 
    
    if(u32Remained)
    {
        BsSkip(ptBs, 8 - u32Remained);
    }
}

/*=============================================================================
函 数 名: BsShow
功    能: 取出nbit值(指针不前移)
全局变量: 无
参    数: ptBs          码流信息结构指针
          unsigned intBits       取出Bit数
返 回 值: nbit值
=============================================================================*/
static unsigned int BsShow(TBitReader *const ptBs, const unsigned int u32Bits)
{
    s32 nbit = (s32)(u32Bits + ptBs->u32Pos) - 32;
    
    if(nbit > 0) 
    {
        return ((ptBs->u32Cur & (0xffffffff >> ptBs->u32Pos)) << nbit) | (ptBs->u32Next >> (32 - nbit));
    }
    else 
    {
        return (ptBs->u32Cur & (0xffffffff >> ptBs->u32Pos)) >> (32 - ptBs->u32Pos - u32Bits);
    }
}

/*=============================================================================
函 数 名: BsGet
功    能: 取出nbit值(指针前移)
全局变量: 无
参    数: ptBs          码流信息结构指针
          unsigned intBits       取出Bit数
返 回 值: nbit值
=============================================================================*/
static unsigned int BsGet(TBitReader *const ptBs, const unsigned int u32Bits)
{
    unsigned int u32Val;
    
    u32Val = BsShow(ptBs, u32Bits);
    
    BsSkip(ptBs, u32Bits);
    
    return u32Val;
}

/*=============================================================================
函 数 名: Log2Bin
功    能: 求取Log2值
全局变量: 无
参    数: l32Value    预求Log2数值[in]
返 回 值: Log2值
=============================================================================*/
static s32 Log2Bin(s32 l32Value)
{
    s32 l32M = 0;

    while(l32Value)
    {
        l32Value >>= 1;

        l32M++;
    }

    return l32M;
}

/*=============================================================================
函 数 名: GetVolHeader
功    能: 解码VOL头信息
全局变量: 无
参    数: ptBs  码流信息结构指针[in/out]  
          pl32Width 图像宽度[out[
          pl32Width 图像高度[out]
返 回 值: VIDEO_SUCCESS 成功, 其他 失败
=============================================================================*/
static s32 GetVolHeader(TBitReader *ptBs, s32 *pl32Width, s32 *pl32Height)
{
    unsigned int u32VolVerid, u32Width = 0, u32Height = 0;
    unsigned int u32TimeIncBits;
    unsigned int u32VoTypeInd;
    u16 u16Shape;
    s32 l32TimeResolution;

    BsSkip(ptBs, 1); // random_accessible_vol

    u32VoTypeInd = BsShow(ptBs, 8); // video_object_type_indication
        
    if((u32VoTypeInd != VOL_TYPE_SIMPLE) && (u32VoTypeInd != VIDOBJLAY_TYPE_ASP) && (u32VoTypeInd != VOL_TYPE_DEFAULT)&& (u32VoTypeInd != VIDEOJLAY_TYPE_CORE_STUDIO))
    {
        return -1;
    }

    BsSkip(ptBs, 8);

    if(BsGet(ptBs, 1)) // is_object_layer_identifier
    {
        u32VolVerid = BsGet(ptBs,4); // video_object_layer_verid
        BsSkip(ptBs, 3); // video_object_layer_priority
    }
    else
    {
        u32VolVerid = 1;
    }

    if(BsGet(ptBs, 4) == VOL_AR_EXTPAR)	// aspect_ratio_info
    {
        BsSkip(ptBs, 8); // par_width
        BsSkip(ptBs, 8); // par_height
    }

    if(BsGet(ptBs, 1)) // vol_control_parameters
    {
        BsSkip(ptBs, 2); // chroma_format
        BsSkip(ptBs, 1); // low_delay

        if(BsGet(ptBs, 1)) // vbv_parameters
        {
            unsigned int u32FirstHalfBitRate, u32LatterHalfBitRate;
            unsigned int u32FirstHalfVbvBufSize, u32LatterHalfVbvBufSize;
            unsigned int u32FisrtsHalfVbvOccup, u32LatterHalfVbvOccup;

            u32FirstHalfBitRate = BsGet(ptBs, 15); // first_half_bitrate
            MARKER();
            u32LatterHalfBitRate = BsGet(ptBs, 15);	// latter_half_bitrate
            MARKER();
            u32FirstHalfVbvBufSize = BsGet(ptBs, 15); // first_half_vbv_buffer_size
            MARKER();
            u32LatterHalfVbvBufSize = BsGet(ptBs, 3); // latter_half_vbv_buffer_size
            u32FisrtsHalfVbvOccup = BsGet(ptBs, 11); // first_half_vbv_occupancy
            MARKER();
            u32LatterHalfVbvOccup = BsGet(ptBs, 15); // latter_half_vbv_occupancy
            MARKER();
        }
    }

    u16Shape = (u16)BsGet(ptBs, 2); // video_object_layer_shape

    if(u16Shape != VOL_SHAPE_RECTANGULAR)
    {
        return -1;
    }

    MARKER();

    l32TimeResolution = BsGet(ptBs, 16); // vop_time_increment_resolution

    if(l32TimeResolution > 1)
    {
        u32TimeIncBits = Log2Bin(l32TimeResolution - 1);
    }
    else
    {
        u32TimeIncBits = 1;
    }

    MARKER();

    if(BsGet(ptBs, 1)) // fixed_vop_rate
    {
        BsSkip(ptBs, u32TimeIncBits); // fixed_vop_time_increment
    }

    if(u16Shape == VOL_SHAPE_RECTANGULAR)
    {
        MARKER();
        u32Width = BsGet(ptBs, 13);	// video_object_layer_width

        MARKER();
        u32Height = BsGet(ptBs, 13); // video_object_layer_height
        MARKER();
    }

    if(u32Width == 0 || u32Height == 0)
    {
        return -1;
    }

    *pl32Width = u32Width;
    *pl32Height = u32Height;

    return 0;
}

/*=============================================================================
函 数 名: MP4BsHeaders
功    能: 解析头信息
全局变量: 无
参    数: ptBs                  码流结构指针[in]  
          ptMP4Info     输出头信息结构体指针[out]
返 回 值: VIDEO_SUCCESS: 成功；其他: 相应错误码
=============================================================================*/
static s32 MP4BsHeaders(TBitReader *ptBs, TMSVideoParam *ptMP4Info)
{
    unsigned int u32StartCode;
    s32 l32Ret = 0;

    ptMP4Info->m_bKeyFrame = 0;
    
    while(!ptBs->u32DecBsErr)
    {
        BsByteAlign(ptBs);
        
        u32StartCode = BsShow(ptBs, 32); //获取起始码
        
        if((u32StartCode & (~0x0ff)) == START_CODE)
        {
            if(u32StartCode == VO_SEQ_START_CODE)
            {
                BsSkip(ptBs, 32);
            }
            else if(u32StartCode == VSO_START_CODE)
            {
                BsSkip(ptBs, 32);
            }
            else if((u32StartCode & (~0xf)) == VOL_START_CODE) // 0x120 ... 0x12f
            {
				//解码VOL头信息
                BsSkip(ptBs, 32);

                l32Ret = GetVolHeader(ptBs, (s32*)&ptMP4Info->m_wVideoWidth, (s32*)&ptMP4Info->m_wVideoHeight);

                if(l32Ret!= 0)
                {
                    ptBs->u32DecBsErr = l32Ret;
                }
            }
            else if(u32StartCode == GOV_START_CODE) //解码GOV头
            {
                BsSkip(ptBs, 32);		
            }
            else if(u32StartCode == VOP_START_CODE)
            {
                s16 s16CodingType;

                BsSkip(ptBs, 32); // alread have the start_code in hands

                s16CodingType = (s16)BsGet(ptBs, 2);	// vop_coding_type

                ptMP4Info->m_bKeyFrame = (s16CodingType == 0);

                break;
            }
            else if(u32StartCode == USERDATA_START_CODE)//解码用户数据
            {
                BsSkip(ptBs, 32);
            }
            else
            {
                BsSkip(ptBs, 32);
            }
        }
        else  // continue seeking by forward 8 bits
        {
            BsSkip(ptBs, 8);
        }
    }

    return ptBs->u32DecBsErr; 
}

static BOOL32 ParseMP4(u8 *pBuf, s32 nSize, TFrameInfo *ptVideoFrameInfo)
{
	TBitReader tBs;
    s32 l32Ret = 0;
	
    if(NULL == pBuf)
    {
        return -1;
    }
	
    if(nSize == 0)
    {
        return -1;
    }
    
    //初始化码流信息
    BsInit(&tBs, pBuf, nSize);
    ptVideoFrameInfo->m_tVideoParam.m_bKeyFrame = FALSE;
	
    //解码VOL等头信息
    l32Ret = MP4BsHeaders(&tBs, &ptVideoFrameInfo->m_tVideoParam);
	
	if (0 == l32Ret)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// ps frame analysis
static u8 PTCovertStream2Rtp(u8 u8StreamType)
{
    u8 u8RtpType;
    
    switch (u8StreamType)
    {
	case PS_PAYLOAD_H264:
		u8RtpType = MEDIA_TYPE_H264;
		break;

	case PS_PAYLOAD_H265_OLD:
	case PS_PAYLOAD_H265:
		u8RtpType = MEDIA_TYPE_H265;
		break;

        case PS_PAYLOAD_MPEG4:
                u8RtpType = MEDIA_TYPE_MP4;
                break;
        
        case PS_PAYLOAD_MP2:
                u8RtpType = MEDIA_TYPE_MP2;
                break;
        
	case PS_PAYLOAD_G711A:
		u8RtpType = MEDIA_TYPE_PCMA;
		break;

	case PS_PAYLOAD_G7221:
		u8RtpType = MEDIA_TYPE_G7221;
		break;

	case PS_PAYLOAD_G7231:
		u8RtpType = MEDIA_TYPE_G7231;
		break;

	case PS_PAYLOAD_G729:
		u8RtpType = MEDIA_TYPE_G729;
		break;

	case PS_PAYLOAD_SVACA:
		u8RtpType = MEDIA_TYPE_SVACA;
		break;

	case PS_PAYLOAD_SVACV:
		u8RtpType = MEDIA_TYPE_SVACV;
		break;
		
	case PS_PAYLOAD_MP1:
	case PS_PAYLOAD_AACLC:
	case PS_PAYLOAD_MP2AAC:
		u8RtpType = MEDIA_TYPE_AACLC;
		break;
        
        default:
            u8RtpType = MEDIA_TYPE_NULL;
    }
    
    return u8RtpType;
}

static BOOL32 CleanFrameInfo(TFrameInfo *ptFrameInfo)
{
	if (ptFrameInfo == NULL)
	{
		return FALSE;
	}
	ptFrameInfo->m_byVideoType = 0;
	ptFrameInfo->m_byFrameRate = 0;

	memset(&ptFrameInfo->m_tVideoParam,0,sizeof(TMSVideoParam));
	ptFrameInfo->m_byAudioType = 0;
	memset(&ptFrameInfo->tAudioParam,0,sizeof(TKdvCodecContext));
	ptFrameInfo->bLastPackHaveSPS = 0; 

	memset(ptFrameInfo->byConnectBuf,0,sizeof(ptFrameInfo->byConnectBuf));
	ptFrameInfo->pbyLastPackPtr = NULL;  
	ptFrameInfo->nLastPackLength = 0;
	ptFrameInfo->wLastRtpPackSeq = 0; 
	ptFrameInfo->dwMaxPartionWidth = 0;
	ptFrameInfo->dwSPSId = 0;

	return TRUE;
}

//analysis ps frame, input buf without rtp head 
static BOOL32 ParsePS(u8 *pBuf, s32 nSize, u16 wRtpSeq, TFrameInfo *ptFrameInfo)
{
	s32 s32maxreadpos = nSize;
	u8 *pbydata = pBuf;
	u8 pframebuf[MAX_NALU_SIZE] = {0};
	s32 s32PackLen = -1;
	u8 bytype = 0;
	TParserPs *ptPsRead = &(ptFrameInfo->tParserps);
	s32 s32startpos = 0;
	BOOL32 bRet = FALSE;
	u8 byRtpPayload = MEDIA_TYPE_NULL;
	if(!ptFrameInfo || !pBuf)
	{
		KDVSTREAMPARSERPRINTF(TRUE, FALSE, "ParsePS param error\n");
		return FALSE;
	}

	//	clear frame information . means return ture at once time
	if(ptFrameInfo->m_tVideoParam.m_bKeyFrame != FALSE &&
	ptFrameInfo->m_tVideoParam.m_wVideoWidth != 0 &&
	ptFrameInfo->m_tVideoParam.m_wVideoHeight != 0)
	{
		//memset(ptFrameInfo, 0, sizeof(TFrameInfo));
		bRet = CleanFrameInfo(ptFrameInfo);
		if (!bRet)
		{
			KDVSTREAMPARSERPRINTF(TRUE, FALSE, "CleanFrameInfo  error\n");
			return FALSE;
		}
	}
	else if(ptFrameInfo->bLastPackHaveSPS && wRtpSeq - ptFrameInfo->wLastRtpPackSeq == 1)//return FALSE last time ,but still have sps, i connect two bufs to analysis again.
	{
		if(ptFrameInfo->nLastPackLength + nSize <= MAX_NALU_SIZE && pBuf)
		{
			memcpy(ptFrameInfo->byConnectBuf + ptFrameInfo->nLastPackLength, pBuf, nSize);
		}
		ptFrameInfo->bLastPackHaveSPS = FALSE;
		s32maxreadpos = ptFrameInfo->nLastPackLength + nSize;
		pbydata = ptFrameInfo->byConnectBuf;
	}

	//record current pack information ,used to copy when find sps flag.
	ptFrameInfo->pbyLastPackPtr = pBuf;
	ptFrameInfo->nLastPackLength = nSize;
	ptFrameInfo->wLastRtpPackSeq = wRtpSeq;		

	//begin to analysis 
	//(1)delete head of hik
	if (pbydata[0] == 0x49 && pbydata[1] == 0x4D && pbydata[2] == 0x4B && pbydata[3] == 0x48
		||pbydata[0] == 0x34 && pbydata[1] == 0x48 && pbydata[2] == 0x4B && pbydata[3] == 0x48 )
	{
		pbydata += 40;
		s32maxreadpos -= 40;
	}

	ptPsRead->pu8FrameBuf = pframebuf;
	ptPsRead->u32FrameLen = 0;

	//analysis ps frame , get es frame
	while(s32maxreadpos > 0)
	{
		s32startpos = find0x001(pbydata, s32maxreadpos);
		if(s32startpos < 0) // not find head 
		{
		  //  KDVSTREAMPARSERPRINTF(TRUE, FALSE, "ParsePS cannot find 0x001\n");
			return FALSE;
		}
		
		//have find head
		pbydata += s32startpos;
		s32maxreadpos -= s32startpos;

		bytype = pbydata[3];

		if((bytype & 0xf0) == PES_VIDEOSTREAM)
		{
			bytype = PES_VIDEOSTREAM;
		}
		else if((bytype & 0xe0) == PES_AUDIOSTREAM)
		{
			bytype = PES_AUDIOSTREAM;
		}

		switch(bytype)
		{
			case PS_HEAD_BYTE:
				//clear ps frame  information first 
// 				memset(ptFrameInfo, 0, sizeof(TFrameInfo));//clear information about last psframe. 				
// 								ptPsRead->pu8FrameBuf = pframebuf; // ptr need set again
				bRet = CleanFrameInfo(ptFrameInfo);
				if (!bRet)
				{
					KDVSTREAMPARSERPRINTF(TRUE, FALSE, "CleanFrameInfo  error\n");
					return FALSE;
				}
				ptPsRead->pu8FrameBuf = pframebuf; 
								s32PackLen = parsepshead(ptPsRead, pbydata, s32maxreadpos);
				break;
			case PS_SYSTEMHEAD_BYTE:
				s32PackLen = parsesystemhead(ptPsRead, pbydata, s32maxreadpos);
				break;
			case PS_MAPHEAD_BYTE:
				s32PackLen = parsepsmhead(ptPsRead, pbydata, s32maxreadpos);			
				break;
			case PS_END_BYTE:
//                  KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[ParsePS]Get A PS End.\n");
				break;
				
			case PES_VIDEOSTREAM:
				s32PackLen = parsepeshead(ptPsRead, pbydata, s32maxreadpos);
				ptFrameInfo->m_byVideoType = PTCovertStream2Rtp(ptPsRead->au8StreamType[ptPsRead->u8StreamId]);
				break;
			case PES_AUDIOSTREAM:
				s32PackLen = parsepeshead(ptPsRead, pbydata, s32maxreadpos);
				ptFrameInfo->m_byAudioType = PTCovertStream2Rtp(ptPsRead->au8StreamType[ptPsRead->u8StreamId]);
				break;
			default:
				//unknown type, analysis by ps->u8VideoType.
				if(MEDIA_TYPE_H264 == ptFrameInfo->m_byVideoType || MEDIA_TYPE_H265 == ptFrameInfo->m_byVideoType
                  || MEDIA_TYPE_SVACV == ptFrameInfo->m_byVideoType) // 264 解析
				{
				        ptFrameInfo->m_byVideoType = PTCovertStream2Rtp(ptPsRead->au8StreamType[ptPsRead->u8StreamId]);
//                                        KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[ParsePS]VideoType is %d.\n", ptFrameInfo->m_byVideoType);
					bRet = ParseWith001(pbydata, s32maxreadpos, ptFrameInfo);
//                                        KDVSTREAMPARSERPRINTF(TRUE, FALSE, "ParsePS ParseWith001(%d)\n", bRet);
					return bRet;
				}
				break;
		}

		if(s32PackLen < 0 || s32maxreadpos < s32PackLen)
		{
			break;
		}
		pbydata += s32PackLen;
		s32maxreadpos -= s32PackLen;
	}

	//analysis key frame by stream is not ok  now, just analysis by psm flag first.Let's modify it later .
	pbydata = ptPsRead->pu8FrameBuf;
	s32maxreadpos = ptPsRead->u32FrameLen;

	// analysis frame information  by type 
	byRtpPayload = PTCovertStream2Rtp(ptPsRead->au8StreamType[ptPsRead->u8StreamId]);
	if (MEDIA_TYPE_H264 == byRtpPayload || MEDIA_TYPE_H265 == byRtpPayload
		|| MEDIA_TYPE_SVACV == byRtpPayload) // 264 解析
	{
		bRet = ParseWith001(pbydata, s32maxreadpos, ptFrameInfo);
		//             KDVSTREAMPARSERPRINTF(TRUE, FALSE, "ParsePS ParseWith001(%d)\n", bRet);
		return bRet;
	}
	else if(MEDIA_TYPE_NULL != byRtpPayload)
	{
	    //目前视频仅支持(MEDIA_TYPE_H264, MEDIA_TYPE_H265, MEDIA_TYPE_SVACV)三种
	    //若PTCovertStream2Rtp返回类型非以上三种及MEDIA_TYPE_NULL及为音频，返回TRUE。
		return TRUE;
	}

	return FALSE;
}

//analysis audio frame, audio mode come from  ParseRtpHead Analysis.
//convert audio mode to context

// 1, context to mode 
static BOOL32 KdvContextToAudioMode_old(TKdvCodecContext *ptAVcontext, u8 *pbyAudioMode, u8 byMediaType)
{
	BOOL32 bRet = TRUE;
	u16 wIndex =0;

	if (NULL == ptAVcontext)
	{
		return FALSE;
	}
	switch(byMediaType)
	{
	case MEDIA_TYPE_AACLC:
		*pbyAudioMode = 0;
		for (wIndex = 0; wIndex < 16; wIndex++)
		{
			if (ptAVcontext->sample_rate == audio_sample_PerSec_old[wIndex])
				break;
		}
		if(wIndex ==16)
		{
			return FALSE;
		}
		*pbyAudioMode =( (ptAVcontext->channels<<4)&0x30)|( wIndex&0xF);
		break;
	case MEDIA_TYPE_MP3:
		{
			if (ptAVcontext->sample_rate == 11025 && ptAVcontext->channels == 1)
			{
				*pbyAudioMode = 0;
			}
			else if (ptAVcontext->sample_rate == 11025 && ptAVcontext->channels == 2)
			{
				*pbyAudioMode = 1;
			}
			else if (ptAVcontext->sample_rate == 22050 && ptAVcontext->channels == 2)
			{
				*pbyAudioMode = 2;
			}
			else if (ptAVcontext->sample_rate == 32000 && ptAVcontext->channels == 1)
			{
				*pbyAudioMode = 3;
			}
			else if (ptAVcontext->sample_rate == 32000 && ptAVcontext->channels == 2)
			{
				*pbyAudioMode = 4;
			}
			else
			{
				*pbyAudioMode = 0;
			}
			break;
		}
	case MEDIA_TYPE_PCMA:
	case MEDIA_TYPE_PCMU:
	case MEDIA_TYPE_G729:
	case MEDIA_TYPE_G7221:
	case MEDIA_TYPE_G7231:
	case MEDIA_TYPE_G7221C:
	case MEDIA_TYPE_G722:
	case MEDIA_TYPE_G728:
	case MEDIA_TYPE_AMR:	
	case MEDIA_TYPE_ADPCM:
		*pbyAudioMode = 0;
		break;
	default:
		*pbyAudioMode = 0;
		bRet = FALSE;
		KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[KdvContextToAudiomode_old] unkonw audio type %d \n", byMediaType);
		break;
	}

	return bRet;
}

// 2. mode to context
static BOOL32  KdvAudioModeToContext_old(TKdvCodecContext *ptAVcontext, u8 byAudioMode, u8 byMediaType)
{
	BOOL32 bRet = TRUE;
	u16 wIndex = 0;

	switch(byMediaType)
	{
	case MEDIA_TYPE_AACLC:
		ptAVcontext->bybits = 16;
		if(0 == byAudioMode)
		{
			ptAVcontext->sample_rate = 16000;
			ptAVcontext->channels= 1;
		}
		else
		{
			wIndex =byAudioMode&0xF;
			ptAVcontext->sample_rate = audio_sample_PerSec_old[wIndex];
			ptAVcontext->channels= (byAudioMode&0x30)>>4;
		}
		break;
	case MEDIA_TYPE_MP3:
		{
			ptAVcontext->bybits = 16;
			if (0 == byAudioMode)
			{
				ptAVcontext->sample_rate = 11025;
				ptAVcontext->channels= 1;
			}
			else if (1 == byAudioMode)
			{
				ptAVcontext->sample_rate = 11025;
				ptAVcontext->channels= 2;
			}
			else if (2 == byAudioMode)
			{
				ptAVcontext->sample_rate = 22050;
				ptAVcontext->channels= 2;
			}
			else if (3 == byAudioMode)
			{
				ptAVcontext->sample_rate = 32000;
				ptAVcontext->channels= 1;
			}
			else if (4 == byAudioMode)
			{
				ptAVcontext->sample_rate = 32000;
				ptAVcontext->channels= 2;
			}
			else
			{
				bRet = FALSE;
			}
			break;
		}
	case MEDIA_TYPE_PCMA:
	case MEDIA_TYPE_AMR:
	case MEDIA_TYPE_ADPCM:
	case MEDIA_TYPE_PCMU:
	case MEDIA_TYPE_G729:
	case MEDIA_TYPE_G728:
		ptAVcontext->channels = 1;
		ptAVcontext->bybits = 16;
		ptAVcontext->sample_rate = 8000;
		break;

	case MEDIA_TYPE_G7221:
	case MEDIA_TYPE_G7221C:
		ptAVcontext->channels = 1;
		ptAVcontext->bybits = 16;
		ptAVcontext->sample_rate = 32000;
		break;

	case MEDIA_TYPE_G722:
		ptAVcontext->channels = 1;
		ptAVcontext->bybits = 16;
		ptAVcontext->sample_rate = 16000;
		break;

	case MEDIA_TYPE_G7231:
		ptAVcontext->channels = 1;
		ptAVcontext->bybits = 0;
		ptAVcontext->sample_rate = 8000;
		break;
	default:
		bRet = FALSE;
		KDVSTREAMPARSERPRINTF(TRUE, FALSE, "[KdvAudiomodeToContext] unkonw audio type %d \n", byMediaType);
		break;
	}

	return bRet;
}

static BOOL32 ParseFrame(u8 byPayload, u8* pbyFrameBuf, u32 dwFrameLen, TFrameInfo* ptFrameInfo)
{
	u32 dwNum = 0;
	u8 *pbyNaluData = NULL;
	BOOL32 bMark = FALSE;
	u32 dwPreNaluLen = 0;
	u8 byInterval = 4;//默认为0001
	u8 byLastInterval = 4;//默认为0001
	BOOL32 bRet = TRUE;
	BOOL32 bFind  = FALSE;
	if (NULL == pbyFrameBuf || 0 == dwFrameLen || NULL == ptFrameInfo)
	{
		return FALSE;
	}
    if (dwFrameLen <= byInterval)
    {
        return FALSE;
    }
	pbyNaluData = pbyFrameBuf;
	for (dwNum = 0; dwNum < dwFrameLen; dwNum++)
	{
		bFind = FALSE;
		if (dwNum >= dwFrameLen - 4)
		{
			dwPreNaluLen = dwFrameLen - (pbyNaluData - pbyFrameBuf);
			bMark = TRUE;
		}
		else if (0 == pbyFrameBuf[dwNum] &&  0 == pbyFrameBuf[dwNum + 1] 
            && 0 == pbyFrameBuf[dwNum + 2] && 1 == pbyFrameBuf[dwNum + 3])//0001
		{
			dwPreNaluLen = dwNum - (pbyNaluData - pbyFrameBuf);
			byLastInterval = 4;
			bFind = TRUE;
		}
		else if (0 == pbyFrameBuf[dwNum] &&  0 == pbyFrameBuf[dwNum + 1] 
            && 1 == pbyFrameBuf[dwNum + 2])//001
		{
			dwPreNaluLen = dwNum - (pbyNaluData - pbyFrameBuf);
			byLastInterval = 3;
			bFind = TRUE;
		}
		else
		{
			dwPreNaluLen = 0;
		}
		
		
		if (0 != dwPreNaluLen)
		{	
			if (MEDIA_TYPE_H264 == byPayload)
			{
				bRet = ParseH264(pbyNaluData+byInterval, dwPreNaluLen - byInterval, ptFrameInfo);
			}
			else if (MEDIA_TYPE_H265 == byPayload)
			{
				bRet = ParseH265(pbyNaluData+byInterval, dwPreNaluLen - byInterval, ptFrameInfo);
			}
			else if (MEDIA_TYPE_SVACV == byPayload)
			{
				bRet = ParseSVACV(pbyNaluData+byInterval, dwPreNaluLen - byInterval, ptFrameInfo);
			}
			else
			{
				break;
			}			

			if (bMark)
            {				
                break;
            }
            else
            {				
                pbyNaluData = pbyFrameBuf + dwNum;										
				
                dwNum += byInterval - 1;
            }		
		}
		else
		{
			if (TRUE == bFind)
			{
				dwNum += byLastInterval - 1;
			}
		}
		byInterval = byLastInterval;
		//单nalu情况下对P帧进行优化，找到一个0001之后即不再遍历查找，降低CPU
		if (MEDIA_TYPE_H264 == byPayload && ((H264_NAL_SLICE == (pbyNaluData[byInterval] & 0x1F) || H264_NAL_IDRSLICE == (pbyNaluData[byInterval] & 0x1F))))
		{
			dwNum = dwFrameLen - 2;
		}
		if (MEDIA_TYPE_H265 == byPayload && ((H265_NAL_SLICE == ((pbyNaluData[byInterval] & 0x7e)>>1) || H265_NAL_IDRSLICE == ((pbyNaluData[byInterval] & 0x7e)>>1))))
		{
			dwNum = dwFrameLen - 2;
		}
		if (MEDIA_TYPE_SVACV == byPayload && ((SVAC_NAL_SLICE == ((pbyNaluData[byInterval]  & 0x3c)>>2) || SVAC_NAL_IDRSLICE == ((pbyNaluData[byInterval] & 0x3c)>>2))))
		{
			dwNum = dwFrameLen - 2;
		}
	}//for
	return TRUE;
}


#ifdef __cplusplus
}
#endif

#endif

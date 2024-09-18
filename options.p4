#ifndef _TNA_PARROT_HASH_OPTIONS_
#define _TNA_PARROT_HASH_OPTIONS_


/*************************************************************************
 ************* C O N S T A N T S    A N D   T Y P E S  *******************
*************************************************************************/

#if __TARGET_TOFINO__ == 1
typedef bit<3> mirror_type_t;
#else
typedef bit<4> mirror_type_t;
#endif


/* Table Sizing */
const int IPV4_LPM_TABLE_SIZE  = 12288;


#endif
/* -*- P4_16 -*- */
#include <core.p4>
#include <tna.p4>

#include "options.p4"
#include "headers.p4"

#define Use_Const_Entries 0


/***********  I N G R E S S   P R O C E S S I N G   *************/

struct ph_ingress_metadata_t {}

struct ph_metadata_t {
    bit<32> mph_bkt_idx;
    bit<8>  mph_bkt_seed;
    bit<2>  mph_slot_idx; 
    bit<32> mph_slot_value;
    bit<32> stash_chain_offset;
    bit<32> mac_dst_addr;
    bit<8>  fwd_port;
}

struct pair {
    bit<32>  mac_addr;
    bit<32>  port;
}

parser IngressParser(
        packet_in pkt,    
        out ph_header_t hdr,
        out ph_ingress_metadata_t ig_md,
        out ingress_intrinsic_metadata_t ig_intr_md)
{
    Checksum() ipv4_checksum;
    state start {
        pkt.extract(ig_intr_md);
        pkt.advance(PORT_METADATA_SIZE);
        transition parse_ethernet;
    }
    state parse_ethernet {
        pkt.extract(hdr.ethernet);
        transition select(hdr.ethernet.etherType) {
            ETHERTYPE_IPV4 :  parse_ipv4;
            default        :  accept;
        }
    }
    state parse_ipv4 {
        pkt.extract(hdr.ipv4);
        ipv4_checksum.add(hdr.ipv4);
        transition select (hdr.ipv4.protocol) {
            IP_PROTOCOLS_TCP : parse_tcp;
            IP_PROTOCOLS_UDP : parse_udp;
            default : accept;
        }
    }
    state parse_tcp {
        pkt.extract(hdr.tcp);
        transition accept;
    }
    state parse_udp {
        pkt.extract(hdr.udp);
        transition accept;
        //transition parse_my_header;
    }

    //state parse_my_header {
    //    pkt.extract(hdr.my_header);
    //    transition accept;
    //}
}


#include "routing.p4"
control Ingress(
        inout ph_header_t hdr,
        inout ph_ingress_metadata_t ig_md,
        in    ingress_intrinsic_metadata_t ig_intr_md,
        in    ingress_intrinsic_metadata_from_parser_t ig_prsr_md,
        inout ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md,
        inout ingress_intrinsic_metadata_for_tm_t ig_tm_md)
{
    ph_metadata_t meta;
    IngressRouting() Routing;

    // (1) Range match part
    action Get_bkt_idx_with_range_match_action(bit<32> bucket_id) {
        meta.mph_bkt_idx = bucket_id;
    }
    @pragma stage 0
    table Get_bkt_idx_with_range_match {
        key = {
            hdr.ethernet.dstAddr[15:0]: range;
        }
        actions = {
            Get_bkt_idx_with_range_match_action;
            NoAction;
        }
        #if Use_Const_Entries
        const entries = {
            (0..3) : Get_bkt_idx_with_range_match_action(0);
            (4..7) : Get_bkt_idx_with_range_match_action(1);
            (8..11) : Get_bkt_idx_with_range_match_action(2);
            (12..15) : Get_bkt_idx_with_range_match_action(3);
            (16..19) : Get_bkt_idx_with_range_match_action(4);
            (20..23) : Get_bkt_idx_with_range_match_action(5);
            (24..27) : Get_bkt_idx_with_range_match_action(6);
            (28..31) : Get_bkt_idx_with_range_match_action(7);
            (32..35) : Get_bkt_idx_with_range_match_action(8);
            (36..39) : Get_bkt_idx_with_range_match_action(9);
            (40..43) : Get_bkt_idx_with_range_match_action(10);
            (44..47) : Get_bkt_idx_with_range_match_action(11);
            (48..51) : Get_bkt_idx_with_range_match_action(12);
            (52..55) : Get_bkt_idx_with_range_match_action(13);
            (56..59) : Get_bkt_idx_with_range_match_action(14);
            (60..63) : Get_bkt_idx_with_range_match_action(15);
        }
        #endif
        size = 1024;
    }


    // (2) Read seeds part, 0-7
    Register<bit<8>, bit<32>> (32w1024, 0) seeds;
    RegisterAction<bit<8>, bit<32>, bit<8>> (seeds) Mph_bkt_seeds_read_reg_action = {
        void apply(inout bit<8> value, out bit<8> read_value) {  
            read_value = value; 
        }
    };
    action Mph_bkt_seeds_read_action() {
        meta.mph_bkt_seed = Mph_bkt_seeds_read_reg_action.execute(meta.mph_bkt_idx);
    }
    @pragma stage 1
    table Read_Mph_bkt_seeds {
        key = {}
        actions = {
            Mph_bkt_seeds_read_action;
        }
        default_action = Mph_bkt_seeds_read_action();
        size = 1;
    }


    // (3) Read bkts part
    Register<bit<32>, bit<32>> (32w1024, 0) buckets; // 4*8
    RegisterAction<bit<32>, bit<32>, bit<32>> (buckets) Mph_bkts_read_reg_action = {
        void apply(inout bit<32> value, out bit<32> read_value) {
            read_value = value;
        }
    };
    action Mph_bkts_read_action() {
        meta.mph_slot_value = Mph_bkts_read_reg_action.execute(meta.mph_bkt_idx);
    }
    @pragma stage 1
    table Read_Mph_bkts {
        actions = {
            Mph_bkts_read_action;
        }
        default_action = Mph_bkts_read_action();
        size = 1;
    }


    // Hash functions
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x00, 8w0xFF) poly0;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x01, 8w0xFF) poly1;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x02, 8w0xFF) poly2;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x03, 8w0xFF) poly3;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x04, 8w0xFF) poly4;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x05, 8w0xFF) poly5;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x06, 8w0xFF) poly6;
    CRCPolynomial<bit<8>>(8w0xB7, true, false, false, 8w0x07, 8w0xFF) poly7;

    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly0) hash0;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly1) hash1;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly2) hash2;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly3) hash3;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly4) hash4;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly5) hash5;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly6) hash6;
    Hash<bit<8>>(HashAlgorithm_t.CUSTOM, poly7) hash7;

    // Compute slot part
    action Get_slot_idx_with_hash_0() { 
        meta.mph_slot_idx = hash0.get(hdr.ethernet.dstAddr)[1:0]; 
    }
    action Get_slot_idx_with_hash_1() {
        meta.mph_slot_idx = hash1.get(hdr.ethernet.dstAddr)[1:0]; 
    }
    action Get_slot_idx_with_hash_2() {
        meta.mph_slot_idx = hash2.get(hdr.ethernet.dstAddr)[1:0];
    }
    action Get_slot_idx_with_hash_3() {
        meta.mph_slot_idx = hash3.get(hdr.ethernet.dstAddr)[1:0];
    }
    action Get_slot_idx_with_hash_4() { 
        meta.mph_slot_idx = hash4.get(hdr.ethernet.dstAddr)[1:0]; 
    }
    action Get_slot_idx_with_hash_5() {
        meta.mph_slot_idx = hash5.get(hdr.ethernet.dstAddr)[1:0]; 
    }
    action Get_slot_idx_with_hash_6() {
        meta.mph_slot_idx = hash6.get(hdr.ethernet.dstAddr)[1:0];
    }
    action Get_slot_idx_with_hash_7() {
        meta.mph_slot_idx = hash7.get(hdr.ethernet.dstAddr)[1:0];
    }
    @pragma stage 2
    table Compute_mph_slot_idx_with_hash {
        key = { 
            meta.mph_bkt_seed: exact; 
        }
        actions = {
            Get_slot_idx_with_hash_0;
            Get_slot_idx_with_hash_1;
            Get_slot_idx_with_hash_2;
            Get_slot_idx_with_hash_3;
            Get_slot_idx_with_hash_4;
            Get_slot_idx_with_hash_5;
            Get_slot_idx_with_hash_6;
            Get_slot_idx_with_hash_7;
            NoAction;
        }
        const entries = {
            0x0 & 0x0: Get_slot_idx_with_hash_0;
            0x1 & 0x1: Get_slot_idx_with_hash_1;
            0x2 & 0x3: Get_slot_idx_with_hash_2;
            0x3 & 0x3: Get_slot_idx_with_hash_3;
            0x4 & 0x7: Get_slot_idx_with_hash_4;
            0x5 & 0x7: Get_slot_idx_with_hash_5;
            0x6 & 0x7: Get_slot_idx_with_hash_6;
            0x7 & 0x7: Get_slot_idx_with_hash_7;
        }
        size = 256;
        default_action = NoAction;
    }


    // Extract fwd port part, 188 full
    action Read_slot_with_idx_0() {
        meta.fwd_port = meta.mph_slot_value[31:24];
    }
    action Read_slot_with_idx_1() {
        meta.fwd_port = meta.mph_slot_value[23:16];
    }
    action Read_slot_with_idx_2() {
        meta.fwd_port = meta.mph_slot_value[15:8];
    }
    action Read_slot_with_idx_3() {
        meta.fwd_port = meta.mph_slot_value[7:0];
    }
    @pragma stage 3
    table Get_forward_port_from_slot_value {
        key = {
            meta.mph_slot_idx: exact;
        }
        actions = {
            Read_slot_with_idx_0;
            Read_slot_with_idx_1;
            Read_slot_with_idx_2;
            Read_slot_with_idx_3;
            NoAction;
        }
        const entries = {
            0x0 & 0x1: Read_slot_with_idx_0;
            0x1 & 0x1: Read_slot_with_idx_1;
            0x2 & 0x3: Read_slot_with_idx_2;
            0x3 & 0x3: Read_slot_with_idx_3;
        }
        size = 4;
    }


    // Read stash chain idx, ~1024
    Register<bit<32>, bit<32>> (32w1024, 0) chain_offsets;  // 32bit bucket stash chain loc, better init with 65535 
    RegisterAction<bit<32>, bit<32>, bit<32>> (chain_offsets) Chain_offset_read_action = {
        void apply(inout bit<32> value, out bit<32> read_value) {
            read_value = value;
        }
    };
    action Read_stash_chain_offset_for_bucket_action() {
        meta.stash_chain_offset = Chain_offset_read_action.execute(meta.mph_bkt_idx);
    }
    @pragma stage 3
    table Read_stash_chain_offset_for_bucket {
        actions = {
            Read_stash_chain_offset_for_bucket_action;
        }
        default_action = Read_stash_chain_offset_for_bucket_action();
        size = 1024;
    }



    Register<bit<32>, bit<32>>(32w512, 0) chains_mac;
    RegisterAction<bit<32>, bit<32>, bit<32>> (chains_mac) Stash_chain_mac_addr_read_reg_action = {
        void apply(inout bit<32> value, out bit<32> read_value) {
            read_value= value;
        }
    };
    action Read_mac_addr_in_stash_chains_action() {
        meta.mac_dst_addr = Stash_chain_mac_addr_read_reg_action.execute(meta.stash_chain_offset);
    }
    @pragma stage 4
    table Read_mac_addr_in_stash_chains {
        actions = {
            Read_mac_addr_in_stash_chains_action;
        }
        default_action = Read_mac_addr_in_stash_chains_action();
        size = 1;
    }


    
    Register<bit<8>, bit<32>>(32w512, 0) chain_ports;
    RegisterAction<bit<8>, bit<32>, bit<8>> (chain_ports) Stash_chain_forward_port_read_reg_action = {
        void apply(inout bit<8> value, out bit<8> read_value) {
            read_value= value;
        }
    };
    action Read_forward_port_in_stash_chains_action() {
        meta.fwd_port = Stash_chain_forward_port_read_reg_action.execute(meta.stash_chain_offset);
    }
    @pragma stage 5
    table Read_forward_port_in_stash_chains {
        actions = {
            Read_forward_port_in_stash_chains_action;
        }
        default_action = Read_forward_port_in_stash_chains_action();
        size = 1;
    }
    @pragma stage 6
    table Read_forward_port_in_stash_chains_2 {
        actions = {
            Read_forward_port_in_stash_chains_action;
        }
        default_action = Read_forward_port_in_stash_chains_action();
        size = 1;
    }


    action Get_overflow_table_for_caching_fwd_port_action(bit<8> port_id) {
        meta.fwd_port = port_id;
    }
    @pragma stage 7
    table Overflow_table_for_caching {
        key = {
            hdr.ethernet.dstAddr[31:0]: exact;
        }
        actions = {
            Get_overflow_table_for_caching_fwd_port_action;
            NoAction;
        }
        const entries = {
            (0x16) : Get_overflow_table_for_caching_fwd_port_action(189);
            (0x15) : Get_overflow_table_for_caching_fwd_port_action(186);
            (0x14) : Get_overflow_table_for_caching_fwd_port_action(145);
            (0x13) : Get_overflow_table_for_caching_fwd_port_action(132);
        }
        size = 2048;
    }



    apply {
        
        // range match 

        Get_bkt_idx_with_range_match.apply();
        
        // mph to get port
        Read_Mph_bkt_seeds.apply();
        Read_Mph_bkts.apply();
        Compute_mph_slot_idx_with_hash.apply();
        Get_forward_port_from_slot_value.apply();


        // linked node for extra two (when stash exists but it is in mph)
        
        Read_stash_chain_offset_for_bucket.apply();
        if (meta.stash_chain_offset != 32w0xffff) { // more than four elements in the bkt
                
            Read_mac_addr_in_stash_chains.apply();
            if (meta.mac_dst_addr == hdr.ethernet.dstAddr[31:0]) {
                Read_forward_port_in_stash_chains.apply();
            } else {
                meta.stash_chain_offset = meta.stash_chain_offset + 1;
                if (meta.stash_chain_offset != 512) {
                    Read_forward_port_in_stash_chains_2.apply();
                }
            }
        
            // overflow table
            // old port value will be written if there is a exact macth in overflow list
            Overflow_table_for_caching.apply();
        }
        
        // Send
        ig_tm_md.ucast_egress_port = (bit<9>) meta.fwd_port;
        //Routing.apply(hdr, meta, ig_md, ig_intr_md, ig_prsr_md, ig_dprsr_md, ig_tm_md);
        ig_tm_md.bypass_egress = 1w1;
    }
}



/*********************  D E P A R S E R  ************************/

control IngressDeparser(
        packet_out pkt,
        inout ph_header_t hdr,
        in    ph_ingress_metadata_t ig_md,
        in    ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md)
{
    Checksum() ipv4_checksum;
    apply {
        hdr.ipv4.hdrChecksum = ipv4_checksum.update({
            hdr.ipv4.version,
            hdr.ipv4.ihl,
            hdr.ipv4.diffserv,
            hdr.ipv4.totalLen,
            hdr.ipv4.identification,
            hdr.ipv4.flags,
            hdr.ipv4.fragOffset,
            hdr.ipv4.ttl,
            hdr.ipv4.protocol,
            hdr.ipv4.srcAddr,
            hdr.ipv4.dstAddr
        });
        pkt.emit(hdr);
    }
}


#include "egress.p4"

Pipeline(
    IngressParser(),
    Ingress(),
    IngressDeparser(),
    EmptyEgressParser(),
    EmptyEgress(),
    EmptyEgressDeparser()
) pipe;


Switch(pipe) main;

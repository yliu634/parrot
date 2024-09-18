

control IngressRouting(
        inout ph_header_t hdr,
        inout ph_metadata_t meta,
        inout ph_ingress_metadata_t ig_md,
        in ingress_intrinsic_metadata_t ig_intr_md,
        in ingress_intrinsic_metadata_from_parser_t ig_prsr_md,
        inout ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md,
        inout ingress_intrinsic_metadata_for_tm_t ig_tm_md) 
{
    action set_egress(bit<9> egress_spec) {
        ig_tm_md.ucast_egress_port = (bit<9>) meta.fwd_port; //(bit<9> egress_spec)
        ig_dprsr_md.drop_ctl = 0x0;
        hdr.ipv4.ttl=hdr.ipv4.ttl-1;
    }

    action drop() {
        ig_dprsr_md.drop_ctl = 0x1;
    }
    
    table ipv4_routing {
        key = { hdr.ipv4.dstAddr : exact; }
        actions = { set_egress; drop; }
        size = 16;
        default_action = drop();
        const entries = {
            0xc0a80101 &&& 0xffffffff: set_egress(0);
            0xc0a80102 &&& 0xffffffff: set_egress(188);
        }
    }
    
    apply {
        ipv4_routing.apply();
    }
}

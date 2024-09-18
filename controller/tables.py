import logging

from base_table import Table

class IngressTables(Table):

    def __init__(self, target, gc, bfrt_info):
        # Set up base class
        super(IngressTables, self).__init__(target, gc)
        self.log = logging.getLogger(__name__)

        self.tables = [
            bfrt_info.table_get("Ingress.Get_bkt_idx_with_range_match")
        ]
        self.ipv4_lpm = self.tables[0]
        #self.mirror_cfg_table = bfrt_info.table_get("$mirror.cfg")
        
        # Clear tables
        self._clear()

    
    def config_default_entries(self):
        action_data = self.ipv4_lpm.make_data([], action_name = "Ingress.drop")
        self.ipv4_lpm.default_entry_set(target = self.target, data = action_data )

    def add_ipv4_lpm_entry(self, dst_addr, port):
        self.ipv4_lpm.entry_add(
            self.target,
            [self.ipv4_lpm.make_key([self.gc.KeyTuple('hdr.ipv4.dst_addr',
                                        self.gc.ipv4_to_bytes(dst_addr),
                                        prefix_len=32)])],
            [self.ipv4_lpm.make_data([self.gc.DataTuple('port', port),
                                        self.gc.DataTuple('new_mac_da', 0x201234567890)],
                                        'Ingress.l3_switch')]
        )
    

    def _clear(self):
        super(IngressTables, self)._clear()



# ingress tables
#self.ingressTables = IngressTables(self.target, gc, self.bfrt_info)
#self.ingressTables.config_default_entries()
#self.ingressTables.add_ipv4_lpm_entry('192.168.1.1', 0)
#self.ingressTables.add_ipv4_lpm_entry('192.168.1.2', 188)
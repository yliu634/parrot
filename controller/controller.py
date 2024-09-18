import os
import re
import sys
import glob
import yaml
import time
import signal
import random
import socket
import threading
import argparse
import logging

# Add BF Python to search path
bfrt_location = '{}/lib/python*/site-packages/tofino'.format(
    os.environ['SDE_INSTALL'])
sys.path.append(glob.glob(bfrt_location)[0])

import bfrt_grpc.bfruntime_pb2 as bfruntime_pb2
import bfrt_grpc.client as gc

# Configure tables of ingress control
from tables import *

# Regexes
mac_address_regex = re.compile(':'.join(['[0-9a-fA-F]{2}'] * 6))
front_panel_regex = re.compile('([0-9]+)/([0-9]+)$')

# IPv4 validation
def validate_ip(value):
    ''' Validate IP address string '''
    try:
        socket.inet_aton(value)
        return True
    except:
        return False

def sigint_handler(signal_received, frame):
    print 'SIGINT or CTRL-C detected. Stopping controller.' 
        
    # Flush log, stdout, stderr
    sys.stdout.flush()
    sys.stderr.flush()
    logging.shutdown()

    # Exit
    os.kill(os.getpid(), signal.SIGTERM)

def macAddr_to_i48(addr):
    def uint_to_i48(u):
        if u > 0x7FFFFFFFFFFF:
            u -= 0x1000000000000
        return u
    byte_array = [int(b, 16) for b in addr.split(":")]
    res = 0
    for b in byte_array:
        res = res * 256 + b
    return uint_to_i48(res)

class Controller(object):

    def __init__(self):
        super(Controller, self).__init__()

        self.log = logging.getLogger(__name__)
        self.log.info('parrot hashing controller')

    def critical_error(self, msg):
        self.log.critical(msg)
        # print msg, file=sys.stderr
        sys.stderr.write(msg)
        logging.shutdown()
        os.kill(os.getpid(), signal.SIGTERM)

    def setup(self,
              program,
              switch_mac,
              switch_ip,
              bfrt_ip,
              bfrt_port,
              ports_file):

        # device 0
        self.dev = 0
        # target all pipes
        self.target = gc.Target(self.dev, pipe_id=0xffff)
        # set switch addresses
        self.set_switch_mac_and_ip(switch_mac, switch_ip)

        # connect to BFRT server
        try:
            self.interface = gc.ClientInterface('{}:{}'.format(bfrt_ip, bfrt_port),
                                           client_id=0,
                                           device_id=self.dev)
        except RuntimeError as re:
            msg = re.args[0] % re.args[1]
            self.critical_error(msg)
        else:
            self.log.info('Connected to BFRT server {}:{}'.format(bfrt_ip, bfrt_port))
        
        # bind program interface
        try:
            self.interface.bind_pipeline_config(program)
        except gc.BfruntimeForwardingRpcException:
            self.critical_error('P4 program {} not found!'.format(program))

        try:
            # init all tables for program
            self.bfrt_info = self.interface.bfrt_info_get(program)

            # Range match table 
            table_range_match = self.bfrt_info.table_get("Ingress.Get_bkt_idx_with_range_match")
            table_range_match.entry_del(self.target)
            resp = table_range_match.entry_get(self.target, None, {"from_hw": True})
            for data, key in resp:
                assert 0, "Shouldn't have hit here since table is supposed to be empty"
            for i in range(0, 1024):
                table_range_match.entry_add(
                    self.target,
                    [table_range_match.make_key([gc.KeyTuple('$MATCH_PRIORITY', 1),
                                                gc.KeyTuple('hdr.ethernet.dstAddr[15:0]',
                                                            low=i*4,
                                                            high=i*4+4)])],
                    [table_range_match.make_data([gc.DataTuple('bucket_id', i)],
                                                'Ingress.Get_bkt_idx_with_range_match_action')]
                )
            

            # Register seeds part
            register_seeds = self.bfrt_info.table_get("Ingress.seeds")
            for register_idx in range(0, 1024):
                register_seed = random.randint(0, 7);
                register_seeds.entry_add(
                    self.target,
                    [register_seeds.make_key([gc.KeyTuple('$REGISTER_INDEX', register_idx)])],
                    [register_seeds.make_data([
                        gc.DataTuple('Ingress.seeds.f1', register_seed)])])
            

            # Register buckets, with all as 188;
            register_bucket = 188 + (188 << 8) + (188 << 16) + (188 << 24);
            register_buckets = self.bfrt_info.table_get("Ingress.buckets")
            for register_idx in range(0, 1024):
                register_buckets.entry_add(
                    self.target,
                    [register_buckets.make_key([gc.KeyTuple('$REGISTER_INDEX', register_idx)])],
                    [register_buckets.make_data([
                        gc.DataTuple('Ingress.buckets.f1', register_bucket)])])

            
            # Register chain offsets
            register_offsets = self.bfrt_info.table_get("Ingress.chain_offsets")
            for register_idx in range(0, 1024):
                register_offset = random.randint(0, 628);
                if (register_offset > 511):
                    register_offset = 0xffff;
                register_offsets.entry_add(
                    self.target,
                    [register_offsets.make_key([gc.KeyTuple('$REGISTER_INDEX', register_idx)])],
                    [register_offsets.make_data([
                        gc.DataTuple('Ingress.chain_offsets.f1', register_offset)])])


            # Register chain mac
            register_chain_mac = self.bfrt_info.table_get("Ingress.chains_mac")
            for register_idx in range(0, 512):
                register_mac = random.randint(0, 4095);
                register_chain_mac.entry_add(
                    self.target,
                    [register_chain_mac.make_key([gc.KeyTuple('$REGISTER_INDEX', register_idx)])],
                    [register_chain_mac.make_data([
                        gc.DataTuple('Ingress.chains_mac.f1', register_mac)])])

            
            # Register chains port
            register_chain_ports = self.bfrt_info.table_get("Ingress.chain_ports")
            for register_idx in range(0, 512):
                register_chain_ports.entry_add(
                    self.target,
                    [register_chain_ports.make_key([gc.KeyTuple('$REGISTER_INDEX', register_idx)])],
                    [register_chain_ports.make_data([
                        gc.DataTuple('Ingress.chain_ports.f1', 188)])])


        except KeyboardInterrupt:
            self.critical_error('Stopping controller.')
        except Exception as e:
            self.log.exception(e)
            self.critical_error('Unexpected error. Stopping controller.')

    def set_switch_mac_and_ip(self, switch_mac, switch_ip):
        ''' Set switch MAC and IP '''
        self.switch_mac = switch_mac.upper()
        self.switch_ip = switch_ip

    def get_switch_mac_and_ip(self):
        ''' Get switch MAC and IP '''
        return self.switch_mac, self.switch_ip


    def run(self):
        try:
            print 'Starting controller..'
            # signal interupt enabled
            signal.signal(signal.SIGINT, sigint_handler)
            
            # run
            time.sleep(2)
            print 'Register and table entries set finished.'
            exit()
            
        except Exception as e:
            self.log.exception(e)


if __name__ == '__main__':
    argparser = argparse.ArgumentParser(description='tna_parrot_hash controller.')
    argparser.add_argument('--program',
                           type=str,
                           default='tna_parrot_hash',
                           help='P4 program name. Default: tna_parrot_hash')
    argparser.add_argument('--bfrt-ip',
                            type=str,
                            default='127.0.0.1',
                            help='Name/address of the BFRuntime server. Default: 127.0.0.1')
    argparser.add_argument('--bfrt-port',
                           type=int,
                           default=50052,
                           help='Port of the BFRuntime server. Default: 50052')
    argparser.add_argument('--switch-mac',
                            type=str,
                            default='d8:9e:f3:1c:d5:17',
                            help='MAC address of the switch. Default: d8:9e:f3:1c:d5:17')
    argparser.add_argument('--switch-ip',
                            type=str,
                            default="128.114.52.212",
                            help='IP address of the switch. Default: 128.114.57.92')
    argparser.add_argument('--ports',
                            type=str,
                            default='ports.yaml',
                            help=
                            'YAML file describing machines connected to ports. Default: ports.yaml')
    argparser.add_argument('--log-level',
                            default='INFO',
                            choices=['ERROR', 'WARNING', 'INFO', 'DEBUG'],
                            help='Default: INFO')

    args = argparser.parse_args()

    # configure logging
    numeric_level = getattr(logging, args.log_level.upper(), None)
    if not isinstance(numeric_level, int):
        sys.exit('Invalid log level: {}'.format(args.log_level))

    logformat = '%(asctime)s - %(levelname)s - %(message)s'
    logging.basicConfig(filename='controller/tna_parrot_runtime.log',
                        filemode='w',
                        level=numeric_level,
                        format=logformat,
                        datefmt='%H:%M:%S')

    args.switch_mac = args.switch_mac.strip().upper()
    args.switch_ip = args.switch_ip.strip()
    args.bfrt_ip = args.bfrt_ip.strip()

    if not mac_address_regex.match(args.switch_mac):
        sys.exit('Invalid Switch MAC address')
    if not validate_ip(args.switch_ip):
        sys.exit('Invalid Switch IP address')
    if not validate_ip(args.bfrt_ip):
        sys.exit('Invalid BFRuntime server IP address')

    ctrl = Controller()
    ctrl.setup(args.program, args.switch_mac, args.switch_ip, args.bfrt_ip,
               args.bfrt_port, args.ports)

    # start controller
    ctrl.run()

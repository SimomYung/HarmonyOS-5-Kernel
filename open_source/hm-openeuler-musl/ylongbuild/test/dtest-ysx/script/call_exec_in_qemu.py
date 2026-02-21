from telnetlib import Telnet
import sys
import time
import logging
import traceback
import re

logging.basicConfig(level=logging.DEBUG)
port_num = int(sys.argv[1])
try:
    tn = Telnet("localhost", port_num, timeout=30)
    # file = open("/usr1/workspace/hpe.log", "w")
    exec_list = sys.argv[2:]
    exec_cmd = f"cd /opt/vrpv8/home/;chmod +x exec_in_qemu.sh;" \
               f"./exec_in_qemu.sh {' '.join(exec_list)}"
    tn.write(exec_cmd.encode('ascii') + b"\n")
    time.sleep(10)
    cmd_output = tn.read_until(b'\r\n').decode('unicode_escape')
    while cmd_output.strip() != "FINISHED" \
    and cmd_output.strip() != "[----------] Global test environment tear-down" \
    and not re.search(r"\[\=*\] (\d+) tests from (\d+) test suites ran.", cmd_output) \
    and not re.search(r"\[\s*PASSED\s*\] (\d+) tests.", cmd_output):
        # print(cmd_output, end='')
        # file.write(cmd_output)
        cmd_output = tn.read_until(b'\r\n').decode('unicode_escape')
    # print(cmd_output, end='')
    # file.write(cmd_output)
    # file.close()
    tn.close()
except Exception:
    logging.debug(traceback.format_exc())
    print(f"localhost with prot {port_num} 连接失败\n")

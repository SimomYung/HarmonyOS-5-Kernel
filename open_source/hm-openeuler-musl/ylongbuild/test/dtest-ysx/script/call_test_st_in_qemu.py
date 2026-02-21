from telnetlib import Telnet
import sys
import time
import logging
import traceback

persist_dir = '/opt/vrpv8/home'

st_hlibc_list = (
    #f'st_hlibc_test_st --gtest_output=xml:{persist_dir}/st_dap_test_st1-gtest-result.xml',
)

st_dap_list = (
    #'st_dap_test_st',
)

logging.basicConfig(level=logging.DEBUG)
try:
    tn = Telnet("localhost", int(sys.argv[1]), timeout=30)
    exec_cmd = f"cd {persist_dir};chmod +x exec_st_in_qemu.sh;chmod +x load_st_in_qemu.sh;"
    tn.write(exec_cmd.encode('ascii') + b"\n")
    exec_cmd = f"cp {persist_dir}/testcase/* /hpe"
    tn.write(exec_cmd.encode('ascii') + b"\n")

    exec_cmd = f"sed -i 's/ydrv.path/ydrv-script.path/g' /bin/install_ydrv.sh"
    tn.write(exec_cmd.encode('ascii') + b"\n")
    exec_cmd = f"/bin/install_ydrv.sh"
    tn.write(exec_cmd.encode('ascii') + b"\n")
    time.sleep(8)

    for exec_index in range(0, len(st_dap_list)):
        exec_cmd = "ps -w | grep vm.elf | grep hpk.bin | awk '{print $1}' | xargs -i kill -9 {}"
        tn.write(exec_cmd.encode('ascii') + b"\n")

        exec_cmd = "ipcrm -a"
        tn.write(exec_cmd.encode('ascii') + b"\n")

        print(f"-------{st_dap_list[exec_index]}-------", end='\n')

        # 更改hpe启动的app
        exec_cmd = f"./load_st_in_qemu.sh '/hpe/{st_hlibc_list[exec_index]}'"
        tn.write(exec_cmd.encode('ascii') + b"\n")
        
        # 启动hpe
        exec_cmd = f"./exec_st_in_qemu.sh"
        tn.write(exec_cmd.encode('ascii') + b"\n")
        cmd_output = tn.read_until(b'\r\n').decode('unicode_escape')
        while cmd_output.strip() != "[----------] Global test environment set-up.":
            print(cmd_output, end='')
            cmd_output = tn.read_until(b'\r\n').decode('unicode_escape')
        print(cmd_output, end='')
        
        # 启动host的app
        exec_cmd = f"./testcase/{st_dap_list[exec_index]}"
        tn.write(exec_cmd.encode('ascii') + b"\n")

        cmd_output = tn.read_until(b'\r\n').decode('unicode_escape')
        while cmd_output.strip() != "[----------] Global test environment tear-down":
            print(cmd_output, end='')
            cmd_output = tn.read_until(b'\r\n').decode('unicode_escape')
        print(cmd_output, end='')
        
        time.sleep(3)

    tn.close()
except Exception:
    logging.debug(traceback.format_exc())
    print("localhost with prot 3333 连接失败\n")
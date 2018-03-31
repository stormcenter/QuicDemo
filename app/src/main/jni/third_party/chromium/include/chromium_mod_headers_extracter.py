#!/usr/bin/env python
import os
import shutil
import sys
def print_usage_and_exit():
    print sys.argv[0] + " [chromium_src_root]" + "[out_dir]" + " [target_name]" + " [targetroot]"
    exit(1)
def copy_file(src_file_path, target_file_path):
    if os.path.exists(target_file_path):
        return
    if not os.path.exists(src_file_path):
        return
    target_dir_path = os.path.dirname(target_file_path)
    if not os.path.exists(target_dir_path):
        os.makedirs(target_dir_path)
    shutil.copy(src_file_path, target_dir_path)
def copy_all_files(source_dir, all_files, target_dir):
    for one_file in all_files:
        source_path = source_dir + os.path.sep + one_file
        target_path = target_dir + os.path.sep + one_file
        copy_file(source_path, target_path)
if __name__ == "__main__":
    if len(sys.argv) < 4 or len(sys.argv) > 5:
        print_usage_and_exit()
    chromium_src_root = sys.argv[1]
    out_dir = sys.argv[2]
    target_name = sys.argv[3]
    target_root_path = "."
    if len(sys.argv) == 5:
        target_root_path = sys.argv[4]
    target_root_path = os.path.abspath(target_root_path)
    os.chdir(chromium_src_root)
    cmd = "gn desc " + out_dir + " " + target_name
    outputs = os.popen(cmd).readlines()
    source_start = False
    all_headers = []
    public_start = False
    public_headers = []
    for output_line in outputs:
        output_line = output_line.strip()
        if output_line.startswith("sources"):
            source_start = True
            continue
        elif source_start and len(output_line) == 0:
            source_start = False
            continue
        elif source_start and output_line.endswith(".h"):
            output_line = output_line[1:]
            all_headers.append(output_line)
        elif output_line == "public":
            public_start = True
            continue
        elif public_start and len(output_line) == 0:
            public_start = False
            continue
        elif public_start:
            public_headers.append(output_line)
    if len(public_headers) == 1:
        public_headers = all_headers
    if len(public_headers) > 1:
        copy_all_files(chromium_src_root, public_headers, target_dir=target_root_path)


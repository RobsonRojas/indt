from ctypes import *

Indt1Dll = cdll.LoadLibrary("Indt1Dll.dll")

libc = cdll.msvcrt
from libc.list import list as c_list


class Table(Structure):
    _fields_ = [("table_id", c_uint),
    ("table_size", c_uint)]

class File_Parse(Structure):
    _fields_ = [("byte1", c_uint),
        ("byte2", c_uint),
        ("APT_table_count", c_uint),
        ("MPT_table_count", c_uint),
        ("ITN_table_count", c_uint),
        ("ATB_table_count", c_uint),
        ("TBA_table_count", c_uint),
        ("TAP_table_count", c_uint)
        ("tables", c_list) 
        ]


file_parse = File_Parse()

Indt1Dll.ParseFile("teste.bin", file_parse)
Indt1Dll.PrintFileParse(file_parse)

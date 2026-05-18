


import os
import sys
import struct

def align(val, alignment):
    return (val + alignment - 1) & ~(alignment - 1)

def generate_pe_image(payload_path, output_path="output.exe"):
    if not os.path.exists(payload_path):
        print(f"[-] Error: Payload file '{payload_path}' not found.")
        sys.exit(1)

    with open(payload_path, "rb") as f:
        payload_data = f.read()
        
    base_name = os.path.basename(payload_path).split(".")[0]
    section_name_str = f".{base_name}"
    if len(base_name) > 7: 
        section_name_str = f".{base_name[:6]}$"
    
    section_name = section_name_str.encode('utf-8')
    section_name = section_name.ljust(8, b'\x00')

    print(f"[+] Section name is {section_name_str}")

    FILE_ALIGN = 0x200
    SEC_ALIGN = 0x1000

    payload_raw_size = len(payload_data)
    payload_aligned_file_size = align(payload_raw_size, FILE_ALIGN)
    payload_aligned_virt_size = align(payload_raw_size, SEC_ALIGN)

    header_size = 0x200 
    
    dos_header = bytearray(64)
    struct.pack_into('<2s', dos_header, 0, b'MZ')
    struct.pack_into('<I', dos_header, 0x3C, 0x40) 

    pe_sig = b'PE\x00\x00'

    coff_header = struct.pack(
        '<HHIIIHH',
        0x8664,  
        1,       
        0,       
        0,       
        0,       
        0x0070,  
        0x0022   
    )

    image_size = align(header_size + payload_aligned_virt_size, SEC_ALIGN)
    
    opt_header = struct.pack(
        '<HBBIIIIIQIIHHHHHHIIIIHHQQQQII',
        0x020B,           
        1, 0,             
        0,                
        0,                
        0,                
        0x1000,           
        0x1000,           
        0x00400000,       
        SEC_ALIGN,        
        FILE_ALIGN,       
        0, 0,             
        0, 0,             
        0, 0,             
        0,                
        image_size,       
        header_size,      
        0,                
        1,                
        0,                
        0x100000, 0x1000, 
        0x100000, 0x1000, 
        0,                
        0                
    )
    
    #opt_header += b'\x00' * 128

    section_header = struct.pack(
        '<8sIIIIIIHHI',
        section_name,               
        payload_raw_size,           
        0x1000,                     
        payload_aligned_file_size,  
        header_size,                
        0, 0, 0, 0,                 
        0xE0000060                  
    )

    full_headers = dos_header + pe_sig + coff_header + opt_header + section_header
    full_headers = full_headers.ljust(header_size, b'\x00')

    padded_payload = payload_data.ljust(payload_aligned_file_size, b'\x00')

    with open(output_path, "wb") as f:
        f.write(full_headers)
        f.write(padded_payload)

    print(f"[+] Wrote '{output_path}'")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python make_pe.py <input_file_path> [output_pe_path]")
        sys.exit(1)
        
    infile = sys.argv[1]
    outfile = sys.argv[2] if len(sys.argv) > 2 else "output.exe"
    generate_pe_image(infile, outfile)
